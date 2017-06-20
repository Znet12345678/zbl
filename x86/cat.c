#include <stdint.h>
#include "lib.h"
int cat(const char *fname){
	uint8_t *buf = (uint8_t*)0x00100000;
	ata_read_master(buf,51,0);
	int pos = 0;
	a:;while(pos < 512 && buf[pos] != 0x7f)
		pos++;
	int origpos = pos;
	if(pos == 512){
		kprintf("NO File System found!\n");
		panic();
	}
	if(buf[pos + 1] != 'z' || buf[pos + 2] != 'B' || buf[pos + 3] != 'L')
		goto a;
	pos+=4;
	while(1){
		int alloc = buf[pos];
		if(alloc != 1){
			print("No such file or directory!\n");
			return -1;
		}
		int size = buf[pos + 1];
		int namelen = buf[pos + 2];
		char name[512];
		for(int i = 0; i < 512;i++)
			name[i] = 0;
		for(int i = 0; i < namelen;i++)
			name[i] = buf[pos + 3 + i];
		if(strcmp(name,fname) == 0){
			int size = buf[pos + namelen + 3] << 24 | buf[pos + namelen + 4] << 16 | buf[pos + namelen + 5] << 8 | buf[pos + namelen + 6];
                        int start_lba = buf[pos + namelen + 7] << 24 | buf[pos + namelen + 8] << 16 | buf[pos + namelen + 9] << 8 | buf[pos + namelen + 10];
                        int start_offset = buf[pos + namelen + 11] << 8 | buf[pos + namelen + 12] + origpos;
                        int end_lba = buf[pos + namelen + 13] << 24 | buf[pos + namelen + 14] << 16 | buf[pos + namelen + 15] << 8 | buf[pos + namelen + 16];
                        int end_offset = buf[pos + namelen + 17] << 8 | buf[pos + namelen + 18] + origpos;
			//kprintf("Reading %d[%d]->%d[%d]\n",start_lba,start_offset,end_lba,end_offset);
			int j = 0;
			for(int i = start_lba; i <= end_lba;i++,j++){
				uint8_t *dest = (uint8_t *)(0x00100000 + 512 * j);
				ata_read_master(dest,51 + i,0);
				if(i == start_lba)
					for(int j = 0; j < start_offset;j++)
						dest[j] = 0;
				else if(i == end_lba)
					for(int j = end_offset; j < 512;j++)
						dest[j] = 0;

			}
			int written = 0;
			int newlines = 0;
			uint8_t *_buf = (uint8_t *)(0x00100000 + start_offset);
			t_init();
			int xwritten = 0;
			//kprintf("Reading size of %d bytes\n",size);
			while(written < size){
				if(_buf[written] == '\n' || xwritten == 80){
					xwritten = 0;
					newlines++;
				}
				if(newlines == 25){
					char *null = (char*)0;
					gets(null);
					newlines = 0;
					t_init();
					continue;
				}
				t_putc(_buf[written]);
				if(_buf[written] != '\n')
					xwritten++;
				written++;
			}
			return 1;
		}
		int _origpos = pos;
		int nxt_lba = buf[pos + namelen + 13] << 24 | buf[pos + namelen + 14] << 16 | buf[pos + namelen + 15] << 8 | buf[pos + namelen + 16];
		pos = buf[pos + namelen + 17] << 8 | buf[pos + namelen + 18] + origpos;
                if(buf[namelen + _origpos + 19] == 0x6f){
			kprintf("No such file or directory\n");
                        return -1;
		}
		ata_read_master(buf,nxt_lba + 51,0);
		if((pos + 100) > 512){
                        uint8_t *_buf = (uint8_t*)0x00100200;
                        ata_read_master(_buf,nxt_lba + 52,0);
                }

	}
}
