#include "lib.h"
#include <stdint.h>
int ldr(const char *str,int drive){
	uint8_t *buf = (uint8_t*)0x00100000;
	int lba = 51;
	int err = ata_read_master(buf,lba,drive);
	int pos = 0;
	if(err < 0){
		kprintf("I/O Error!\n");
		return -1;
	}
	a:;while(buf[pos] != 0x7f && pos < 512)
		pos++;
	int _origpos = pos;
	if(pos == 512){
		lba++;
		goto a;
		kprintf("FS not found\n");
		return -1;
	}
	if(buf[pos + 1] != 'z' || buf[pos + 2] != 'B' || buf[pos + 3] != 'L')
		goto a;
	pos+=4;
	while(1){
		int alloc = buf[pos];
		if(alloc != 1){
			kprintf("\nNo such file or directory!");
			return -1;
		}
		int size = buf[pos + 1];
		int namelen = buf[pos + 2];
		char name[512];
		for(int i = 0; i < 512;i++)
			name[i] = 0;
		for(int i = 0; i < namelen;i++)
			name[i] = buf[pos + i + 3];
		if(strcmp(name,str) == 0){
			int size = buf[pos + namelen + 3] << 24 | buf[pos + namelen + 4] << 16 | buf[pos + namelen + 5] << 8 | buf[pos + namelen + 6];
			int start_lba = buf[pos + namelen + 7] << 24 | buf[pos + namelen + 8] << 16 | buf[pos + namelen + 9] << 8 | buf[pos + namelen + 10];
			int start_offset = buf[pos + namelen + 11] << 8 | buf[pos + namelen + 12] + _origpos;
			int end_lba = buf[pos + namelen + 13] << 24 | buf[pos + namelen + 14] << 16 | buf[pos + namelen + 15] << 8 | buf[pos + namelen + 16];
			int end_offset = buf[pos + namelen + 17] << 8 | buf[pos + namelen + 18] + _origpos;
			kprintf("\nReading %d[%d]->%d[%d]\n",start_lba,start_offset,end_lba,end_offset);
			int j = 0;
			for(int i = start_lba; i <= end_lba;i++,j++){
				//kprintf("Reading %d\n",i);
				uint8_t *dest = (uint8_t*)(0x01000000 + 512 * j);
				for(int j = 0; j < 512;j++)
					dest[j] = 0;
				ata_read_master(dest,i + lba,drive);
				if(i == end_lba)
					for(int i = end_offset; i < 512;i++)
						dest[i] = 0;
				if(i == start_lba)
					for(int i =0; i < start_offset;i++)
						dest[i] = 0;
			}
			uint8_t *dest = (uint8_t*)(0x01000000);
	//		ata_read_master_no(dest,51,start_offset,(end_lba - start_lba),end_offset);
			//ata_read_master_nb(dest,51,(end_lba - start_lba));
			for(int i = end_offset;i < 512;i++)
				dest[i + end_lba *512] = 0;
			uint8_t *end = (uint8_t*)(0x01000000 + 512 * end_lba);
			uint8_t *pntr = (uint8_t*)(0x01000000 + start_offset);
			kprintf("Attempting to load ELF don't worry if there are errors in this part\n");
			//int ldr_res = load_elf_from_memory(pntr);
			uint8_t *pnt = (uint8_t*)0x00100000;
			exec_elf(pnt,pntr);
			kprintf("\nJumping to location\n");
			__asm__("mov $0x105000,%esp");
			void (*main)(void) = pntr;
			main();
			goto *pntr;
			return 1;
		}
		int origpos = pos;
		int nxt_lba = buf[pos + namelen + 13] << 24 | buf[pos + namelen + 14] << 16 | buf[pos + namelen + 15] << 8 | buf[pos + namelen + 16];
		pos = buf[pos + namelen + 17] << 8 | buf[pos + namelen + 18] + _origpos;
		if(buf[namelen + origpos + 19] == 0x6f)
			return -1;
		ata_read_master(buf,nxt_lba + lba,drive);
		if((pos + 100) > 512){
			uint8_t *_buf = (uint8_t*)0x00100200;
			ata_read_master(_buf,nxt_lba + 52,drive);
		}
	}
}
