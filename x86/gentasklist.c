#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fs/src/init.h"

int main(int argc,char *argv[]){
	if(argc < 3)
		return -1;
	FILE *f = fopen(argv[1],"wb");
	for(int i = 2; i < argc;i++){
		struct init_task *task = malloc(sizeof(*task));
		task->alloc = 1;
		task->pathlen = strlen(argv[i]);
		fwrite(task,sizeof(uint8_t),sizeof(*task),f);
		fwrite(argv[i],sizeof(uint8_t),strlen(argv[i]),f);
	}
	fclose(f);
	return 0;
}
