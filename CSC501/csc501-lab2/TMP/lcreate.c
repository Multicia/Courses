#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <lock.h>
#include <stdio.h>

LOCAL int newlock();
int mod_counter=0;
/*------------------------------------------------------------------------
 * lcreate  --  create and initialize a lock, returning its id
 *------------------------------------------------------------------------
 */
SYSCALL lcreate(void)
{
        STATWORD ps;
        int     lock;

        disable(ps);
        if ( (lock=newlock())==SYSERR ) {
                restore(ps);
                return(SYSERR);
        }
        lockarr[lock].lockcnt = 1;
        /* sqhead and sqtail were initialized at system startup */

	/*Modifying the lock id*/
	lock = ((++mod_counter%100)*100)+lock;
	lockarr[lock%100].mod_number = mod_counter;
        restore(ps);
        return(lock);
}

/*------------------------------------------------------------------------
 * newsem  --  allocate an unused semaphore and return its index
 *------------------------------------------------------------------------
 */
LOCAL int newlock()
{
        int     lock;
        int     i;

        for (i=0 ; i<NLOCKS ; i++) {
                lock=nextlock--;
                if (nextlock < 0)
                        nextlock = NLOCKS-1;
                if (lockarr[lock].lstate==LFREE) {
                        lockarr[lock].lstate = LUSED;
                        return(lock);
                }
        }
        return(SYSERR);
}

