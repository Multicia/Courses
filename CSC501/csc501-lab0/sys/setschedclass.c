#include <lab1.h>
#include <stdio.h>

void setschedclass(int sched_class)
{
	if(sched_class != RANDOMSCHED && sched_class != LINUXSCHED)
		kprintf("\n Invalid Input");
	else
	{
		used_scheduler = sched_class;
	}
	
}

