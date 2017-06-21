#include <lib.h>
int shell(char *cmd){
	char **arr = sep(cmd,' ');
	int last = 0;
	while(arr[last] != 0)
		last++;
	int argc = last;
	char *path = malloc(1024);
	strcpy(path,"/fs/exec/");
	strcat(path,arr[0]);
	strcat(path,".elf");
	int size = fsize(path);
	if(!size)
		return 0;
	uint8_t *buf = malloc(size);
	int ret = read_file(path,buf);
	if(!ret)
		return 0;
	uint8_t *pntr = (uint8_t*)0x00100000;
	uint8_t *jmp = exec_elf(pntr,buf);
	int (*main)(int argc,char **argv) = (int*)jmp;
	kprintf("\n");
	t_writevals();
	return main(argc,arr);
//	return exec_elf_args(pntr,buf,argc,arr);
}
int main(){
	mem_init();
	t_readvals();
	list("/");
	kprintf("Control given to init!\n");
	kprintf("TODO\n");
	while(1){
		kprintf("KSH$");
		char *cmd = malloc(1024);
		gets(cmd);
		if(!shell(cmd)){
			kprintf("Failed to run command %s",cmd);
		}
		kprintf("\n");
	}
}
