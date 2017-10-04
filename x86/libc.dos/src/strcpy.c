/*
*zBL C Library
*7-18-17
*/
#include <string.h>
#include <sys/types.h>
char *strcpy(char *dest,const char *src){
	int i = 0;
	while(i < strlen(src)){
		dest[i] = src[i];
		i++;
	}
	return dest;
}
