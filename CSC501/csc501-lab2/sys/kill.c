/* kill.c - kill */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <sem.h>
#include <mem.h>
#include <lock.h>
#include <io.h>
#include <q.h>
#include <stdio.h>


void inversion(int pid)
{
	int waiting_lock_id,max=-1,i;
	waiting_lock_id = proctab[pid].waiting_lock.lock_id;
	if(lockarr[waiting_lock_id].maxprio == getprio(pid))
	{
		int head=lockarr[waiting_lock_id].lqhead;
		int next = next = q[head].qnext;
		while(next<NPROC)
		{
			if(getprio(next) && (next!=pid))
				max = getprio(next);
			next = q[next].qnext;
		}
		lockarr[waiting_lock_id].maxprio = max;
		for(i=0;i<NPROC;i++)
		{
			if(lockarr[waiting_lock_id].process_lock[i]==1)
				proctab[i].pinh = max;
		}
	}
}

/*------------------------------------------------------------------------
 * kill  --  kill a process and remove it from the system
 *------------------------------------------------------------------------
 */
SYSCALL kill(int pid)
{
	STATWORD ps;    
	struct	pentry	*pptr;		/* points to proc. table for pid*/
	int	dev;
	inversion(pid);
	disable(ps);
	if (isbadpid(pid) || (pptr= &proctab[pid])->pstate==PRFREE) {
		restore(ps);
		return(SYSERR);
	}
	if (--numproc == 0)
		xdone();

	dev = pptr->pdevs[0];
	if (! isbaddev(dev) )
		close(dev);
	dev = pptr->pdevs[1];
	if (! isbaddev(dev) )
		close(dev);
	dev = pptr->ppagedev;
	if (! isbaddev(dev) )
		close(dev);
	
	send(pptr->pnxtkin, pid);

	freestk(pptr->pbase, pptr->pstklen);
	switch (pptr->pstate) {

	case PRCURR:	pptr->pstate = PRFREE;	/* suicide */
			resched();

	case PRWAIT:	semaph[pptr->psem].semcnt++;
			int i;
			for(i=0;i<NLOCKS;i++)
			{
				if(proctab[pid].holding_lock[i].type != -1)
				{
					lockarr[i].lockcnt--;
					lockarr[i].process_lock[pid]=-1;
					if(proctab[pid].holding_lock[i].type==READ)
						lockarr[i].read_count--;
				}
			}

	case PRREADY:	dequeue(pid);
			pptr->pstate = PRFREE;
			break;

	case PRSLEEP:
	case PRTRECV:	unsleep(pid);
						/* fall through	*/
	default:	pptr->pstate = PRFREE;
	}
	restore(ps);
	return(OK);
}
