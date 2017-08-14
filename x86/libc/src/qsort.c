#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <string.h>
void qsort(void *base,size_t num,size_t size,int (*compar)(const void *v1,const void *v2)){
	uint8_t *mem = (uint8_t*)base;
	for(int i = 0; i < num;i++){
		for(int j = 0; j < num;j++){
			void *v1 = base + i * size;
			void *v2 = base + j * size;
			if(compar(v1,v2) > 0){
				int *save = malloc(size);
				memcpy(save,base + i * size,size);
				*(int*)(base + i) =  *(int*)v2;
				*(int*)(base + j) = *save;
			}
		}
	}
}
