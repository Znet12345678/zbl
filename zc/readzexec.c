#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include "zexec.h"
int main(int argc,char *argv[]){
	if(argc != 2)
		return -1;
	struct __zexec_hdr *hdr = malloc(sizeof(*hdr));
	FILE *f = fopen(argv[1],"rb");
	if(!f)
		return -1;
	fread(hdr,sizeof(uint8_t),sizeof(*hdr),f);
	printf("Main Header\n[Signature] %s\n[arch] %s\n[System] %s\n[entry] 0x%.8x\n[Number of program headers] %d\n[Offset of first program header] 0x%.8x\n[Number of symbol tables] %d\nOffset of first symbol table] 0x%.8x\n",&hdr->sig,hdr->arch == 32 ? "i386 x86 CPU" : "unkown",hdr->sys == 0 ? "zBL/minOS" : "Some other UNIX system",hdr->entry,hdr->nphdr,hdr->phdr_offset,hdr->nsymtab,hdr->symtab_offset);
	printf("Program headers\n");
	fseek(f,0,SEEK_SET);
	uint8_t *buf = malloc(10240);
	int i = 0,c;
	while((c = getc(f)) != EOF){
		memcpy(buf + i,&c,1);
		i++;
	}
	struct __zexec_phdr *phdr = (struct __zexec_phdr *)(buf + hdr->phdr_offset);
	for(i = 0; i < hdr->nphdr;i++){
		printf("Program header %d\n",i);
		printf("[Allocated] %s\n",phdr[i].alloc ? "Yes" : "No");
		printf("[Type] %d\n",phdr[i].type);
		printf("[Location of load in virtual memory] 0x%.8x\n",phdr[i].vmem_addr);
		printf("[Offset of binary code] 0x%.8x\n",phdr[i].rawd_offset);
		printf("[Size of binary code] %d bytes\n",phdr[i].size);
	}
	printf("Symbol table\n");
	struct __zexec_symtab *symtab = (struct __zexec_symtab *)(buf + hdr->symtab_offset);
	for(int i = 0; i < hdr->nsymtab;i++){
		printf("Symbol table %d\n",i);
		printf("[Allocated] %s\n",symtab[i].alloc ? "Yes" : "No");
		printf("[Length of name] 0x%.2x\n",symtab[i].namelen);
		printf("[Internal or external symbol] %s\n",symtab[i].symLookup ? "External symbol(Needs to be looked up)" : "Internal symbol");
		printf("[Name of symbol] %s\n",symtab[i].symname);
		printf("[Location to load in virtual memory] 0x%.8x\n", symtab[i].vmem_addr);
		printf("[Location of binary code in the file] 0x%.8x\n",symtab[i].rawd_offset);
		printf("[size of symbol] 0x%.8x\n",symtab[i].size);
	}
	fclose(f);
	free(buf);
	free(hdr);
	return 0;
}
