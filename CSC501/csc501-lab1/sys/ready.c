/* ready.c - ready */
#include <lab1.h>
#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
extern long ctr1000;
/*------------------------------------------------------------------------
 * ready  --  make a process eligible for CPU service
 *------------------------------------------------------------------------
 */
int ready(int pid, int resch)
{
	register struct	pentry	*pptr;

	if (isbadpid(pid))
		return(SYSERR);
	pptr = &proctab[pid];
	pptr->pstate = PRREADY;
	//Project1:Ques2: Have 3 queues and insert the values accordingly
	if(used_scheduler == RANDOMSCHED)
	{
		pptr->arrival_time = ctr1000;
	}

	insert(pid,rdyhead,pptr->pprio);
	if (resch)
		resched();
	return(OK);
}
