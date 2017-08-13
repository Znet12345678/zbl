#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <module.h>
int main(int argc,char *argv[]){
	if(argc != 2)
		return -1;
	return call_module_function(argv[1]);	
}
