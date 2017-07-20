#include "idt.h"
#include "lib.h"
#include "mem.h"
void load_idt(void *base,int size){
	struct{
		uint16_t size;
		void *pntr;
	}__attribute__((packed)) IDT = {size,base};
	__asm__("lidt %0" : : "m"(IDT));
}
void helloworld(){
	kprintf("Interupts work!\n");
}
void write_int(int a){
	kprintf("%d\n",a);
}
