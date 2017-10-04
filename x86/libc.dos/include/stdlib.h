#ifndef __STDLIB_H
#define __STDLIB_H
#include <sys/types.h>

void *malloc(size_t size);
void *calloc(size_t n,size_t size);
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
#ifndef NULL
#define NULL 0
#endif
void abort();
void exec(const char *str);
void *bsearch(const void *key, const void *base,
                     size_t nmemb, size_t size,
                     int (*compar)(const void *, const void *));
void *realloc(void *pntr,size_t s);
char **sep(const char *str,int c);
#endif
