#ifndef __ELF_H
#define __ELF_H
#include <stdint.h>
typedef uint16_t Elf32_Half;
typedef uint32_t Elf32_Off;
typedef uint32_t Elf32_Addr;
typedef uint32_t Elf32_Word;
typedef int32_t Elf32_Sword;
struct ELF_header{
	uint8_t	e_ident[4];
	Elf32_Half e_type;
	Elf32_Half e_machine;
	Elf32_Word e_version;
	Elf32_Addr e_entry;
	Elf32_Off e_phoff;
	Elf32_Off e_shoff;
	Elf32_Word e_flags;
	Elf32_Half e_ehsize;
	Elf32_Half e_phentsize;
	Elf32_Half e_phnum;
	Elf32_Half e_shentsize;
	Elf32_Half e_shnum;
	Elf32_Half e_shstrndx;
};
#define ELFMAG0 0x7f
#define ELFMAG1 'E'
#define ELFMAG2 'L'
#define ELFMAG3 'F'
#define ELFDATA2LSB (1)
#define ELFCLASS32  (1)
enum Elf_Type{
	ET_NONE = 0,
	ET_REL = 1,
	ET_EXEC = 2,
};
#define EM_386     (3)
#define EV_CURRENT (1)
struct Elf32_Shdr{
	Elf32_Word sh_name;
	Elf32_Word sh_type;
	Elf32_Word sh_flags;
	Elf32_Addr sh_addr;
	Elf32_Off  sh_offset;
	Elf32_Word sh_size;
	Elf32_Word sh_link;
	Elf32_Word sh_info;
	Elf32_Word sh_addralign;
	Elf32_Word sh_entsize;
};
enum SH_Types{
	SHT_NULL 	= 0,
	SHT_PROGBITS 	= 1,
	SHT_SYMTAB	= 2,
	SHT_STRTAB	= 3,
	SHT_RELA	= 4,
	SHT_NOBITS	= 8,
	SHT_REL		= 9,
};
enum SH_Attr{
	SHF_WRITE	= 0x01,
	SHF_ALLOC	= 0x02
};
struct Elf32_PHeader{
	uint32_t ph_type;
	uint32_t ph_offset;
	uint32_t ph_vaddr;
	uint32_t ph_paddr;
	uint32_t ph_filesz;
	uint32_t ph_memsz;
	uint32_t p_flags;
	uint32_t p_align;
};
#define PT_NULL 0x00000000
#define PT_LOAD 0x00000001
#define PT_DYNAMIC 0x00000002
#define PT_INTERP 0x00000003
#define PT_NOTE 0x00000004
#define PT_SHLIB 0x00000005
#define PT_PHDR 0x00000006
#define PT_LOOS 0x60000000
#define PT_HIOS 0x6FFFFFFF
#define PT_LOPROC 0x70000000
#define PT_HIPROC 0x7FFFFFFF
#endif
