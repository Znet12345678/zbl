#include <fcntl.h>
int read(int fd,void *buf,int n){
	__asm__("pushal");
	__asm__("xor %eax,%eax");
	__asm__("mov $7,%ah");
	__asm__("mov %0,%%ebx" : :"m"(fd));
	__asm__("mov %0,%%ecx" : :"m"(buf));
	__asm__("mov %0,%%edx" : :"m"(n));
	__asm__("int $0x80");
	int ret;
	__asm__("mov %%eax,%0" : "=m"(ret));
	__asm__("popal");
	return ret;
}
