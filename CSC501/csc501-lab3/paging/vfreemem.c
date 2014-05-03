#include <conf.h>
#include <kernel.h>
#include <mem.h>
#include <proc.h>

extern struct pentry proctab[];
/*------------------------------------------------------------------------
 *  vfreemem  --  free a virtual memory block, returning it to vmemlist
 *------------------------------------------------------------------------
 */
SYSCALL vfreemem(block, size)
        struct  mblock  *block;
        unsigned size;
{
        STATWORD ps;
        struct  mblock  *p, *q, *temp_block;
        unsigned top;
       temp_block = (struct mblock*)(2048*NBPG+proctab[currpid].store*128*NBPG + ((unsigned int)block - 4096*NBPG));
        unsigned int hmaxaddr = 2048*NBPG + proctab[currpid].store*128*NBPG + proctab[currpid].vhpnpages*NBPG - 1;
        //kprintf("%x\n",hmaxaddr);
        unsigned int hend = 2048*NBPG + proctab[currpid].store*128*NBPG;

        if (size==0 || (unsigned)block>hmaxaddr || ((unsigned)block)< hend)
                return(SYSERR);
        size = (unsigned)roundmb(size);
        kprintf("vfreemem: %x, size: %d\n",block,size);
        disable(ps);
        p=proctab[currpid].vmemlist.mnext;
        q= &proctab[currpid].vmemlist;
        while(p != (struct mblock *) NULL && p <temp_block)
        {
                q=p;
                p=p->mnext;
        }
        /*
        for( p=proctab[currpid].vmemlist.mnext,q= &proctab[currpid].vmemlist;
             p != (struct mblock *) NULL && p <temp_block ;
             q=p,p=p->mnext )
                ;
        */

        if (((top=q->mlen+(unsigned)q)>(unsigned)block && q!= &proctab[currpid].vmemlist) || (p!=NULL && (size+(unsigned)block) > (unsigned)p ))
        {
                restore(ps);
                return(SYSERR);
        }
        if ( q!= &proctab[currpid].vmemlist && top == (unsigned)block )
                q->mlen += size;
        else
        {
               temp_block->mlen = size;
               temp_block->mnext = p;
                q->mnext =temp_block;
                q =temp_block;
        }
        if ( (unsigned)( q->mlen + (unsigned)q ) == (unsigned)p)
        {
                q->mlen += p->mlen;
                q->mnext = p->mnext;
        }
        restore(ps);
        return(OK);
}

