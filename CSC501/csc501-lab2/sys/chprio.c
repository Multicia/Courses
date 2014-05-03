/* chprio.c - chprio */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <lock.h>
#include <q.h>
#include <stdio.h>

void inversion_chprio(int pid,int newprio)
{
        int waiting_lock_id,max=-1,i;
        waiting_lock_id = proctab[pid].waiting_lock.lock_id;
        if(lockarr[waiting_lock_id].maxprio == getprio(pid))
	{
		if(getprio(pid)<newprio)
		{
			lockarr[waiting_lock_id].maxprio = newprio;
			for(i=0;i<NPROC;i++)
                	{
	                        if(lockarr[waiting_lock_id].process_lock[i]==1)
	                                proctab[i].pinh = newprio;
	                }
		}
		else
		{
	                int head=lockarr[waiting_lock_id].lqhead;
	                int next = q[head].qnext;
	                while(next<NPROC)
	                {
	                        if(max < getprio(next) && (next!=pid))
	                                max = getprio(next);
	                        next = q[next].qnext;
	                }
			if(max<newprio)
				max=newprio;
	                lockarr[waiting_lock_id].maxprio = max;
	                for(i=0;i<NPROC;i++)
	                        if(lockarr[waiting_lock_id].process_lock[i]==1)
	                                proctab[i].pinh = max;
		}
	}
}


/*------------------------------------------------------------------------
 * chprio  --  change the scheduling priority of a process
 *------------------------------------------------------------------------
 */
SYSCALL chprio(int pid, int newprio)
{
	STATWORD ps;    
	struct	pentry	*pptr;

	disable(ps);
	if (isbadpid(pid) || newprio<=0 ||
	    (pptr = &proctab[pid])->pstate == PRFREE) {
		restore(ps);
		return(SYSERR);
	}
        inversion_chprio(pid,newprio);
	pptr->pprio = newprio;
	if(pptr -> pstate == PRREADY) {
		dequeue(pid);
		insert(pid, rdyhead, pptr -> pprio);
	}
	restore(ps);
	return(newprio);
}
