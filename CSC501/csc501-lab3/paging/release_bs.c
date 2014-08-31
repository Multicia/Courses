#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <paging.h>

SYSCALL release_bs(bsd_t bs_id) 
{
  /* release the backing store with ID bs_id */
	xmunmap(bsm_tab[bs_id].bs_vpno);
	free_bsm(bs_id);
	return OK;
}

