#ifndef __DEV_H
#define __DEV_H
#include <stdint.h>
typedef struct dev{
	uint8_t alloc;
	uint8_t *ident;
	uint8_t type;
	uint8_t slave;
	uint16_t io;
}*__kern_dev_t;
void init_devs();
#endif
