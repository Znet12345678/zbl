#include <stdio.h>
#include <stdlib.h>
void abort(){
	printf("abort()");
	exec("/fs/init.elf");
}
