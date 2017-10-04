#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include "zexec.h"
int resolv_symbol(const char *path,const char *symname){
	FILE *f = fopen(path,"rb");
	if(!f)
		return -1;
	fseek(f,0,SEEK_END);
	int size =ftell(f);
	fseek(f,0,SEEK_SET);
	uint8_t *pntr = malloc(size);
	int c,i = 0;
	while((c = getc(f)) != EOF){
		memcpy(pntr + i,&c,1);
		i++;
	}
	struct __zexec_hdr *hdr = (struct __zexec_hdr *)pntr;
	struct __zexec_symtab *symtab =(struct __zexec_symtab *)(pntr + hdr->symtab_offset);
	for(int i = 0; i < hdr->nsymtab;i++){
		if(strcmp(symtab[i].symname,symname) == 0 && !symtab[i].symLookup)
			return symtab[i].rawd_offset;
	}
	return -1;
}
