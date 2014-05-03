/* frame.c - manage physical frames */
#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <paging.h>
#include <stdio.h>
#define FNUM(x) ((x)/0x1000)
#define FRAMEBASEADDRESS(x) (PAGESIZE*(x))
void insertq();
fr_map_t* getnext();

unsigned int init_ps()
{

    int i,j;
    unsigned int pdbaddr = 0x400000 + get_frame(1,FR_DIR,0)*NBPG;
    pd_t * pdp = (pd_t*)pdbaddr;
    bzero(pdp,NBPG);
    for(i=0;i<4;i++)
    {
        unsigned int nullpdbaddr = proctab[0].pdbr;
        pd_t * nullpd = (pd_t *)nullpdbaddr;

        (pdp+i)->pd_pres = 1;
        (pdp+i)->pd_write = 1;
        (pdp+i)->pd_base = (nullpd+i)->pd_base;
    }
    return pdbaddr;
}

unsigned int create_ps()
{
    int i,j;
    
    unsigned int pdbaddr = 0x00400000 + get_frame(1,FR_DIR,0)*NBPG;
    pd_t * pdp = (pd_t*)pdbaddr;
    bzero(pdbaddr,NBPG);
    for(i=0;i<4;i++)
    {
        unsigned int pdtaddr = 0x00400000 + get_frame(1,FR_TBL,0)*NBPG;
	bzero(pdtaddr,NBPG);
        pt_t * ptp = (pt_t *)pdtaddr;
	unsigned int temp_ptp = ptp;


        (pdp+i)->pd_pres = 1;
        (pdp+i)->pd_write = 1;
        (pdp+i)->pd_base = pdtaddr >> 12;
        for(j=0;j<1024;j++)
        {
            (ptp+j)->pt_pres = 1;
            (ptp+j)->pt_write = 1;
            (ptp+j)->pt_base =(i*1024 + j);
        }
    }
/*	{
        unsigned int *a;
        a = 0x400000;
        int i,j;
        i=j=0;	
        for(i=0;i<4096;i+=1024)
	        {
                	for(j=i;j<i+1024;j++)
	                {
                	        kprintf("%x ",*a);
				a++;
        	       	 }
			kprintf("\n");
        	}

	}*/
    return pdbaddr;
}


/*-------------------------------------------------------------------------
 * init_frm - initialize frm_tab
 *-------------------------------------------------------------------------
 */
SYSCALL init_frm()
{
	int  i;
	for(i = 0 ;i < NFRAMES; i++)
	{
		frm_tab[i].fr_status = FRM_UNMAPPED;
		frm_tab[i].next = NULL;
		frm_tab[i].fr_loadtime = 0;
	}
	return OK;
}

/*-------------------------------------------------------------------------
 * get_frm - get a free frame according page replacement policy
 *-------------------------------------------------------------------------
 */
SYSCALL get_frame(int frame_num, int frametype, int virt_page)
{
	int  i;
	fr_map_t *rp;
	for( i = 0; i < NFRAMES; i++)
		if(frm_tab[i].fr_status == FRM_UNMAPPED)
		{
			frm_tab[i].fr_status = FRM_MAPPED;
			frm_tab[i].fr_type = frametype;
			frm_tab[i].fr_pid = currpid;
	                frm_tab[i].fr_vpno = virt_page;
        	        frm_tab[i].next = NULL;
                	if(frametype == FR_PAGE)
				insertq(i);
			return i;
		}
	
	// No free frames available Page Replacement required
	rp = (fr_map_t *) getnext();
	if(rp == NULL)
	{
        	kprintf("ERROR!!\n");
		return SYSERR;
	}
	else
	{
		kprintf("Frame %d is replaced\n",rp-frm_tab+NFRAMES);
	}
	//else //Not required
	//        pageq.next = rp->next;

	for(i=0; i<=MAX_ID; i++)
	{
        	if(bsm_tab[i].bs_status == BSM_MAPPED && bsm_tab[i].bs_pid == rp->fr_pid && rp->fr_vpno >= bsm_tab[i].bs_vpno && (rp->fr_vpno < bsm_tab[i].bs_vpno + bsm_tab[i].bs_npages ))
	        {
	            write_bs((char*)(0x400000+(rp-frm_tab)*NBPG),i,rp->fr_vpno-bsm_tab[i].bs_vpno);
	            //kprintf("bsm_vpno: %x, fr_vpno: %x\n",rp->fr_vpno,bsm_tab[i].bs_vpno);
	            unsigned int * phyaddr = (unsigned int*)(0x800000 + (i<<20) + (rp->fr_vpno-bsm_tab[i].bs_vpno)*NBPG);
	            //kprintf("%x\n",i);
	            int * maddr = (int*)(0x400000 + (rp-frm_tab)*NBPG);
	            frm_tab[rp-frm_tab].fr_status = FRM_MAPPED;
	            frm_tab[rp-frm_tab].fr_type = frametype;
	            frm_tab[rp-frm_tab].fr_pid = currpid;
	            frm_tab[rp-frm_tab].fr_vpno = virt_page; 
	            frm_tab[rp-frm_tab].fr_loadtime = 128; 
	            frm_tab[rp-frm_tab].next = NULL; 
	            insertq(rp-frm_tab);
	            return rp-frm_tab;
	        }
	}
  
  return SYSERR;
}
void insertq(int k)
{
	if(page_replace_policy == SC)
	{
		if(myheadq == NULL)
		{
			myheadq = &frm_tab[k];
			myheadq->next = myheadq;
			currq = pcurrq = myheadq;
			//kprintf("$$$$$$$$$$$$$$curr  %x \n",currq);
			//kprintf("$$$$$$$$$$$$$$prev  %x \n",pcurrq);

		}
		else
		{
			pcurrq->next = &frm_tab[k];
			pcurrq = pcurrq->next;
			pcurrq->next = currq;
                        //kprintf("$$$$$$$$$$$$$$curr  %x \n",currq);
                        //kprintf("$$$$$$$$$$$$$$prev  %x \n",pcurrq);
		}
	}
	else if(page_replace_policy == NRU)
	{
                if(myheadq == NULL)
                {
                        myheadq = &frm_tab[k];
                        myheadq->next = myheadq;
                        currq = pcurrq = myheadq;
                        //kprintf("$$$$$$$$$$$$$$curr  %x \n",currq);
                        //kprintf("$$$$$$$$$$$$$$prev  %x \n",pcurrq);

                }
                else
                {
                        pcurrq->next = &frm_tab[k];
                        pcurrq = pcurrq->next;
                        pcurrq->next = currq;
                        //kprintf("$$$$$$$$$$$$$$curr  %x \n",currq);
                        //kprintf("$$$$$$$$$$$$$$prev  %x \n",pcurrq);
                }

	}
	else
		return SYSERR;
}
/*-------------------------------------------------------------------------
 * free_frm - free a frame 
 *-------------------------------------------------------------------------
 */
SYSCALL free_frm(int i)
{
  if(frm_tab[i].fr_status == FRM_UNMAPPED)
      return OK;
  else
  {
      frm_tab[i].fr_status = FRM_UNMAPPED;
  }
  return OK;
}
unsigned long getOffsetAddress(int frameNumber, int offset)
{
	if(offset >= 1024 || offset < 0)
	{
		kprintf("getOffsetAddress: Something is wrong");
		return SYSERR;
	}
	unsigned long result = FRAMEBASEADDRESS(frameNumber) + (unsigned long)(offset*sizeof(long));
	return result;

}

SYSCALL writeLong(int frame, int offset, unsigned long value)
{
	if((frame < 1024 || frame > 4095) || (offset < 0  || offset > 1024))
		return SYSERR;
	unsigned long *data = (unsigned long *)getOffsetAddress(frame, offset);
	*data = value;
	return OK;

}

SYSCALL readLong(int frame , int offset, unsigned long **data)
{
	if((frame < 1024 || frame > 4095) || (offset < 0  || offset > 1024))
			return SYSERR;
	*data = (unsigned long *)getOffsetAddress(frame, offset);
	return OK;
}
pt_t* get_ptentry(int framenum){
	struct pentry *pd;
	int i,j;
	unsigned long pdbr;
	pd = &proctab[currpid];
	pdbr = pd->pdbr;
	unsigned int *temp = (unsigned int *)(pdbr);
	for(i=0;i<1024;i++)
	{
		pd_t *ptp = (pd_t *) temp;
		if (ptp->pd_pres == 1){
			unsigned int *temp2 = (unsigned int *)(ptp->pd_base*NBPG);
			for(j=0;j<1024;j++)
			{
				pt_t *ptt = (pt_t *) temp2;
				if (ptt->pt_pres == 1 && ptt->pt_base == framenum){
					return ptt;
				}
				temp2++;		
			}
		}
		temp++;		
	}	
}
fr_map_t * getnext()
{
	fr_map_t *p,*q,*leastp;
	if(page_replace_policy == SC)
	{
		while(currq !=NULL)	//INFINITE LOOP
		{
			unsigned int temp = (currq-frm_tab);
			temp = (temp+NFRAMES);
			pt_t *ptp = (pt_t *)get_ptentry(temp);
			if(ptp->pt_acc){
				ptp->pt_acc = 0;
				pcurrq = currq;
				currq = currq->next;
			}
			else
			{
				pcurrq->next = currq->next;
				p = currq;
				currq=currq->next;
				return p;
			}
		}
		
	}
	else if(page_replace_policy == NRU)
	{
                while(currq !=NULL)     //INFINITE LOOP
                {
                        unsigned int temp = (currq-frm_tab);
                        temp = (temp+NFRAMES);
                        pt_t *ptp = (pt_t *)get_ptentry(temp);
                        if(ptp->pt_acc){
                                ptp->pt_acc = 0;
                                pcurrq = currq;
                                currq = currq->next;
                        }
                        else
                        {
                                pcurrq->next = currq->next;
                                p = currq;
                                currq=currq->next;
                                return p;
                        }
                }
	}
}
