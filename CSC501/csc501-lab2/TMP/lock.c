/* insert.c  -  insert */
#include <conf.h>
#include <kernel.h>
#include <lock.h>
#include <q.h>
#include <proc.h>

/*------------------------------------------------------------------------
 * insert.c  --  insert an process into a q list in key order
 *------------------------------------------------------------------------
 */
int insert_lock(int proc, int head, int key,int type)
{
        int     next;                   /* runs through list            */
        int     prev;
	int w1,w2;

        next = q[head].qnext;
	if(next <NPROC)
	{
	        while (q[next].qkey < key)      /* tail has maxint as key       */
		{
	                next = q[next].qnext;
		}
		w2 = ctr1000 - proctab[proc].waiting_lock.start_time;
		while(q[next].qkey == key)
		{
			w1 = ctr1000 - proctab[next].waiting_lock.start_time; /*waiting time in queue*/
			if((abs(w2-w1) < 1000) && type == WRITE && proctab[next].waiting_lock.type == READ)
				break;
			else if((abs(w2-w1) < 1000) && type == READ && proctab[next].waiting_lock.type == WRITE)
				next=q[next].qnext;
			else if(w2 < w1)
		                next=q[next].qnext;
			else
				break;
		}
	}	
        q[proc].qnext = next;
        q[proc].qprev = prev = q[next].qprev;
        q[proc].qkey  = key;
        q[prev].qnext = proc;
        q[next].qprev = proc;
	next = q[head].qnext;
        while (next< NPROC)      /* tail has maxint as key       */
        {
                        next = q[next].qnext;		
	}
        return(OK);
}


int find_state(int lock,int key) /*key means priority*/
{
	int head;
	int tail;
	int next;
	int pid;
	int ret=0; /* 0- False(no write exists) 1-true*/

	head = lockarr[lock%100].lqhead;
	tail = lockarr[lock%100].lqtail;
        next = q[head].qnext;
        while (q[next].qkey < key)      /* tail has maxint as key       */
        {
	       next = q[next].qnext;
	}
	while(next < NPROC)
	{
		pid = next;
		if(proctab[pid].waiting_lock.type == WRITE)
		{
			return 1;
		}	
                next = q[next].qnext;
	}
	return 0;
}
int inherit(int ldes1,int priority)
{
	int i=0;
	for(i=0;i<NPROC;i++)
	{
		if(lockarr[ldes1%100].process_lock[i] == 1)
		{
			proctab[i].pinh = priority;
			if(proctab[i].waiting_lock.lock_id != -1)
			{
				lockarr[proctab[i].waiting_lock.lock_id].maxprio = priority;
				inherit(proctab[i].waiting_lock.lock_id,priority);
			}
		}
	}
}
int lock (int ldes1, int type, int priority)
{

	STATWORD ps;
        struct  lentry  *lptr;
        struct  pentry  *pptr;
	kprintf("\n LOCK");
        disable(ps);
	/* returns system error if it's deleted */
	if(((lockarr[ldes1%100].mod_number*100)+(ldes1%100)) != ldes1)
	{
		restore(ps);
		return(SYSERR);
	}
        if (isbadlock(ldes1%100) || (lptr= &lockarr[ldes1%100])->lstate==LFREE) 
	{
                restore(ps);
                return(SYSERR);
        }

        if (--(lptr->lockcnt) < 0 )
        {
		if(lockarr[ldes1%100].maxprio == -1)
		{
			lockarr[ldes1%100].maxprio = getprio(currpid);
		}
		else
		{
			if(lockarr[ldes1%100].maxprio < getprio(currpid))
			{
				lockarr[ldes1%100].maxprio = getprio(currpid);
				inherit(ldes1,getprio(currpid));
			}
		}

		if(type == READ && lptr->lstate == READ)
		{
			if(!find_state(ldes1,priority))
			{
		                lptr->read_count++;
				pptr = &proctab[currpid];
		                pptr->holding_lock[ldes1%100].type = type;
		                pptr->holding_lock[ldes1%100].priority = priority;
		                lockarr[ldes1%100].process_lock[currpid]=1; /* the processes holding locks */
			        restore(ps);
			        return(OK);
			}
		}
                (pptr = &proctab[currpid])->pstate = PRWAIT;
		pptr->waiting_lock.lock_id = ldes1%100;
		pptr->waiting_lock.start_time=ctr1000;
                pptr->waiting_lock.type=type;
                pptr->waiting_lock.priority=priority;
                insert_lock(currpid,lptr->lqhead,priority,type);
                pptr->pwaitret = OK;
                resched();
                restore(ps);
                return pptr->pwaitret;
        }
	else
	{
		if(type==READ)
			lptr->read_count++;
		pptr = &proctab[currpid];
		pptr->holding_lock[ldes1%100].type = type;
		pptr->holding_lock[ldes1%100].priority = priority;
		lptr->process_lock[currpid]=1; /* the processes holding locks */
		lptr->maxprio = getprio(currpid);
		lptr->lstate=type;
	}


        restore(ps);
        return(OK);
}
