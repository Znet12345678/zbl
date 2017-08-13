#ifndef __MODULE_H
#define __MODULE_H
#include <stdint.h>
struct __mod_table_ent{
	uint8_t alloc;
	uint8_t namelen;
	uint8_t *name;
	uint32_t module_len;
	uint8_t *mod;
};
extern struct __mod_table_ent *mtable;
#endif
