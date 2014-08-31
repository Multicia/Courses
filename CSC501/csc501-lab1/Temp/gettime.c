/* gettime.c - gettime */

#include <conf.h>
#include <kernel.h>
#include <date.h>
#include <proc.h>
#include <lab0.h>
extern long ctr1000;
extern struct data Info[][27];

extern int getutim(unsigned long *);

/*------------------------------------------------------------------------
 *  gettime  -  get local time in seconds past Jan 1, 1970
 *------------------------------------------------------------------------
 */
SYSCALL	gettime(long *timvar)
{
    /* long	now; */

	/* FIXME -- no getutim */
    int start;
    if(activated == 1)
    {
		start = ctr1000;
	        Info[currpid][GETTIME].freq++;
	        Info[currpid][GETTIME].time += (ctr1000 - start);
    }

    return OK;
}
