#ifndef __LIBELF_H
#define __LIBELF_H
#include <stdint.h>
struct Elf32_Hdr{
	uint8_t e_ident[16];
	uint16_t type;
	uint16_t machine;
	uint32_t version;
//	uint8_t abi;
//	uint8_t padding[8];
	//uint16_t instruction_set;
	//uint32_t version2;
	uint32_t entry;
	uint32_t phdr_pos;
	uint32_t shdr_pos;
	uint32_t  flags;
	uint16_t header_size;
	uint16_t size_of_ent_phdr;
	uint16_t num_ent_phdr;
	uint16_t size_of_ent_shdr;
	uint16_t num_of_ent_shdr;
	uint16_t index_shdr_tab;
};
#define INSTRUCT_X86 3
struct elf32_Phdr{
	uint32_t type;
	uint32_t p_offset;
	uint32_t p_vaddr;
	uint32_t p_paddr;
	uint32_t p_filesz;
	uint32_t p_memsz;
	uint32_t flags;
	uint32_t alignemnt;
};
struct Elf32_Shdr{
	uint32_t sh_name;
	uint32_t sh_type;
	uint32_t sh_flags;
	uint32_t sh_addr;
	uint32_t sh_offset;
	uint32_t sh_size;
	uint32_t sh_link;
	uint32_t sh_info;
	uint32_t sh_addralign;
	uint32_t sh_entsize;
};
#define FLAG_EXEC 1
#define FLAG_WRITE 2
#define FLAG_READ 4
struct Elf32_SymTab{
	uint32_t st_name;
	uint32_t st_value;
	uint32_t st_size;
	uint8_t st_info;
	uint8_t st_other;
	uint16_t st_shndx;
};
#endif
