#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <paging.h>

int get_bs(bsd_t bs_id, unsigned int npages) {

  /* requests a new mapping of npages with ID map_id */
    if((bs_id < 0 || bs_id > NUM_BACKING_STORE) || (npages < 1 || npages > 100))
		return SYSERR;
	

    //If a part of a private heap then return SYSERR
    if(bsm_tab[bs_id].bs_private_heap == 1)
    {
	kprintf("Trying to access private heap");
	return SYSERR; 
    }


    if(bsm_tab[bs_id].bs_status == BSM_UNMAPPED)
    {
        bsm_tab[bs_id].bs_status = BSM_MAPPED;
        bsm_tab[bs_id].bs_pid = currpid;
        bsm_tab[bs_id].bs_vpno = 0;
        bsm_tab[bs_id].bs_npages = npages;
	bsm_tab[bs_id].next = NULL;
	return npages;
        
    }
    else if(bsm_tab[bs_id].bs_status == BSM_MAPPED)
    {
	//Add mapping for more processes
	bs_map_t *temp = &bsm_tab[bs_id];
	struct pentry *ptr = &proctab[currpid];
	bs_map_t *map = &(ptr->map[bs_id]);
	if(temp->bs_pid != currpid && temp->next == NULL )
	{
		kprintf("ERROR");
		return SYSERR;
	}
	else 
	{
		int temp_return;
		while(temp->next !=NULL)
			temp=temp->next;
		temp->bs_status = BSM_MAPPED;
		temp->bs_pid = currpid;
		temp->bs_vpno = 0;
		temp_return = temp->bs_npages;
		temp->bs_npages = npages;
		temp->next = map;	
		return temp_return;
	}
    }
    else
        return SYSERR;
    

}


