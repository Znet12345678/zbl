#ifndef __LIBDYNM_H
#define __LIBDYNM_H
struct symbol{
	uint8_t namelen;
	uint8_t name[0xff];
	uint32_t symoffset;
	uint32_t size;
	uint8_t *pntr;
};
#endif
