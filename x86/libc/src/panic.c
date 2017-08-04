#include <stdio.h>
#include <kernel.h>
int panic(){
	t_readvals();
	printf("panic();");
	while(1);
}
