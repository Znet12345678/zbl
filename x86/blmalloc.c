#include "lib.h"
void *_blmalloc(unsigned long size){
	int *mem = (int*)0x01000000;
	int *ret;
	int allocated = 0;
	while(allocated < size){
		if(*mem == 0){
			*mem = 1;
			*mem++;
			if(*mem == 0 || *mem < size){
				int tmpsize = *mem;
				*mem = size;
				*mem++;
				while(allocated < size){
					*mem = 0;
					*ret = *mem;
					*ret++;
					*mem++;
					allocated++;
				}
				if(tmpsize != 0){
					*mem = 0;
					*mem++;
					*mem = 0;
				}
				return (void*)ret;
			}else{
				int tmpsize = *mem;
				while(allocated < tmpsize){
					*mem = 0;
					*ret = *mem;
					*ret++;
					*mem++;
					allocated++;
				}
			}
		}else{
			*mem++;
			int size = *mem;
			*mem++;
			for(int i = 0; i < size;i++)
				*mem++;
		}
	}
	return (void*)ret;
}
void blfree(void *pntr){
	char *cpntr = (char*)pntr;
	*cpntr-=2;
	*cpntr=0;
}
