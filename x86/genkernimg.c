#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "img.h"
int main(int argc,char *argv[]){
	if(argc != 4)
		return -1;
	FILE *stage1 = fopen(argv[1],"rb");
	FILE *in = fopen(argv[2],"rb");
	FILE *out = fopen(argv[3],"wb");
	fseek(in,0,SEEK_END);
	int size = ftell(in);
	fseek(in,0,SEEK_SET);
	if(!in || !out || !stage1){
		perror("Couldn't open file");
		return -1;
	}
	int c;
	uint32_t pos = 0;
	while((c = getc(stage1)) != EOF){
		putc(c,out);
		pos++;
	}
	while((pos % 512) != 0){
		putc(0,out);
		pos++;
	}
	struct kernel_img *img = malloc(sizeof(*img) * sizeof(img));
	memcpy(img->sig,sig,4);
	img->start_lba = pos/512;
	img->start_offset = 16;
	img->end_lba = pos/512 + size/512;
	img->end_offset = (16 + size) % 512;
	int i = 0;
	while(i < 4){
		putc(img->sig[i],out);
		i++;
	}
	putc(img->start_lba >> 24,out);
	putc(img->start_lba >> 16,out);
	putc(img->start_lba >> 8,out);
	putc(img->start_lba,out);
	putc(img->start_offset >> 8,out);
	putc(img->start_offset,out);
	for(int i = 24; i >= 0;i-=8)
		putc(img->end_lba >> i,out);
	putc(img->end_offset >> 8,out);
	putc(img->end_offset,out);
	while((c = getc(in)) != EOF)
		putc(c,out);
	free(img);
	fclose(in);
	fclose(out);
	fclose(stage1);
	return 0;
}
