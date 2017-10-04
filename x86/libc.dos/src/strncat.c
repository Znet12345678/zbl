#include <string.h>
char *strncat(char *dest,const char *src,size_t n){
	return strncpy(&dest[strlen(dest)],src,n);
}
