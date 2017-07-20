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
//	uint8_t *pntr = (uint8_t*)0x8048000;
	uint8_t *jmp = exec_elf(pntr,buf);
	int (*main)(int argc,char **argv) = (int*)jmp;
	kprintf("\n");
	t_writevals();
	free(buf);
	free(path);
	return main(argc,arr);
//	return exec_elf_args(pntr,buf,argc,arr);
}
int main(){
	mem_init();
	__asm__("mov $0,%ah\nint $0x80");
	__asm__("mov $1,%ah\nmov $9,%al\nint $0x80");
	char *pntr = malloc(1024);
	strcpy(pntr,"Test");
//	__asm__("movl %0,%%ebx" : : "m"(pntr));
//	__asm__("movl $2,%ah\nmovl %0,%%ebx\nmovl $1,%ecx\nmovl $2,%edx\n" : : "m"(pntr));
	__asm__("mov $2,%ah");
	__asm__("movl %0,%%ebx" : : "m"(pntr));
	__asm__("movl $1,%ecx");
	__asm__("movl $2,%edx");
	__asm__("int $0x80");
	int ret;
	__asm__("movl %%eax,%0" : "=m"(ret) :);
//	kprintf("%d\n",ret);
	t_readvals();
	list("/");
	kprintf("Control given to init!\n");
	kprintf("TODO\n");
	char *cmd = malloc(1024);
	while(1){
		for(int i = 0; i < 1024;i++)
			cmd[i] = 0;
		kprintf("KSH$");
		gets(cmd);
		if(!shell(cmd)){
			kprintf("Failed to run command %s",cmd);
		}
		kprintf("\n");
	}
}
