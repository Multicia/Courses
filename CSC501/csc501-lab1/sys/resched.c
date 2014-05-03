/* resched.c  -  resched */
#include <lab1.h>
#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>

extern long ctr1000;
unsigned long currSP;	/* REAL sp of current process */
extern int ctxsw(int, int, int, int);
void new_epoch();
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
	int rand_num,temp_currpid,i;
	/* no switch needed if current process priority higher than next*/

        if(used_scheduler == RANDOMSCHED)
        {
		optr= &proctab[currpid];
	}
        else if(used_scheduler == LINUXSCHED)
        {
		optr= &proctab[currpid];
	}
	else
	{

		if ( ( (optr= &proctab[currpid])->pstate == PRCURR) &&
	   (lastkey(rdytail)<optr->pprio)) 
		{
			return(OK);
		}
	}
	
	/* force context switch */

	if (optr->pstate == PRCURR) 
	{
		optr->pstate = PRREADY;
		optr->arrival_time = ctr1000;
		if(preempt >= 0)
			optr->quantum_left = preempt;
		insert(currpid,rdyhead,optr->pprio);
	}

	/* remove highest priority process at end of ready list */
	if(used_scheduler != LINUXSCHED)
        {
		execution_time[currpid]+=(QUANTUM-preempt);
	}
	else
		execution_time[currpid]+= (proctab[currpid].quantum_left -preempt);
	//kprintf("\nPreempt: %d",preempt);
	

        if(used_scheduler == RANDOMSCHED)
        {
		if(q[rdyhead].qnext == rdytail)
			return(OK);
		do
		{
			rand_num = rand()%100;
		//	kprintf("Random Number: %d",rand_num);
			if(rand_num >=80)
				 temp_currpid = random_getpid(1);
			else if(rand_num <80 && rand_num >=50)
                                 temp_currpid = random_getpid(2);			
			else	
                                 temp_currpid = random_getpid(3);

		}while(temp_currpid == -1);
		currpid = temp_currpid;
		nptr = &proctab[currpid];
		dequeue(currpid);
		//kprintf("\n Current PID : %d",currpid);
	
        }
	else if(used_scheduler == LINUXSCHED)
        {
		temp_currpid = linux_getpid();
		if(temp_currpid == -1)
		{
			 new_epoch();
                         temp_currpid = linux_getpid();
			 if(temp_currpid == -1)
        		 {
				temp_currpid=0;;
			 }	
			
		}
                currpid = temp_currpid;
                nptr = &proctab[currpid];
                dequeue(currpid);		
	}
	else
	{
		nptr = &proctab[ (currpid = getlast(rdytail)) ];
	}
	nptr->pstate = PRCURR;		/* mark it currently running	*/
#ifdef	RTCLOCK
	if(used_scheduler != LINUXSCHED)
		preempt = QUANTUM;		/* reset preemption counter	*/
	else
	{		preempt = proctab[currpid].quantum_left;
	}
#endif
	
	ctxsw((int)&optr->pesp, (int)optr->pirmask, (int)&nptr->pesp, (int)nptr->pirmask);
	
	/* The OLD process returns here when resumed. */
	return OK;
}


int random_getpid(int type)
{
	int hdr;
	long min_arrival=-1;
	int pid=-1;
	struct pentry *proc = &proctab[0];
	hdr=q[rdyhead].qnext;
	//kprintf("\n");
	while(hdr!=rdytail)
	{
		//kprintf(" %d %d %d",hdr,rdyhead,rdytail);
		if(type == 1)
		{	if(q[hdr].qkey < 33)
			{
				if(min_arrival == -1)
				{
					min_arrival = proc[hdr].arrival_time;
					pid = hdr;
				}
				else
				{	if(proc[hdr].arrival_time < min_arrival)
					{
						min_arrival = proc[hdr].arrival_time;
						pid =hdr;
					}
				}
			}
		}
                else if(type == 2)
                {        if(q[hdr].qkey >= 33 && q[hdr].qkey < 66)
			{
                                if(min_arrival == -1)
                                {
                                        min_arrival = proc[hdr].arrival_time;
                                        pid = hdr;
                                }
                                else
                                {        if(proc[hdr].arrival_time < min_arrival)
                                        {
                                                min_arrival = proc[hdr].arrival_time;
                                                pid =hdr;
                                        }
				}

			}
		}
                else if(type == 3)
                {
			if(q[hdr].qkey >= 66)
                        {
                                if(min_arrival == -1)
                                {
                                        min_arrival = proc[hdr].arrival_time;
                                        pid = hdr;
                                }
                                else
                                {        if(proc[hdr].arrival_time < min_arrival)
                                        {
                                                min_arrival = proc[hdr].arrival_time;
                                                pid =hdr;
                                        }
				}

                        }
		}
		hdr=q[hdr].qnext;
		
	}
	if(pid != -1)
		return pid;
	return -1;
}
int linux_getpid()
{
        struct pentry *proc = &proctab[0];
	int hdr=q[rdyhead].qnext;
	int max_goodness=-1,index=-1;
        while(hdr!=rdytail)
        {
		if(max_goodness < (proc[hdr].old_priority + proc[hdr].quantum_left) && proc[hdr].quantum_left >0)
		{
			max_goodness = (proc[hdr].old_priority + proc[hdr].quantum_left);
			index = hdr;
		}
		else if(max_goodness ==  (proc[hdr].old_priority + proc[hdr].quantum_left))
		{
			if(proc[index].count >proc[hdr].count)
				index =hdr;
		}
		hdr=q[hdr].qnext;
	}
	if(index!= -1)
		proc[index].count++;
	return index;
}
void new_epoch()
{
	int i;
        struct pentry *proc = &proctab[0];
        for(i=0;i<NPROC;i++)
	{
		if(proc[i].pstate != PRFREE)
                {
			proc[i].old_priority = proc[i].pprio;
			proc[i].quantum_left = (proc[i].quantum_left/2) + proc[i].pprio; 
			proc[i].count = 0;
		}
	}

}
