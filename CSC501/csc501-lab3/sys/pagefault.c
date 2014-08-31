
#include <conf.h>
#include <kernel.h>
#include <sleep.h>
#include <i386.h>
#include <stdio.h>
#include <q.h>

void pagefault()
{
	kprintf("pagefault");
}
