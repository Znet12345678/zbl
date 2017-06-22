#include <lib.h>
int main(int argc,char *argv[]){
	t_readvals();
	for(int i = 1; i < argc; i++){
		int size = fsize(argv[i]);
		if(!size)
			return 0;
		char *buf = malloc(size);
		read_file(argv[1],buf);
		kprintf("%s\n",buf);
	}
}
