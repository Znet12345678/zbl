#include <stdio.h>
#include <kernel.h>
int panic(){
	__asm__("int $0x1f");
}
