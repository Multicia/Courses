/* xm.c = xmmap xmunmap */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <paging.h>


/*-------------------------------------------------------------------------
 * xmmap - xmmap
 *-------------------------------------------------------------------------
 */
SYSCALL xmmap(int virtpage, bsd_t source, int npages)
{
/* sanity check ! */

	if ((virtpage < 4096)|| ( source < 0 ) || ( source > MAX_ID) ||(npages < 1) || ( npages >128))
	{
		kprintf("xmmap call error: parameter error! \n");
		return SYSERR;
	}
	else
	{
		//kprintf(">>>>>>>>>>>>>>>>>>> %d \n",virtpage);
		return bs_add_mapping(virtpage,source, npages, currpid);
	}


  return SYSERR;
}



/*-------------------------------------------------------------------------
 * xmunmap - xmunmap
 *-------------------------------------------------------------------------
 */
SYSCALL xmunmap(int virtpage )
{
  /* sanity check ! */
  if ( (virtpage < 4096) ){ 
	kprintf("xmummap call error: virtpage (%d) invalid! \n", virtpage);
	return SYSERR;
  }

  int i;
//  kprintf("xmunmap implemented!");

  for(i=0;i<=MAX_ID;i++)
  {
      if(bsm_tab[i].bs_status == BSM_MAPPED && bsm_tab[i].bs_vpno <= virtpage && (bsm_tab[i].bs_vpno+bsm_tab[i].bs_npages) > virtpage)
      {
          bsm_tab[i].bs_status = BSM_UNMAPPED;
          unsigned int pdbaddr = proctab[currpid].pdbr;
          pd_t * pdep = (pd_t*)pdbaddr + (bsm_tab[i].bs_vpno>>10);  
          int page=0;
          //write the page that have accessed back to the backing store
          if(pdep->pd_pres != 0)
          {
                pt_t * ptbp = (pt_t*)(pdep->pd_base << 12);
                pt_t * ptep = &ptbp[bsm_tab[i].bs_vpno&0x3ff];
                while((ptep-ptbp) < 1024 && page < bsm_tab[i].bs_npages)
                {
                    if(ptep->pt_pres == 1)
                    {
                        write_bs((char*)(ptep->pt_base<<12),i,page);
                        ptep->pt_pres = 0;
                        int index = ((ptep->pt_base<<12)-1024*NBPG)/NBPG;
                        frm_tab[index].fr_status = FRM_UNMAPPED;
                    }
                    page++;
                    ptep++;
                }
          }
          else
              page = 1024 - (bsm_tab[i].bs_vpno&0x3ff);
          //if the mapping virtual address is in different page table
          if(page < bsm_tab[i].bs_npages)
          {
             pdep++;
             if(pdep->pd_pres != 0)
             {
                pt_t * ptbp = (pt_t*)(pdep->pd_base << 12);
                pt_t * ptep = &ptbp[0];
                while((ptep-ptbp) < 1024 && page < bsm_tab[i].bs_npages)
                {
                    if(ptep->pt_pres == 1)
                    {
                        write_bs((char*)(ptep->pt_base<<12),i,page);
                        ptep->pt_pres = 0;
                        int index = ((ptep->pt_base<<12)-1024*NBPG)/NBPG;
                        frm_tab[index].fr_status = FRM_UNMAPPED;
                    }
                    page++;
                    ptep++;
                }
             }
          }
          return OK;
      }
  }
  return SYSERR;
}

