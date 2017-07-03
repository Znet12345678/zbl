#ifndef __IDT_H
#define __IDT_H
#include <stdint.h>
struct idt_descr{
	uint16_t offset_1;
	uint16_t selector;
	uint8_t zero;
	uint8_t type_attr;
	uint16_t offset_2;
}__attribute__((packed));
struct idt_pntr{
	uint8_t limit;
	uint32_t base;
}__attribute__((packed));
void load_idt(void *base,int size);
///struct idt_descr idt[256];
//struct idt_pntr idtp;
extern void idt_load();
#endif
