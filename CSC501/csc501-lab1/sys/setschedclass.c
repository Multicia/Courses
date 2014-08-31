#include <lab1.h>
#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <stdio.h>

void setschedclass(int sched_class)
{
	int i;
        struct pentry *proc = &proctab[0];
	if(sched_class == used_scheduler)
		return;
	
	if(sched_class != RANDOMSCHED && sched_class != LINUXSCHED)
		kprintf("\n Invalid Input");
	else
	{
		used_scheduler = sched_class;
	}
	
	if(used_scheduler==RANDOMSCHED)
		for(i=0;i<NPROC;i++)
			proc[i].arrival_time = 0;
	else if(used_scheduler==LINUXSCHED)
	{
                for(i=0;i<NPROC;i++)
		{
			proc[i].quantum_left=0;
			proc[i].count =0;
		}
		if(preempt > 0)
	                proc[currpid].quantum_left = preempt; 
	}


	
}

