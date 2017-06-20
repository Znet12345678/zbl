#include "str.h"
void strncpy(char *dest,const char *src,int n){
        for(int i = 0; i < n;i++)
                dest[strlen(dest) + i] = src[i];
}

