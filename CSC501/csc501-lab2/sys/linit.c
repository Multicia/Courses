#include <lock.h>
#include <q.h>

void linit(void)
{
        struct  lentry  *lptr;
	int i;
        for (i=0 ; i<NLOCKS ; i++) 
	{      /* initialize locks */
                (lptr = &lockarr[i])->lstate = LFREE;
                lptr->lqtail = 1 + (lptr->lqhead = newqueue());
        }

}
