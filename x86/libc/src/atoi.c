#include <stdlib.h>
#include <string.h>
int __pow(int n,int pow){
	int ret = 1;
	int i =0;
	while(i < pow){
		ret*=n;
		i++;
	}
	return ret;
}
int atoi(const char *str){
	int ret = 0;
	for(int i = 0; i< strlen(str);i++)
		ret+=(str[i] - '0') * __pow(10,strlen(str)-i-1);
	return ret;
}
