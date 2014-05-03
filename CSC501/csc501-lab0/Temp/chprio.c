/* chprio.c - chprio */
#include<lab0.h>
#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <stdio.h>
extern int activated;
extern long ctr1000;
extern struct data Info[][27];
/*------------------------------------------------------------------------
 * chprio  --  change the scheduling priority of a process
 *------------------------------------------------------------------------
 */
SYSCALL chprio(int pid, int newprio)
{
	int start ;
	if(activated==1)
		start = ctr1000;
	STATWORD ps;    
	struct	pentry	*pptr;

	disable(ps);
	if (isbadpid(pid) || newprio<=0 ||
	    (pptr = &proctab[pid])->pstate == PRFREE) {
		restore(ps);
		return(SYSERR);
	}
	pptr->pprio = newprio;
	restore(ps);
	if(activated == 1)
	{
	        Info[currpid][CHPRIO].freq++;
        	Info[currpid][CHPRIO].time += (ctr1000 - start);
	}
	return(newprio);
}
