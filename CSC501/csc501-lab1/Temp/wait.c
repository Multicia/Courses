/* wait.c - wait */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <sem.h>
#include <stdio.h>

#include <lab0.h>
extern long ctr1000;
extern struct data Info[][27];

/*------------------------------------------------------------------------
 * wait  --  make current process wait on a semaphore
 *------------------------------------------------------------------------
 */
SYSCALL	wait(int sem)
{
        int start;
	if(activated == 1)
		start = ctr1000;
	STATWORD ps;    
	struct	sentry	*sptr;
	struct	pentry	*pptr;

	disable(ps);
	if (isbadsem(sem) || (sptr= &semaph[sem])->sstate==SFREE) {
		restore(ps);
		if(activated == 1)
	        {        
	                Info[currpid][WAIT].freq++;
	                Info[currpid][WAIT].time += (ctr1000 - start);
	        }
		return(SYSERR);
	}
	
	if (--(sptr->semcnt) < 0) {
		(pptr = &proctab[currpid])->pstate = PRWAIT;
		pptr->psem = sem;
		enqueue(currpid,sptr->sqtail);
		pptr->pwaitret = OK;
		resched();
		if(activated == 1)
	        {        
	                Info[currpid][WAIT].freq++;
	                Info[currpid][WAIT].time += (ctr1000 - start);
	        }
		restore(ps);
		return pptr->pwaitret;
	}
	restore(ps);
	if(activated == 1)
	{
	        Info[currpid][WAIT].freq++;
	        Info[currpid][WAIT].time += (ctr1000 - start);
	}
	return(OK);
}
