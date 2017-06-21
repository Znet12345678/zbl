#include <lib.h>
int main(){
	t_readvals();
	kprintf("Welcome to zbl!\nRight now there is not too much going on but if you'd like you could put some more executables in fs/exec make sure they are linked with -lk and .text is\n");
	kprintf("Loaded to 0x00100000\n");
	return 1;
}
