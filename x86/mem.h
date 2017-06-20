#ifndef __MEM_H
#define __MEM_H
#include <stdint.h>
struct mem_part{
	uint8_t alloc;
	unsigned long size;
	uint8_t complete;
	struct mem_part *nxt;
	uint8_t *begin;
	uint8_t *end;
	uint32_t n;
};
void *malloc(unsigned long size);
#define NULL 0
#endif
