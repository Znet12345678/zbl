/*
*STDIO.H
*zBL C Library
*TODO
*/
#ifndef __STDIO_H
#define __STDIO_H
#include <sys/types.h>
typedef struct __IO_FILE{
	//TODO
	int fd;
}FILE;
#define SEEK_SET 0
#define SEEK_END 2
#define SEEK_CUR 1
#define EOF -1
extern FILE *stderr;
extern FILE *stdin;
extern FILE *stdout;
int fgets(void *buf,size_t n,FILE *f);
int putc(int c,FILE *f);
int fputc(int c,FILE *f);
int fclose(FILE *f);
int feof(FILE *f);
FILE *fopen(const char *path,const char *mode);
size_t fwrite(const void *pntr,size_t size,size_t nb,FILE *f);
size_t fread(void *pntr,size_t size,size_t nb,FILE *f);
void printf(const char *format,...);
int sprintf(char *buf,const char *format,...);
#ifndef NULL
#define NULL 0
#endif
#endif
