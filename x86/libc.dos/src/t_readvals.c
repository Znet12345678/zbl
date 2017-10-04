#include <kernel.h>
void t_readvals(){
	__asm__("pushal");
	__asm__("mov $8,%ah");
	__asm__("int $0x80");
	__asm__("popal");
}
