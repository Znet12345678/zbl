#include <fcntl.h>
int close(int fd){
	asm("pushal");
	asm("xor %eax,%eax");
	asm("mov $3,%ah");
	asm("mov %0,%%ebx"::"m"(fd));
	asm("int $0x80");
	int ret;
	asm("mov %%eax,%0":"=m"(ret));
	asm("popal");
	return ret;
}
