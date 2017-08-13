#include <libelf.h>
int contains_symbol(void *mem,const char *name){
	asm("pushal");
	int ret;
	asm("xor %eax,%eax");
	asm("mov $15,%ah");
	asm("mov %0,%%ebx"::"m"(mem));
	asm("mov %0,%%ecx"::"m"(name));
	asm("int $0x80");
	asm("mov %%eax,%0":"=m"(ret));
	return ret;
}
