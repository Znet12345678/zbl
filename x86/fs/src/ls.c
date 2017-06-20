#include <lib.h>
int main(int argc,char *argv[]){
	if(argc != 2)
		return -1;
	list(argv[1]);
	return 0;
}
