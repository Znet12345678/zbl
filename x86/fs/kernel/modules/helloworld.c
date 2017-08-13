#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <kernel.h>
int mod_init(){
	t_readvals();
	return 1;
}
int func1(){
	printf("Function 1 executed\n");
	return 1;
}
int func2(){
	printf("Function 2 executed\n");
	return 1;
}
int mod_fin(){
	t_writevals();
	return 1;
}
