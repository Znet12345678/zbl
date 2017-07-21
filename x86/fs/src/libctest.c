#include <stdio.h>
#include <stdlib.h>
int main(){
	printf("klibc works!\n");
	char *str = malloc(1024);
	strcpy(str,"Malloc");
	strcat(str," Works!\n");
	printf("%s",str);
	return 1;
}
