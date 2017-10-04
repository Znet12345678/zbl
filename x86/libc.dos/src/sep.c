#include <stdlib.h>
char **sep(const char *str,int c){
        int i = 0,k = 0,l = 0;
        char **ret = (char**)malloc(102400);
        while(str[i] != 0){
                while(str[i] == c)
                        i++;
                ret[l] = malloc(1024);
                while(str[i] != c && str[i] != 0){
                        ret[l][k] = str[i];
                        k++;
                        i++;
                }
                l++;
                k = 0;
        }
        ret[l] = (char*)0;
        return ret;
}

