#include "fat.h"
#include "lib.h"
struct __fat_BS *parse_bs(){
	uint8_t *buf = malloc(512);
	ata_read_master(buf,0,0);
	struct __fat_BS *fbs = (struct __fat_BS*)buf;
	return fbs;
}
