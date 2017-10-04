#ifndef __ZEXEC_H
#define __ZEXEC_H
#include <stdint.h>
#define TYPE_LOAD 0
#define TYPE_DYNAM 1
struct __zexec_hdr{
	uint32_t sig;
	uint8_t arch;
	uint8_t sys;
	uint32_t entry;
	uint16_t nphdr;
	uint32_t phdr_offset;
	uint16_t nsymtab;
	uint32_t symtab_offset;
	uint8_t padding[256];
};
struct __zexec_phdr{
	uint8_t alloc;
	uint8_t type;
	uint32_t size;
	uint32_t vmem_addr;
	uint32_t rawd_offset;
};
struct __zexec_symtab{
	uint8_t alloc;
	uint8_t namelen;
	uint8_t symLookup;
	uint8_t symname[80];
	uint32_t vmem_addr;
	uint32_t rawd_offset;
	uint32_t phdr;
	uint32_t size;
};
#endif
