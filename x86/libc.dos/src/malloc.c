#include <sys/types.h>
#include <stdlib.h>
void *malloc(size_t size){
	void *ret;
	__asm__("pushal");
	__asm__("mov $10,%ah");
	__asm__("mov %0,%%ebx": : "m"(size));
	__asm__("int $0x80");
	__asm__("movl %%eax,%0": "=m"(ret));
	__asm__("popal");
	return ret;
}
void free(void *pntr){
	__asm__("pushal");
	__asm__("mov $12,%ah");
	__asm__("movl %0,%%ebx" : :"m"(pntr));
	__asm__("int $0x80");
	__asm__("popal");
}
