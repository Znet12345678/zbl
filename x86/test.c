#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
int countc(char *str,int c){
	int ret = 0, i = 0;
	while(i < strlen(str)){
		if(str[i] == c)
			ret++;
		i++;
	}
	return ret;
}
char **sep(char *str,int c){
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
        return ret;
}
char **_sep(const char *str,int c){
        int i = 0,k = 0,l = 0;
        char **ret = (char**)malloc(1024 * (countc(str,c) + 1));
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
        return ret;
}
int main(){
	const char *path = "/usr/bin/startx";
	char **_sep = sep(path,'/');
	printf("%s\n",_sep[0]);
}
