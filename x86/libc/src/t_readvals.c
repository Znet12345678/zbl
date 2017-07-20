#include <kernel.h>
void t_readvals(){
	__asm__("mov $8,%ah");
	__asm__("int $0x80");
}
