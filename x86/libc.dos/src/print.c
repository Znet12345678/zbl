#include <kernel.h>
void print(const char *str){
	t_readvals();
	__asm__("mov $6,%ah");
	__asm__("movl %0,%%ebx" : : "m"(str));
	__asm__("int $0x80");
	t_writevals();
}
