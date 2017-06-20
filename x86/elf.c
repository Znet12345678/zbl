#include "lib.h"
#include "elf.h"
int iself(uint8_t *pntr){
	if(pntr[0] == 0x7f && pntr[1] == 'E' && pntr[2] == 'L' && pntr[3] == 'F')
		return 1;
	return 0;
}
int load_elf_from_memory(uint8_t *pntr){
	struct ELF_header hdr;
	for(int i = 0 ;i < 4;i++)
		hdr.e_ident[i] = pntr[i];
	hdr.e_type = pntr[4];
	hdr.e_machine = pntr[5];
	hdr.e_version = pntr[6];
	hdr.e_entry = pntr[27] << 24 | pntr[26] << 16 | pntr[25] << 8 | pntr[24];
	hdr.e_phoff = pntr[31] << 24 | pntr[30] << 16 | pntr[29] << 8 | pntr[28];
	hdr.e_shoff = pntr[35] << 24 | pntr[34] << 16 | pntr[33] << 8 | pntr[32];
	hdr.e_flags = pntr[39] << 24 | pntr[38] << 16 | pntr[37] << 8 | pntr[36];
	hdr.e_ehsize = pntr[41] << 9 | pntr[40];
	hdr.e_phentsize = pntr[43] << 8 | pntr[42];
	hdr.e_phnum = pntr[45] << 8 | pntr[44];
	hdr.e_shentsize = pntr[47] << 8 | pntr[46];
	hdr.e_shnum = pntr[49] << 8 | pntr[48];
	hdr.e_shstrndx = pntr[51] << 8 | pntr[50];
	if(!iself(pntr)){
		print("Not a valid elf exec!\n");
		return -1;
	}
	if(hdr.e_type != ELFCLASS32){
		print("Invalid architecture!\n");
		return -1;
	}
	if(hdr.e_machine != ELFDATA2LSB){
		print("Unsupported byte order\n");
		return -1;
	}
	int type = pntr[17] << 8 | pntr[16];
	if(type != 1){
		print("Not relocatable!\n");
		kprintf("Val:%d\n",type);
	}
	kprintf("ELF looks valid!\n");
        struct Elf32_PHeader *pheader;
        //pheader->ph_type = pntr[hdr.e_phoff + 3] << 24 | pntr[hdr.e_phoff + 2] << 16 | pntr[hdr.e_phoff + 1] | pntr[hdr.e_phoff];
        //pheader->ph_offset = pntr[hdr.e_phoff + 7] << 24 | pntr[hdr.e_phoff + 6] << 16 | pntr[hdr.e_phoff + 5] << 8 | pntr[hdr.e_phoff + 4];
	//pheader->ph_vaddr = pntr[hdr.e_phoff + 11] << 24 | pntr[hdr.e_phoff + 10] << 16 | pntr[hdr.e_phoff + 9] << 8 | pntr[hdr.e_phoff + 8];
	//pheader->ph_paddr = pntr[hdr.e_phoff + 15] << 24 | pntr[hdr.e_phoff + 14] << 16 | pntr[hdr.e_phoff + 13]  << 8 | pntr[hdr.e_phoff + 12];
	//pheader->ph_filesz = pntr[hdr.e_phoff + 19] << 24 | pntr[hdr.e_phoff + 18] << 16 | pntr[hdr.e_phoff + 17] << 8 | pntr[hdr.e_phoff + 16];
	pheader = (struct Elf32_PHeader *)(pntr + hdr.e_phoff);
	unsigned char *start;
	Elf32_Addr addr;
	Elf32_Addr estart;
	unsigned char *exec = (unsigned char *)0x00100000;//0x10000000;
	int offset = 0;
	for(int i = 0; i < hdr.e_phnum;i++){
		if(pheader[i].ph_type != PT_LOAD)
			continue;
		if(pheader[i].ph_filesz > pheader[i].ph_memsz)
			return -1;
		if(!pheader[i].ph_filesz)
			continue;
		start = (unsigned char *)(pntr + pheader[i].ph_offset);
		if(!estart)
			estart = pheader[i].ph_paddr;
		addr = (Elf32_Addr)exec + (pheader[i].ph_paddr - estart);
		memmove((unsigned char *)addr,(unsigned char *)start,pheader[i].ph_filesz);
		offset+=(pheader[i].ph_memsz + (pheader[i].ph_paddr - estart));
	}
	//kprintf("Dump:e_type:[%d] e_machine:[%d] e_version:[%d] e_entry: [%d] e_phoff: [%d] e_shoff: [%d] e_flags: [%d] e_ehsize: [%d] e_phentsize: [%d] e_phnum: [%d] e_shentsize: [%d] e_shnum: [%d] e_shstrndx: [%d] ph_type [%d] ph_offset [%d] ph_vaddr [%d] ph_paddr [%d] ph_filesz[%d]\n",hdr.e_type,hdr.e_machine,hdr.e_version,hdr.e_entry,hdr.e_phoff,hdr.e_shoff,hdr.e_flags,hdr.e_ehsize,hdr.e_phentsize,hdr.e_phnum,hdr.e_shentsize,hdr.e_shnum,hdr.e_shstrndx,pheader->ph_type,pheader->ph_offset,pheader->ph_vaddr,pheader->ph_paddr,pheader->ph_filesz);
	uint8_t *mem = (uint8_t*)0x00100000;
	print("Jumping\n");
	//goto *mem;
	int *_pntr = (int*)(hdr.e_entry - estart + (Elf32_Addr)exec);
	t_writevals();
	goto *_pntr;
	while(1)
		;
}
static inline struct Elf32_Shdr *elf_sheader(struct ELF_header *hdr){
	return (struct Elf32_Shdr *)((int)hdr + hdr->e_shoff);
}
static inline struct Elf32_Shdr *elf_section(struct ELF_header *hdr,int indx){
	return &elf_sheader(hdr)[indx];
}
