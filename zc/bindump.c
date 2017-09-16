#include <stdio.h>
#include <stdint.h>
int main(int argc,char *argv[]){
	if(argc != 2)
		return -1;	
	FILE *f = fopen(argv[1],"rb");
	if(!f)
		return -1;
	char c;
	while((c = getc(f)) != EOF){
		printf("0b");
		for(int i = 7; i >= 0;i--)
			printf("%d",(c & (1 << i)) >> i);
		printf(" ");
	}
	printf("\n");
	fclose(f);
	return 0;
}
