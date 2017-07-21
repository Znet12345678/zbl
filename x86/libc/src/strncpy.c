#include <string.h>
char *strncpy(char *dest,const char *src,size_t n){
	int i = 0;
	while(i < n){
		dest[i] = src[i];
		i++;
	}
}
