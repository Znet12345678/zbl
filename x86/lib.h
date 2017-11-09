#ifndef __LIB_H
#define __LIB_H
#include <stdint.h>
#include "simpfs.h"
int *detect_drives();
void hello_world();
int vga_putc(int c,int x,int y);
int load_elf_from_memory(uint8_t *pntr);
void delay(uint16_t io);
struct __fdat *__parse_fdat(int lba,int offset);
void t_writevals();
int elf_get_size(void *pntr);
int *exec_elf(void *unused,void *src);
void debug(const char *func,const char *msg);
int ata_write_master(uint8_t *buf,uint32_t _lba);
int _ata_write_master(uint8_t *pntr,uint32_t lba);
int strncmp(const char *s1,const char *s2,int n);
int __prim_getlba();
void free(void *pntr);
void error(const char *err);
char *strcpy(char *dest,const char *src);
unsigned long strlen(const char *str);
void gets(char *str);
void silent_gets(char *str);
int ata_read_master(uint8_t *buf,uint32_t _lba,uint16_t drive);
int _ata_read_master(uint8_t *pntr,uint32_t lba,int drive);
int strcmp(const char *str1,const char *str2);
void *memcpy(void *dstptr,const void *srcptr,unsigned long size);
void *memmove(void *dstpntr,const void *srcptr,unsigned long size);
void t_init();
void kprintf(const char *format,...);
void panic();
void blfree(void *pntr);
void read_disk(uint8_t *buf,uint32_t lba);
void *blmalloc(unsigned long size);
void t_putc(char c);
int memcmp(const void *pntr1, const void *pntr2,int n);
#ifndef STAGE2
char *strcat(char *dest,const char *src);
#else
void strcat(char *dest,const char *src);
#endif
inline static void outb(uint16_t port,uint8_t val);
inline static uint16_t inb(uint32_t s);
static inline uint16_t inw(uint32_t s);
static inline uint16_t inw(uint32_t s);
void print(const char *buf);
char **sep(const char *str,int c);
static inline void outb(uint16_t port,uint8_t val){
     __asm__ volatile ("outb %w0, %w1" : : "a"(val), "Nd"(port));
}
static inline uint16_t inb(uint32_t port){
        uint16_t ret;
        __asm__ volatile("inw %%dx, %%ax": "=a"(ret):  "d"(port));
        return ret;
}
static inline uint16_t inportb(uint32_t port){
        return inb(port);
}

static inline void outw(uint16_t port,uint8_t val){
     __asm__ volatile ("outw %w0, %w1" : : "a"(val), "Nd"(port));
}
static inline void outsw(unsigned short int port,const void *val,unsigned long int n){
        unsigned long cnt = 1;
        __asm__ volatile ("rep outsw" :  "+S"(val),"+c"(n) : "d"(port));
}
static inline uint16_t inw(uint32_t s){
        uint16_t ret;
        __asm__ volatile("inw %%dx, %%ax": "=a"(ret):  "d"(s));
        return ret;
}

#endif
