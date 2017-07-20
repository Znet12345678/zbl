#include <kernel.h>
void t_writevals(){
	__asm__("mov $9,%ah");
	__asm__("int $0x80");
}
