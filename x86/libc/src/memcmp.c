#include <string.h>
int memcmp(const void *mem1,const void *mem2,size_t n){
	unsigned const char *u8mem1 = (unsigned const char *)mem1;
	unsigned const char *u8mem2 = (unsigned const char *)mem2;
	for(int i = 0; i < n;i++)
		if(u8mem1[i] != u8mem2[i])
			return ++i;
	return 0;
}
