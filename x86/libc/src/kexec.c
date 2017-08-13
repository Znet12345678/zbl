#include <kernel.h>
int *kexec(void *unused,void *pntr){
	__asm__("pushal");
	__asm__("xor %eax,%eax");
	__asm__("mov $14,%ah");
	__asm__("mov %0,%%ebx" : :"m"(unused));
	__asm__("mov %0,%%ecx" : :"m"(pntr));
	__asm__("int $0x80");
	int *ret;
	__asm__("mov %%eax,%0" : "=m"(ret));
	__asm__("popal");
	return ret; 
}
