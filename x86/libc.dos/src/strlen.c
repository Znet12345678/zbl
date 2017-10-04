/*
*zBL C Library
*7-18-17
*/
#include <string.h>
#include <sys/types.h>
size_t strlen(const char *str){
	int i = 0;
	while(str[i] != 0)
		i++;
	return i;
}
