#ifndef __STDLIB_H
#define __STDLIB_H
#include <sys/types.h>
void *malloc(size_t size);
void free(void *pntr);
struct mem_part{
        uint8_t alloc;
        unsigned long size;
        uint8_t complete;
        struct mem_part *nxt;
        uint8_t *begin;
        uint8_t *end;
        uint32_t n;
};

#endif
