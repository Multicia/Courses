#include<stdio.h>

void byteorder()
{
	int x = 1;
	if(*(char *)&x)
		kprintf("\n\nLittle Endian");
	else
		kprintf("\n\nBig Endian");
}
