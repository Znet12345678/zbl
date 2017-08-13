#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <module.h>
#include <libelf.h>
struct Elf32_Shdr *get_section_header(struct Elf32_Hdr *hdr){
	return (struct Elf32_Shdr *)hdr + hdr->shdr_pos;
}
struct Elf32_Shdr *get_section(struct Elf32_Hdr *hdr,int indx){
	return &get_section_header(hdr)[indx];
}
int call_module_function(const char *name){
	struct __mod_table_ent *mtable = (struct __mod_table_ent *)0x0A000000;
	int found = 0;
	while(mtable->alloc == 1){
		found = contains_symbol(mtable->mod,name);
		if(found){
			int (*pntr)() = found;
			pntr();
			return 1;
		}
		mtable+=sizeof(*mtable);
	}
	if(!found){
		printf("Module function not found!\n");
	}
	return found;
}
