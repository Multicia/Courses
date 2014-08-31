/* sleep.c - sleep */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <sleep.h>
#include <stdio.h>

#include <lab0.h>
extern long ctr1000;
extern struct data Info[][27];

/*------------------------------------------------------------------------
 * sleep  --  delay the calling process n seconds
 *------------------------------------------------------------------------
 */
SYSCALL	sleep(int n)
{
        int start;
	int i,j;
	if(activated == 1)
		start = ctr1000;
	STATWORD ps;    
	if (n<0 || clkruns==0)
	{
		if(activated == 1)
		{
		        Info[currpid][SLEEP].freq++;
		        Info[currpid][SLEEP].time += (ctr1000 - start);
		}
		return(SYSERR);
	}
	if (n == 0) {
	        disable(ps);
		resched();
		restore(ps);
		if(activated == 1)
		{
	        	Info[currpid][SLEEP].freq++;
		        Info[currpid][SLEEP].time += (ctr1000 - start);
		}
		return(OK);
	}
	while (n >= 1000) {
		sleep10(10000);
		n -= 1000;
	}
	if (n > 0)
		sleep10(10*n);
	if(activated == 1)
	{
	        Info[currpid][SLEEP].freq++;
	        Info[currpid][SLEEP].time += (ctr1000 - start);
	}
	return(OK);
}
