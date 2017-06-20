#include <lib.h>
int main(){
	mem_init();
	t_readvals();
	kprintf("Control given to init!\n");
	kprintf("TODO\n");
	while(1){
		kprintf("KSH$");
		char *cmd = malloc(1024);
		gets(cmd);
		kprintf("\n{%s}\n",cmd);
	}
}
