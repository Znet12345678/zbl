/*
*zBL C Library
*7-18-17
*/
#include <string.h>
#include <sys/types.h>
char *strcpy(char *dest,const char *src){
	int i = 0;
	int j = strlen(dest);
	while(i < strlen(src)){
		dest[j] = src[i];
		i++;
		j++;
	}
	return dest;
}
