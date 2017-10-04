#ifndef __ENV_H
#define __ENV_H
#include <stdint.h>
struct env{
	uint8_t alloc;
	uint8_t *name;
	uint8_t *val;	
};
void setenv(const char *name,const char *val);
char *getenv(const char *name);
#endif
