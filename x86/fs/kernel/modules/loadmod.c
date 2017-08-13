#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <module.h>
#include <kernel.h>
int init_mod(){
}
int load_mod(const char *filename){
	struct __mod_table_ent *mtable = 0x0A000000;
	__asm__("pushal"); 
	struct __mod_table_ent *pntr = 0x0A000000;
	char *fname = malloc(1024);
	strcpy(fname,"/fs/kernel/modules/");
	strcat(fname,filename);
	strcat(fname,".ko");
	while(pntr->alloc == 1){
		if(strcmp(pntr->name,fname) == 0){
			printf("Already loaded module %s\n",filename);
			return -1;
		}
		pntr+=sizeof(*pntr);
	}
	pntr->alloc = 1;
	pntr->namelen = strlen(filename) + strlen("/fs/kernel/modules/") + strlen(".ko");
	pntr->name = malloc(pntr->namelen);
	strcpy(pntr->name,"/fs/kernel/modules/");
	strcat(pntr->name,filename);
	strcat(pntr->name,".ko");
	pntr->module_len = fsize(pntr->name);
	pntr->mod = malloc(pntr->module_len);
	int fd = open(pntr->name,O_RDONLY);
	int ret = read(fd,pntr->mod,fsize(pntr->name));
	if(ret < 1){
		printf("Does not exist\n");
		pntr->alloc = 0;
		pntr->namelen = 0;
		pntr->module_len = 0;
		return -1;
	}
	__asm__("popal");
	return 1;
}
int main(int argc,char *argv[]){
	if(argc != 2)
		return 0;
	return load_mod(argv[1]);
}
