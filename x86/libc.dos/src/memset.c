#include <string.h>
#include <sys/types.h>
void *memset(void *pntr,int val,size_t n){
	unsigned char *cpntr = (unsigned char*)pntr;
	int i = 0;
	while(i < n){
		cpntr[i] = val;
		i++;
	}
	return pntr;
}
