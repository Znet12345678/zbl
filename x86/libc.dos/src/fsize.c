#include <kernel.h>
int fsize(const char *filename){
	int ret;
	__asm__("pushal");
	__asm__("xor %eax,%eax");
	__asm__("mov $13,%ah");
	__asm__("mov %0,%%ebx" : :"m"(filename));
	__asm__("int $0x80");
	__asm__("mov %%eax,%0" : "=m"(ret));
	__asm__("popal");
	return ret;
}
