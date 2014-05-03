/* queue.c - dequeue, enqueue */

#include <conf.h>
#include <kernel.h>
#include <q.h>

/*------------------------------------------------------------------------
 * enqueue  --  insert an item at the tail of a list
 *------------------------------------------------------------------------
 */
int enqueue(int item,int queue_type, int tail)
/*      int     item;                   - item to enqueue on a list     */
/*      int     tail;                   - index in q of list tail       */
{
        struct  qent    *tptr;          /* points to tail entry         */
        struct  qent    *mptr;          /* points to item entry         */

	if(queue_type == 1)
	{
	        tptr = &q1[tail];
        	ptr = &q1[item];
	        mptr->qnext = tail;
	        mptr->qprev = tptr->qprev;
	        q1[tptr->qprev].qnext = item;
	        tptr->qprev = item;
	        return(item);
	}
	else if(queue_type == 2)
	{
                tptr = &q2[tail];
                ptr = &q2[item];
                mptr->qnext = tail; 
                mptr->qprev = tptr->qprev;
                q2[tptr->qprev].qnext = item;
                tptr->qprev = item;
                return(item);

	}
	else
	{
                tptr = &q3[tail];
                ptr = &q3[item];
                mptr->qnext = tail; 
                mptr->qprev = tptr->qprev;
                q3[tptr->qprev].qnext = item;
                tptr->qprev = item;
                return(item);
	}
}

