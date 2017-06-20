#include "lib.h"
#include "mem.h"
int __prim_getlba(){
	int drive = *(int*)0x500;
	int i = 0;
	uint8_t *buf = malloc(1024);
	char sig[] = {0x1f,0x7f,0x2f,0x1f};
	while(1){
		int err = ata_read_master(buf,i,drive);
		if(err < 0)
			panic();
		if(buf[0] == sig[0] && buf[1] == sig[1] && buf[2] == sig[2] && buf[3] == sig[3])
                        break;
		for(int j = 0; j < 512;j++)
			buf[j] = 0;
		i++;
	}
	int ret = buf[10] << 24 | buf[11] << 16 | buf[12] << 8 | buf[13];
	return ret;
}
