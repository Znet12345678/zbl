#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <kernel.h>
#include <fcntl.h>

#include <dev.h>
int main(int argc,char *argv[]){
	if(strcmp(argv[1],"ls") == 0){
		__kern_dev_t dev = 0x00A00000;
		while(dev->alloc){
			printf("%s ",dev->ident);
			dev+=sizeof(*dev);
		}
		return 1;
	}else if (argc < 3)
		return 0;
	__kern_dev_t dev = 0x00A00000;
	while(dev->alloc){
		if(strcmp(dev->ident,argv[1]) == 0)
			break;
		dev+=sizeof(*dev);
	}
	if(!dev->alloc)
		return 0;
	char *path = malloc(1024);
	strcpy(path,"/dev/");
	strcat(path,argv[1]);
	return 1;
}