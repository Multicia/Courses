/* sleep1000.c - sleep1000 */

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
 * sleep1000 --  delay the caller for a time specified in 1/100 of seconds
 *------------------------------------------------------------------------
 */
SYSCALL sleep1000(int n)
{
        int start;
	if(activated == 1)
		start = ctr1000;
	STATWORD ps;    

	if (n < 0  || clkruns==0)
	{
		if(activated == 1)
	        {
        	        Info[currpid][SLEEP1000].freq++;
        	        Info[currpid][SLEEP1000].time += (ctr1000 - start);
	        }
	        return(SYSERR);
	}
	disable(ps);
	if (n == 0) {		/* sleep1000(0) -> end time slice */
	        ;
	} else {
		insertd(currpid,clockq,n);
		slnempty = TRUE;
		sltop = &q[q[clockq].qnext].qkey;
		proctab[currpid].pstate = PRSLEEP;
	}
	resched();
        restore(ps);
	if(activated == 1)
	{
	        Info[currpid][SLEEP1000].freq++;
	        Info[currpid][SLEEP1000].time += (ctr1000 - start);
	}
	return(OK);
}
