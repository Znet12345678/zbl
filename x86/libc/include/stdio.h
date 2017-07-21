/*
*STDIO.H
*zBL C Library
*TODO
*/
#ifndef __STDIO_H
#define __STDIO_H
typedef struct __IO_FILE{
	//TODO
}FILE;
#define SEEK_SET 0
#define SEEK_END 2
#define SEEK_CUR 1
#define EOF -1
extern FILE *stderr;
extern FILE *stdin;
extern FILE *stdout;
#ifndef NULL
#define NULL 0
#endif
#endif
