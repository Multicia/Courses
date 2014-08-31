#include <stdio.h>
extern char *etext,*edata,*end;

void printsegaddress()
{
	kprintf("\n\n Task 2 (printsegaddress)");
	kprintf("\n Current: etext[0x%08x] = 0x%08x, edata[0x%08x] = 0x%08x, ebss[0x%08x] = 0x%08x",&etext, *(&etext), &edata, *(&edata), &end, *(&end));
	kprintf("\n Preceding: etext[0x%08x] = 0x%08x, edata[0x%08x] = 0x%08x, ebss[0x%08x] = 0x%08x",((int *)&etext - 1), *((int *)&etext - 1),((int *)&edata - 1), *((int *)&edata - 1),((int *)&end-1), *((int *)&end-1));
	kprintf("\n After: etext[0x%08x] = 0x%08x, edata[0x%08x] = 0x%08x, ebss[0x%08x] = 0x%08x",((int *)&etext + 1), *((int *)&etext + 1),((int *)&edata + 1), *((int *)&edata + 1),((int *)&end + 1), *((int *)&end + 1));	
}
