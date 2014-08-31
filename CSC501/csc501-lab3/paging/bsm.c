/* bsm.c - manage the backing store mapping*/

#include <conf.h>
#include <kernel.h>
#include <paging.h>
#include <proc.h>

/*-------------------------------------------------------------------------
 * init_bsm- initialize bsm_tab
 *-------------------------------------------------------------------------
 */
SYSCALL init_bsm()
{
	int i;
	for(i=0;i<NUM_BACKING_STORE;i++)
	{
		bsm_tab[i].bs_status = BSM_UNMAPPED;
		bsm_tab[i].next=NULL;
  		bsm_tab[i].bs_pid = -1;
		bsm_tab[i].bs_vpno = -1;	
  		bsm_tab[i].bs_npages = -1;
		bsm_tab[i].bs_private_heap = -1;
	}
	return OK;
}

/*-------------------------------------------------------------------------
 * get_bsm - get a free entry from bsm_tab 
 *-------------------------------------------------------------------------
 */
SYSCALL get_bsm(int* avail)
{
	int  i;
	for(i=0;i<NUM_BACKING_STORE;i++)
	{
		if(bsm_tab[i].bs_status == BSM_UNMAPPED)
		{
			*avail = i;
			return OK;
		}
	}
	return SYSERR;
}


/*-------------------------------------------------------------------------
 * free_bsm - free an entry from bsm_tab 
 *-------------------------------------------------------------------------
 */
SYSCALL free_bsm(int i)
{
                bsm_tab[i].bs_status = BSM_UNMAPPED;
                bsm_tab[i].next=NULL;
                bsm_tab[i].bs_pid = -1;
                bsm_tab[i].bs_vpno = -1;
                bsm_tab[i].bs_npages = -1;
                bsm_tab[i].bs_private_heap = -1;

}

/*-------------------------------------------------------------------------
 * bsm_lookup - lookup bsm_tab and find the corresponding entry
 *-------------------------------------------------------------------------
 */
SYSCALL bsm_lookup(int pid, long vaddr, int* store, int* pageth)
{
}


/*-------------------------------------------------------------------------
 * bsm_map - add an mapping into bsm_tab 
 *-------------------------------------------------------------------------
 */
SYSCALL bsm_map(int pid, int vpno, int source, int npages)
{
}



/*-------------------------------------------------------------------------
 * bsm_unmap - delete an mapping from bsm_tab
 *-------------------------------------------------------------------------
 */
SYSCALL bsm_unmap(int pid, int vpno, int flag)
{
}
int bs_add_mapping(int virtpage, bsd_t source, int npages, int pid)
{ 
     int i;
    bs_map_t *temp,*next, *p;
    for (i=0;i<16;i++){
	next = &bsm_tab[i];
    }
    if(bsm_tab[source].bs_status == BSM_MAPPED)
    {
	temp = &bsm_tab[source];
	while(temp !=NULL)
	{
		if(temp->bs_pid == currpid)
		{
		        temp->bs_vpno = virtpage;
		        temp->bs_npages = npages;
			return OK;
		}
		temp=temp->next;
	}
	return SYSERR;
        
    }
    else
    {
          return SYSERR;
    }
 
    return OK;
}
