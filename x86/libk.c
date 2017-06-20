#include "lib.h"
unsigned long strlen(const char *str){
	int i = 0;
	while(str[i] != 0)
		i++;
	return i;
}
void *memcpy(void *dstptr,const void *srcptr,unsigned long size){
        unsigned char *dst = (unsigned char *)dstptr;
        const unsigned char * src = (const unsigned char *)srcptr;
        for(int i = 0; i < size;i++)
                dst[i] = src[i];
        return dstptr;
}
/*
void *memmove(void *dstpntr,const void *srcptr,unsigned long size){
        unsigned char * dst = (unsigned char *)dstpntr;
        const unsigned char *src = (const unsigned char *)srcptr;
        if(dst < src)
                for(unsigned long i = 0; i < size;i++)
                        dst[i] = src[i];
        else
                for(unsigned long i = size; i != 0;i--)
                        dst[i - 1] = src[i - 1];
        return dstpntr;
}*/
void panic(){
	kprintf("panic();\n");
	while(1);
}
char *strcpy(char *dest,const char *src){
        int i = strlen(dest);
        int j = 0;
        while(j < strlen(src)){
                dest[i] = src[j];
                i++;
                j++;
        }
        dest[strlen(dest)] = 0;
        return dest;
}
char *strcat(char *dest,const char *src){
        return strcpy(&dest[strlen(dest)],src);
}
void print(const char *str){
	kprintf("%s",str);
}
