#include <lib.h>
int main(int argc,char *argv[]){
	t_readvals();
	for(int i = 1; i < argc; i++)
		kprintf("%s ",argv[i]);
}
