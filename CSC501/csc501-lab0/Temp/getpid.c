/* getpid.c - getpid */
#include <lab0.h>
#include <conf.h>
#include <kernel.h>
#include <proc.h>

extern long ctr1000;
extern struct data Info[][27];
/*------------------------------------------------------------------------
 * getpid  --  get the process id of currently executing process
 *------------------------------------------------------------------------
 */
SYSCALL getpid()
{
	if(activated == 1)
	{
		int start = ctr1000;
		Info[currpid][GETPID].freq++;
	        Info[currpid][GETPID].time += (ctr1000 - start);
	}
	return(currpid);
}
