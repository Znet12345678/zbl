/*
*String.h
*zBL C Library
*/
#ifndef __STRING_H/*__STRING_H*/
#define __STRING_H

#include <sys/types.h>
#if defined(__cplusplus)
extern "C" {
#endif
/*
*strlen(*str)
*Returns an Unsigned long value for the length of the string
*/
size_t strlen(const char *str);
/*
*strcpy(dest,src)
*Copies strlen(src) bytes of data from src to dest
*Similar to memcpy but n is strlen(src)
*/
char *strcpy(char *dest,const char *src);
/*
*strcat(dest,src)
*Copies strlen(src) bytes of data from src to the end of dest.(dest[strlen(dest)])
*/
char *strcat(char *dest,const char *src);
/*
*memcpy(mem1,mem2,n)
*Copies n bytes of data from mem2 to mem1.
*/
void *memcpy(void *mem1,const void *mem2,size_t n);
/*
*memmove(mem1,mem2,n);
*similar to memcpy
*/
void *memmove(void *mem1,const void *mem2,size_t n);
/*
*strcmp(str1,str2)
*returns 0 on equivilence of both strings else it returns the index + 1 of the first off character
*/
int strcmp(const char *str1,const char *str2);
/*
*strncmp(str1,str2,n)
*returns 0 if the first n bytes of both strings are equal else it returns the index + 1 of the first off character
*/
int strncmp(const char *str1,const char *str2,size_t n);
/*
*memset(pntr,val,n);
*sets n bytes of pntr to val
*/
void *memset(void *pntr,int value,size_t n);
/*
*memcmp (mem1,mem2,n)
*Compares two memory locations to eachother
*/
int memcmp(const void *mem1,const void *mem2,size_t n);
/*
*strncpy(dest,src,n)
*/
char *strncpy(char *dest,const char *src,size_t n);
char *strncat(char *dest,const char *src,size_t n);
char *strchr(const char *str,int c);
char *strrchr(const char *str,int c);
#if defined(__cplusplus)
}
#endif
#endif/*__STRING_H*/
