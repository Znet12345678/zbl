#include <lib.h>
int main(int argc,char *argv[]){
	t_readvals();
	kprintf("LS:");
	if(argc != 2){
		kprintf("Invalid number of args %d\n",argc);
		return 0;
	}
	list(argv[1]);
	return 1;
}
