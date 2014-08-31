include <conf.h>
#include <kernel.h>
#include <q.h>


/*------------------------------------------------------------------------
 *  dequeue  --  remove an item from the head of a list and return it
 *------------------------------------------------------------------------
 */
int dequeue_random(int item,int queue_type)
{
        struct  qent    *mptr;          /* pointer to q entry for item  */

        if(queue_type ==1)
	{
		mptr = &q1[item];
	        q1[mptr->qprev].qnext = mptr->qnext;
	        q1[mptr->qnext].qprev = mptr->qprev;
        	return(item);
	}	
	else if(queue_type ==2)
	{
		mptr = &q2[item];
                q2[mptr->qprev].qnext = mptr->qnext;
                q2[mptr->qnext].qprev = mptr->qprev;
                return(item);
	}
	else
	{
		mptr = &q3[item];
                q3[mptr->qprev].qnext = mptr->qnext;
                q3[mptr->qnext].qprev = mptr->qprev;
                return(item);
	}
}

