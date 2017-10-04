#include <kernel.h>
void t_putc(char c){
	t_readvals();
	__asm__("pushal");
	__asm__("mov $11,%ah");
	__asm__("mov %0,%%ebx" : :"m"(c));
	__asm__("int $0x80");
	__asm__("popal");
}
