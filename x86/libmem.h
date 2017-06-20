#ifndef __LIBMEM_H
#define __LIBMEM_H
#include <stdint.h>
struct mem_ent{
	uint8_t alloc;
	uint32_t size;
	struct mem_ent *nxt;
};
#endif
