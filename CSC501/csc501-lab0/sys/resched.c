/* resched.c  -  resched */
#include <lab1.h>
#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>


unsigned long currSP;	/* REAL sp of current process */
extern int ctxsw(int, int, int, int);
/*-----------------------------------------------------------------------
 * resched  --  reschedule processor to highest priority ready process
 *
 * Notes:	Upon entry, currpid gives current process id.
 *		Proctab[currpid].pstate gives correct NEXT state for
 *			current process if other than PRREADY.
 *------------------------------------------------------------------------
 */
int resched()
{
	register struct	pentry	*optr;	/* pointer to old process entry */
	register struct	pentry	*nptr;	/* pointer to new process entry */

	/* no switch needed if current process priority higher than next*/

	if ( ( (optr= &proctab[currpid])->pstate == PRCURR) &&
	   (lastkey(rdytail)<optr->pprio)) 
	{
================//Lab1_q1: When only one process is running on the system==============================//
		execution_time[currpid]+=1;
=======================================================================================================//
		return(OK);
	}
	
	/* force context switch */

	if (optr->pstate == PRCURR) 
	{
//Project1:Ques2: 
//1. Generate a Random Number
//2. Based on arrival time insert the data into corresponding queue
		optr->pstate = PRREADY;
		insert(currpid,rdyhead,optr->pprio);
	}

	/* remove highest priority process at end of ready list */
//===============Lab1_q1: Case where resched is called after the expiry of Quantum=====================//
	if(preempt != QUANTUM)
		execution_time[currpid]+=(QUANTUM-preempt);
	else 
		execution_time[currpid]+=QUANTUM;
//====================================================================================================//
	nptr = &proctab[ (currpid = getlast(rdytail)) ];
	nptr->pstate = PRCURR;		/* mark it currently running	*/
#ifdef	RTCLOCK
	preempt = QUANTUM;		/* reset preemption counter	*/
#endif
	
	ctxsw((int)&optr->pesp, (int)optr->pirmask, (int)&nptr->pesp, (int)nptr->pirmask);
	
	/* The OLD process returns here when resumed. */
	return OK;
}
