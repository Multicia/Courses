#include <conf.h>
#include <i386.h>
#include <kernel.h>
#include <proc.h>
#include <sem.h>
#include <mem.h>
#include <io.h>
#include <lock.h>
#include <stdio.h>

void test_func(nargs,args)
	int nargs;
	long args;
{
        unsigned long   *a;             /* points to list of args       */
        a = (unsigned long *)(&args) + (nargs-1); /* last argument      */
        for ( ; nargs > 0 ; nargs--)
                kprintf("\n%ld",*a--);
}

