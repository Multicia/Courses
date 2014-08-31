/* user.c - main */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <stdio.h>
volatile int a_cnt = 0;
volatile int b_cnt = 0;
volatile int c_cnt = 0;
void halt();
proc_a(c)
	char c; {
	int i;
	kprintf("Start... %c\n", c);
	b_cnt = 0;
	c_cnt = 0;
	sleep(1);
	ps();
	sleep(5);
}
/*------------------------------------------------------------------------
 *  main  --  user main program
 *------------------------------------------------------------------------
 */
int main()
{
	int prA,i=0,j=0;
	kprintf("\n\nHello World, Xinu lives\n\n");
	prA = create(proc_a, 2000, 30, "proc A", 1, 'A');
	resume(prA);
	sleep(10);
	while(i<10000)
	{
		j=0;
		while(j<10000)
			j++;
		i++;
	}
	ps();
	return 0;
}
