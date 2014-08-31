#include <conf.h>
#include <i386.h>
#include <kernel.h>
#include <proc.h>
#include <sem.h>
#include <mem.h>
#include <io.h>
#include <lock.h>
#include <q.h>
#include <stdio.h>

int find_priority(int tail)
{
	int temp_proc = q[tail].qprev;
	int temp_type,ret;
	while(temp_proc<NPROC)
	{
		temp_type = proctab[temp_proc].waiting_lock.type;
		if(temp_type == WRITE)
			return(proctab[temp_proc].waiting_lock.priority);			
		temp_proc=q[temp_proc].qprev;
	}
	return -1;
}

int releaseall(nargs,args)
        int nargs;
        int args;
{
        int   *a;             /* points to list of args       */
        int i,temp_lock,temp_type,temp_tail,temp_proc;
	int ret = OK;
	int temp_nargs=nargs;
	int priority;
        STATWORD ps;
        register struct lentry  *lptr;
        disable(ps);

	a = (int *)(&args) + (nargs-1); /* last argument      */
        for ( ; temp_nargs > 0 ; temp_nargs--)
                *a--;
	a++;
	for (i=0;i<nargs;i++)
	{
		temp_lock=a[i]%100;
		if(proctab[currpid].holding_lock[temp_lock].type==-1)
			ret=SYSERR;
		if(isbadlock(temp_lock) || (lptr = &lockarr[temp_lock])->lstate == LFREE)
			ret=SYSERR;
		//Book keeping
		lptr->process_lock[currpid]=0;
		proctab[currpid].holding_lock[temp_lock].type=-1;
		proctab[currpid].holding_lock[temp_lock].priority=-1;


		if(lptr->lstate == READ)
			--lptr->read_count;

		if ((lptr->lockcnt++) < 0 && (lptr->read_count) == 0)
		{
			temp_tail = lptr->lqtail;
			temp_proc = q[temp_tail].qprev;
			if(temp_proc < NPROC)
			{
				temp_type = proctab[temp_proc].waiting_lock.type;
				if(temp_type == READ)
				{
					priority=find_priority(lptr->lqtail);
					while(temp_proc < NPROC)
					{
	                                        
						if(proctab[temp_proc].waiting_lock.priority >= priority && proctab[temp_proc].waiting_lock.type == READ)
						{
							ready(dequeue(temp_proc), RESCHNO);
							//book keeping
					                proctab[temp_proc].holding_lock[temp_lock].type=READ;
					                proctab[temp_proc].holding_lock[temp_lock].priority=proctab[temp_proc].waiting_lock.priority;
							proctab[temp_proc].waiting_lock.priority=-1;
							proctab[temp_proc].waiting_lock.start_time=-1;
							proctab[temp_proc].waiting_lock.type=-1;

							lptr->read_count++;
							lptr->lstate = READ;
							lptr->process_lock[temp_proc]=1;
						}
						temp_proc = q[temp_proc].qprev;
					}
				}
				else if(temp_type == WRITE)
				{
			                ready(getlast(lptr->lqtail), RESCHYES);
				
                                        //book keeping
                                        proctab[temp_proc].holding_lock[temp_lock].type=WRITE;
                                        proctab[temp_proc].holding_lock[temp_lock].priority=proctab[temp_proc].waiting_lock.priority;
                                        proctab[temp_proc].waiting_lock.priority=-1;
                                        proctab[temp_proc].waiting_lock.start_time=-1;
                                        proctab[temp_proc].waiting_lock.type=-1;

                                        lptr->lstate = WRITE;
                                        lptr->process_lock[temp_proc]=1;
				}
			}
		} 
	}
	int j,max=-1,temp_max;
        {
		max=-1;
		temp_lock = a[i]%100;	
		for(j=0;j<NLOCKS;j++)
		{
			if(proctab[currpid].holding_lock[j].type != -1)
			{
				if(max<lockarr[i].maxprio)
					max = lockarr[i].maxprio;
			}
		}
		proctab[currpid].pinh = max; //This means the process holds no other lock so pinh is made as default value.
	}

	
	restore(ps);
	return(ret);
	
}

