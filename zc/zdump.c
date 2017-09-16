#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include "zexec.h"
int main(int argc,char *argv[]){
	if(argc != 3)
		return -1;
	if(strcmp(argv[1],"dump_exec") == 0){
		FILE *f = fopen(argv[2],"rb");
		if(!f)
			return -1;
		fseek(f,0,SEEK_END);
		int size = ftell(f);
		fseek(f,0,SEEK_SET);
		uint8_t *buf = malloc(size);
		int c,i = 0;
		while((c = getc(f)) != EOF){
			memcpy(buf + i,&c,1);
			i++;
		}
		struct __zexec_hdr *hdr = (struct __zexec_hdr *)buf;
		struct __zexec_phdr *phdr = (struct __zexec_phdr *)(buf + hdr->phdr_offset);
		for(int i = 0; i < hdr->nphdr;i++){
			printf("phdr[%d]\n",i);
			uint8_t *pntr = buf + phdr[i].rawd_offset;
			for(int j = 0; j < phdr[i].size;j++){
				printf("%.2x ",pntr[j]);
			}
			printf("\n");
		}
		free(buf);
		fclose(f);	
	}
}
