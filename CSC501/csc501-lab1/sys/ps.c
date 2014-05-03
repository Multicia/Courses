#include <conf.h>
#include <kernel.h>
#include <stdio.h>
#include <proc.h>
#include <lab1.h>

void ps(void)
{
	int i,j,index,permit,k;
	unsigned long max;
	int order[NPROC];
	for(i=0;i<NPROC;i++)
		order[i]=-1;
	struct pentry *proc = &proctab[0];
//	disable(ps);
	for(i=0;i<NPROC;i++)
        {
                index=-1;
		max=0;
                for(j=0;j<NPROC;j++)
                {
			if(proc[j].pstate != PRFREE)
                        {
			        if(execution_time[j] >= max)
                                {
					permit=-1;
                                        k=0;
                                        while(order[k]!=-1)
                                        {       if(order[k]==j)
                                                        permit=1;
                                                k++;
                                        }
                                        if(permit == -1)
                                        {
                                                max = execution_time[j];
                                                index = j;
                                        }
                                }
			}
                }
                order[i]=index;
        }
//	restore(ps);
	kprintf("\n\nName\t\tPID\tPriority\tStatus\t\tTicks\tStackSize");
	k=0;
	while(order[k]!=-1)
	{
		i = order[k];
		if(proc[i].pstate != PRFREE)
		{

			kprintf("\n%s \t\t",proc[i].pname);
			kprintf("%d\t",i);
			kprintf("%d\t\t",proc[i].pprio);
			if(proc[i].pstate == PRCURR)
				kprintf("PRCURR\t");
			else if(proc[i].pstate == PRRECV)
				kprintf("PRRECV\t");
			else if(proc[i].pstate == PRREADY)
				kprintf("PRREADY\t");
			else if(proc[i].pstate == PRSLEEP)
				kprintf("PRSLEEP\t");
			else if(proc[i].pstate == PRSUSP)
				kprintf("PRSUSP\t");
			else if(proc[i].pstate == PRWAIT)
				kprintf("PRWAIT\t");
			else if(proc[i].pstate == PRTRECV)
				kprintf("PRTRECV\t");
			kprintf("\t%ld\t",execution_time[i]);
			kprintf("%d",proc[i].pstklen);
		}
		k++;
	}
	kprintf("\n");

}
