#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main(int argc,char *argv[]){
	if(argc != 3)
		return -1;
	FILE *in = fopen(argv[1],"rb");
	if(!in)
		return -1;
	fseek(in,0,SEEK_END);
	int pos = ftell(in);
	fclose(in);
	FILE *out = fopen(argv[2],"wb");
	if(!out)
		return -1;
	while((pos % 512) != 0){
		putc(0,out);
		pos++;
	}
	fclose(out);
	return 0;
}
