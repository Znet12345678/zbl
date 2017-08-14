#include <fcntl.h>
int write(int fd,void *buf,int n){
	asm("pushal");
	asm("mov $16,%ah");
	asm("mov %0,%%ebx" : :"m"(fd));
 	asm("mov %0,%%ecx" : :"m"(buf));
	asm("mov %0,%%edx"::"m"(n));
	asm("int $0x80");
	int ret;
	asm("mov %%eax,%0" :"=m"(ret));
	asm("popal");
	return ret;
}
