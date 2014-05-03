#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <lock.h>
#include <stdio.h>

int ldelete (int lockdescriptor)
{
        STATWORD ps;
        int     pid;
        struct  lentry  *lptr;

        disable(ps);
        if (isbadlock(lockdescriptor%100) || lockarr[lockdescriptor%100].lstate==LFREE) {
                restore(ps);
                return(SYSERR);
        }
        lptr = &lockarr[lockdescriptor%100];
        lptr->lstate = LFREE;
        if (nonempty(lptr->lqhead)) {
                while( (pid=getfirst(lptr->lqhead)) != EMPTY)
                  {
                    proctab[pid].pwaitret = DELETED;
                    ready(pid,RESCHNO);
                  }
                resched();
        }
        restore(ps);
        return(OK);
}
