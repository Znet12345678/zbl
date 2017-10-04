/*
*zBL C Library
*7-18-17
*/
#include <string.h>
#include <sys/types.h>
int strncmp(const char *str1,const char *str2,size_t n){
	int i = 0;
	while(i < n){
		if(str1[i] != str2[i])
			return ++i;
		i++;
	}
	return 0;
}
