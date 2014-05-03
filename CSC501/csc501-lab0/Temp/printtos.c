#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <stdio.h>

static unsigned long    *esp;
static unsigned long    *ebp;

void printtos()
{
        struct pentry   *proc = &proctab[getpid()];
        unsigned long   *sp;
	int a1=256;
	int a2=257;
	int a3=258;
	int a4=259;
        asm("movl %ebp,ebp");
	sp = ebp;
        kprintf("\nBefore[0x%08x]: 0x%08x", sp + 2, *(sp + 2));
        kprintf("\nAfter[0x%08x]: 0x%08x", sp, *sp);
        kprintf("\n\t element[0x%08x]: 0x%08x",(sp - 1), *(sp - 1));
        kprintf("\n\t element[0x%08x]: 0x%08x",(sp - 2), *(sp - 2));
        kprintf("\n\t element[0x%08x]: 0x%08x",(sp - 3), *(sp - 3));
        kprintf("\n\t element[0x%08x]: 0x%08x",(sp - 4), *(sp - 4));
	kprintf("\n\t element[0x%08x]: 0x%08x",(sp - 5), *(sp - 5));
        kprintf("\n\t element[0x%08x]: 0x%08x",(sp - 6), *(sp - 6));

}
