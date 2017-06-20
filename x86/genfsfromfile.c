#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gfff.h"

int main(int argc,char *argv[]){
	if(argc < 3)
		return -1;
	FILE *out = fopen(argv[1],"wb");
	putc(0x7f,out);
	putc('z',out);
	putc('B',out);
	putc('L',out);
	if(!out){
		perror("Failed to open file");
		return -1;
	}
	for(int i = 0; i < (argc - 2);i++){
		FILE *in = fopen(argv[i + 2],"rb");
		if(!in){
			perror("Failed to open input file");
			return -1;
		}
		fseek(in,0,SEEK_END);
		int size = ftell(in);
		fseek(in,0,SEEK_SET);
		int origpos = ftell(out);
		putc(1,out);
		int headersize = 20 + strlen(argv[i + 2]);
		putc(headersize,out);
		putc(strlen(argv[i + 2]),out);
		for(int j = 0; j < strlen(argv[i + 2]);j++)
			putc(argv[i+2][j],out);
		putc(size >> 24,out);
		putc(size >> 16,out);
		putc(size >> 8,out);
		putc(size,out);
		putc(((origpos + headersize)/512) >> 24,out);
		putc(((origpos + headersize)/512) >> 16,out);
		putc(((origpos + headersize)/512) >> 8,out);
		putc(((origpos + headersize)/512),out);
		putc(((origpos + headersize)%512) >> 8,out);
		putc(((origpos + headersize)%512),out);
		putc(((origpos + headersize + size)/512) >> 24,out);
		putc(((origpos + headersize + size)/512)  >> 16,out);
		putc(((origpos + headersize + size)/512)  >> 8,out);
		putc(((origpos + headersize + size)/512),out);
		putc(((origpos + headersize + size)%512) >> 8,out);
		putc(((origpos + headersize + size)%512),out);
//		printf("[%d] [%d]\n",(origpos + headersize + size)/512,(origpos + headersize + size)%512);
		if((i + 1) == (argc - 2))
			putc(0x6f,out);
		else
			putc(0,out);
		int c;
		while((c = getc(in)) != EOF){
			putc(c,out);
		}
		fclose(in);
	}
	fclose(out);
	return 0;
}
