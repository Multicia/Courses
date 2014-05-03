/* pfint.c - pfint */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <paging.h>


/*-------------------------------------------------------------------------
 * pfint - paging fault ISR
 *-------------------------------------------------------------------------
 */
SYSCALL pfint()
{

	//kprintf("PAGE INTERRUPT ");
	int count;
	pd_t *ptr;
	pt_t *ptr1;
	unsigned int frame_number,i;
	unsigned int fault_addr = read_cr2();// faulted address 
	//kprintf(" Faulted Address is %x\n",fault_addr);
	unsigned int page_directory_address = read_cr3() & 0xFFFFF000;
	unsigned int page_table_number,page_number,page_offset;	


	//kprintf("Page Directory Address %d\n",FNUM(page_directory_address));
	page_table_number = fault_addr >> 22;
	page_number = (fault_addr >> 12)&0x3FF;
	page_offset = fault_addr & 0xFFF;
	ptr = (pd_t *)page_directory_address + (page_table_number);//pointer type casting takes care of shift by 2
	//kprintf("Page Table Number = %d\n",page_table_number);
	//kprintf("The value of the entry = %x\n",*ptr);
	//kprintf("ptr -> pd_pres = %d\n",ptr -> pd_pres);
	if(ptr -> pd_pres == 0)
	{
		//kprintf("PAGE DIRECTORY ENTRY NOT PRESENT\n");
		frame_number = get_frame(FR_TBL, fault_addr >> 12);
		unsigned int *temp = 0x400000 + NBPG*frame_number;
		bzero(temp,NBPG);
		//kprintf("Frame Number %d\n",frame_number);
	        if(frame_number == SYSERR)
		        return SYSERR;
		ptr->pd_write =1;
                ptr->pd_pres = 1;
		ptr->pd_base = (0x00400000 + frame_number*4096) >> 12;
      		write_cr3(page_directory_address);
	        return OK;
	}
	else 
	{
		//Looking for the qth page
		ptr1 = (pt_t *) (ptr->pd_base << 12) + page_number;
               //kprintf("faulted addr: %x xth page table: %x, content: %x\n",fault_addr,ptr1,*ptr1);
	        if(ptr1 -> pt_pres == 0)
	        {
			//kprintf("PAGE TABLE ENTRY NOT PRESENT \n");
	 	        for(i=0; i<=MAX_ID; i++)
		        {
			//kprintf("%d %d %d %d %d\n",bsm_tab[i].bs_status,bsm_tab[i].bs_pid,bsm_tab[i].bs_vpno,bsm_tab[i].bs_npages,currpid);
               		if(bsm_tab[i].bs_status == BSM_MAPPED && bsm_tab[i].bs_pid == currpid && (fault_addr >> 12) >= bsm_tab[i].bs_vpno && ((fault_addr >> 12) < bsm_tab[i].bs_vpno + bsm_tab[i].bs_npages ))
		                {
			//		kprintf("SEARCHING FOR BS\n");
	       		    	      frame_number = get_frame(1,FR_PAGE,fault_addr>>12);
		                      if(frame_number == SYSERR)
				      {
					  kprintf("ERROR!!\n");					
		                          return SYSERR;
				      }
		                      ptr1->pt_base = (0x00400000 + frame_number*4096) >> 12;
				      ptr->pd_write =1;
		                      ptr1 -> pt_pres = 1;
		                    //  read_bs((char*)(ptr1->pt_base<<12),i,(fault_addr>>12)-bsm_tab[i].bs_vpno);
		                      write_cr3(page_directory_address);
		                      return OK;
		                }
  	        	}
			kill(currpid);
		        return SYSERR;
      		}
      		else
			  //It should not come here
		          return OK;
  	}
}
