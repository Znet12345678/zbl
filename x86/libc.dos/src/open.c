#include <fcntl.h>
int open(const char *str,int mode,int o){
	__asm__("pushal");
	__asm__("xor %eax,%eax");
	__asm__("mov $2,%ah");
	__asm__("mov %0,%%ebx": :"m"(str));
	__asm__("mov %0,%%ecx" : :"m"(mode));
	__asm__("xor %edx,%edx");
	__asm__("mov %0,%%edx" : :"m"(o));
	__asm__("int $0x80");
	int ret;
	__asm__("mov %%eax,%0" :"=m"(ret));
	__asm__("popal");
	return ret;
}
