#include <stdint.h>
#include "lib.h"
void _error(){
	int code;
	kprintf("\n_error()\n");
	while(1);
}
int dummy(char *path,int flags,int mode){
	kprintf("%s %d %d\n",path,flags,mode);
	return 50;
}
int __ide_wait_for_read(uint16_t io){
        int i = 0;
        while(i < 4){
                inb(io + 0x0C);
                i++;
        }
	retry:;
               	uint8_t status = inportb(io + 0x07);
                if(status & 0x80) goto retry;
        retry2: status = inb(io + 0x07);
                if(status & 0x01){
                        kprintf("I/O Error\n");
                        panic();
                }
                if(!(status & 0x08)) goto retry2;
                return 1;
}

void read_disk(uint8_t *buf,uint32_t lba){
	uint16_t io = 0x1f0;
	uint8_t cmd = 0x0e;
        outb(io + 0x06,(cmd | (uint8_t)((lba >> 24 & 0x0F))));
        outb(io + 1,0x00);
        outb(io + 0x02,1);
        outb(io + 0x03,(uint8_t)lba);
        outb(io + 0x04,(uint8_t)((lba) >> 8));
        outb(io + 0x05,(uint8_t)((lba) >> 16));
        outb(io + 0x07,0x20);
	ide_wait_for_read(io);
	for(int i = 0; i < 512;i++){
		uint16_t dat = inw(io);
		*(uint16_t*)(buf + i * 2) = dat;
	}
}
void *memmove(void *dstpntr,const void *srcptr,unsigned long size){
	unsigned char * dst = (unsigned char *)dstpntr;
	const unsigned char *src = (const unsigned char *)srcptr;
	if(dst < src)
		for(unsigned long i = 0; i < size;i++)
			dst[i] = src[i];
	else
		for(unsigned long i = size; i != 0;i--)
			dst[i - 1] = src[i - 1];
	return dstpntr;
}
