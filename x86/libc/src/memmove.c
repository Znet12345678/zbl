#include <string.h>
void *memmove(void *dstpntr,const void *srcptr,size_t size){
        unsigned char * dst = (unsigned char *)dstpntr;
        const unsigned char *src = (const unsigned char *)srcptr;
        if(dst < src)
                for(unsigned long i = 0; i < size;i++)
                        dst[i] = src[i];
        else
                for(unsigned long i = size; i != 0;i--)
                        dst[i - 1] = src[i - 1];
        return dstpntr;
}

