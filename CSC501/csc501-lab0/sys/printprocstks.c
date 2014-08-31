#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <stdio.h>

static unsigned long    *ebp;

void printprocstks(int priority)
{
	int i;
	unsigned long *sp;
	struct pentry   *proc = &proctab[0];
	kprintf("\n # Filter: %d",priority);
	for(i=0;i<NPROC;i++)
	{
		if(proc[i].pprio > priority)
		{
			kprintf("\nProcess [ %s ]",proc[i].pname);
			kprintf("\n\t pid: %d",i);
			kprintf("\n\t priority: %d ",proc[i].pprio);
			kprintf("\n\t base : 0x%08X",proc[i].pbase);
			kprintf("\n\t limit: 0x%08X",proc[i].plimit);
			//kprintf("\n\t len : %d",(proc[i].pbase - proc[i].plimit + sizeof (long)));
			kprintf("\n\t len : %d",proc[i].pstklen);
			if(i == currpid)
                        {
                                asm("movl %ebp,ebp");
                                sp=ebp;
                        }
                        else
                        {
                                sp = (unsigned long *)proc->pesp;
                                sp = sp + 2;
                        }
                        kprintf("\n\t pointer: 0x%08X \n\n",sp);
		}
	}

}
