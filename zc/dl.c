#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include "zexec.h"
#include "libdynm.h"
int count_dynmsymb(const char *path){
	int ret = 0;
	FILE *f = fopen(path,"rb");
	if(!f)
		return 0;
	int size;
	fseek(f,0,SEEK_END);
	size = ftell(f);
	fseek(f,0,SEEK_SET);
	uint8_t *buf = malloc(size);
	fread(buf,sizeof (uint8_t),size,f);
	struct __zexec_hdr *hdr = (struct __zexec_hdr *)buf;
	struct __zexec_symtab *stab = (struct __zexec_symtab *)(hdr + hdr->symtab_offset);
	for(int i = 0; i < hdr->nsymtab;i++){
		if(stab[i].symLookup){
			ret++;
		}
	}
	fclose(f);
	return ret;
}
struct symbol * resolv_sym(const char *symn,const char **arr,int n,int s){
	struct symbol *ret = malloc(sizeof(*ret));
	for(int i = s; i < n;i++){
		FILE *f = fopen(arr[i],"rb");
		if(!f){
			perror("Failed to open file");
			return 0;
		}
		fseek(f,0,SEEK_END);
		int size = ftell(f);
		fseek(f,0,SEEK_SET);
		uint8_t *buf = malloc(size);
		fread(buf,sizeof (uint8_t),size,f);
		struct __zexec_hdr *hdr = (struct __zexec_hdr *)buf;
		struct __zexec_symtab *stab = (struct __zexec_symtab *)(buf + hdr->symtab_offset);
		for(int i = 0; i < hdr->nsymtab;i++){
			if(strcmp(stab[i].symname,symn) == 0 && !stab[i].symLookup){
				ret->namelen = strlen(symn);
				strcpy(ret->name,symn);
				ret->symoffset = i;
				ret->size = stab[i].size;
				ret->pntr = malloc(ret->size);
				memcpy(ret->pntr,buf + stab[i].rawd_offset,ret->size);
				return ret;
			}
		}	
		fclose(f);
	}
	return 0;
}
int main(int argc,char *argv[]){
	if(argc < 3)
		return -1;
	struct __zexec_hdr *hdr = malloc(sizeof *hdr);
	hdr->sig = 0x7f| 'e' << 8 | 'x' << 16 | 'e' << 24;
	hdr->arch = 32;
	hdr->sys = 0;
	hdr->entry = 0x10000000;
	hdr->nphdr = 1;
	hdr->phdr_offset = sizeof(*hdr);
	for(int i = 2; i < argc;i++){
		hdr->nsymtab+=count_dynmsymb(argv[i]);
	}
	hdr->symtab_offset = hdr->phdr_offset + sizeof(struct __zexec_phdr);
	struct symbol *start = resolv_sym("_start",argv,argc,2);
	char *startn = malloc(1024);
	strcpy(startn,"_start");
	while(!start){
		printf("Couldn't resolve %s! Enter a symbol you would like to use as _start:",startn);
		free(startn);
		startn = malloc(1024);
		scanf("%s",startn);
		start = resolv_sym(startn,argv,argc,2);
	}
	printf("Resolved entry point: %s\n",startn);
	return 0;	
}
