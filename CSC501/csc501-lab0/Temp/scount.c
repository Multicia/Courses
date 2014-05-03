/* scount.c - scount */

#include <conf.h>
#include <kernel.h>
#include <sem.h>

#include <proc.h>
#include <lab0.h>
extern long ctr1000;
extern struct data Info[][27];
/*------------------------------------------------------------------------
 *  scount  --  return a semaphore count
 *------------------------------------------------------------------------
 */
SYSCALL scount(int sem)
{
extern	struct	sentry	semaph[];
	int start;
	if(activated == 1)
		start = ctr1000;
	if (isbadsem(sem) || semaph[sem].sstate==SFREE)
	{
		if(activated == 1)
		{
			Info[currpid][SCOUNT].freq++;
	                Info[currpid][SCOUNT].time += (ctr1000 - start);
		}
	return(SYSERR);
	}
	if(activated == 1)
	{
	        Info[currpid][SCOUNT].freq++;
	        Info[currpid][SCOUNT].time += (ctr1000 - start);
	}
	return(semaph[sem].semcnt);
}
