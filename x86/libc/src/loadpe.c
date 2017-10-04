#include <stdlib.h>
int *loadPE(uint8_t *pntr){
	asm("pushal");
	asm("mov $17,%ah");
	asm("mov %0,%%ebx" : :"m"(pntr));
	asm("int $0x80");
	int *ret;
	asm("mov %%eax,%0" :"=m"(ret));
	asm("popal");
	return ret;
}
