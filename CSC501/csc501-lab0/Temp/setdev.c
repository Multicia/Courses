/* setdev.c - setdev */

#include <conf.h>
#include <kernel.h>
#include <proc.h>

#include <lab0.h>
extern long ctr1000;
extern struct data Info[][27];
/*------------------------------------------------------------------------
 *  setdev  -  set the two device entries in the process table entry
 *------------------------------------------------------------------------
 */
SYSCALL	setdev(int pid, int dev1, int dev2)
{
	int start;
	if(activated == 1)
		start = ctr1000;
	short	*nxtdev;

	if (isbadpid(pid))
		return(SYSERR);
	nxtdev = (short *) proctab[pid].pdevs;
	*nxtdev++ = dev1;
	*nxtdev = dev2;
	if(activated == 1)
	{
	        Info[currpid][SETDEV].freq++;
	        Info[currpid][SETDEV].time += (ctr1000 - start);
	}
	return(OK);
}
