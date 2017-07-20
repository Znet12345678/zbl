/*
*zBL C Library
*7-18-17
*/
#include <string.h>
int strcmp(const char *str1,const char *str2){
	if(strlen(str1) != strlen(str2))
		return -1;
	int i = 0;
	while(i < strlen(str1)){
		if(str1[i] != str2[i])
			return ++i;
		i++;
	}
	return 0;
}
