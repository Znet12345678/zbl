#ifndef __IMG_H
#define __IMG_H
#include <stdint.h>
const uint8_t sig[4] = {0x1f,0x7f,0x2f,0x1f};
struct kernel_img{
	uint8_t sig[4];
	uint32_t start_lba;
	uint16_t start_offset;
	uint32_t end_lba;
	uint16_t end_offset;
};
#endif
