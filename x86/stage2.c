#include <stdint.h>
#include "lib.h"
__asm__(".code32");
__asm__("jmp main");
void *memcpy(void *dstptr,const void *srcptr,unsigned long size){
        unsigned char *dst = (unsigned char *)dstptr;
        const unsigned char * src = (const unsigned char *)srcptr;
        for(int i = 0; i < size;i++)
                dst[i] = src[i];
        return dstptr;
}

unsigned long strlen(const char *str){
	int i = 0;
	while(str[i] != 0)
		i++;
	return i;
}
int strcmp(const char *str1,const char *str2){
	int i = 0;
	if(strlen(str1) != strlen(str2))
		return -1;
	while(i < strlen(str1)){
		if(str1[i] != str2[i])
			return ++i;
		i++;
	}
	return 0;
}
void print(const char *str){
	for(int i = 0; i < strlen(str);i++)
		t_putc(str[i]);
}
void panic(){
	print("panic()");
	while(1)
		__asm__("hlt");
}
char *strcpy(char *dest,const char *src){
	int i = strlen(dest);
	int j = 0;
	while(i < strlen(dest) + strlen(src)){
		dest[i] = src[j];
		i++;
		j++;
	}
	return dest;
}
void strcat(char *dest,const char *src){
	strcpy(&dest[strlen(dest) - 1],src);
}
int main(){
	t_init();
	int *drives = detect_drives();
	kprintf("Searching for kernel\n");
	for(int i = 0; i < 4;i++)
		if(drives[i] == 1){
			uint8_t *buf = malloc(513);
			int r = __ata_read_master(buf,0,i);
			if(!r)
				kprintf("I/O Error\n");
			if(buf[2] == 0x1f && buf[3] == 0xaf && buf[4] == 0x0f && buf[5] == 0x9f){
				*(int*)0x499 = i;
				kprintf("Found kernel on drive %d\n",i);
				//while(1);
				goto g;
			}
			kprintf("Kernel not found on drive %d,sig %d %d %d %d\n",i,buf[2],buf[3],buf[4],buf[5]);
		}
	print("Kernel not found!\n");
	panic();
	print("Scanning for installation media\n");
	print("HDA->");
	uint8_t *buf = malloc(513);
	int drive = 0;
	int err = ata_read_master(buf,0,drive);
	if(err < 0)
		print("Primary drive not attached!\n");
	if(buf[2] == 0x1f && buf[3] == 0xaf &&buf[4] ==  0x0f && buf[5] == 0x9f)
		goto g;
	free(buf);
	buf = malloc(513);
	print("\nHDB->");
	drive = 1;
	err = ata_read_master(buf,0,drive);
	if(err < 0)
		print("Secondary drive not attached!\n");
	if(buf[2] == 0x1f && buf[3] == 0xaf &&buf[4] ==  0x0f && buf[5] == 0x9f)
                goto g;
	print("\nNo Drives found!");
	scroll();
	panic();
	g:kprintf("Found\n");
	free(buf);
	kprintf("[SEARCH]Looking for kernel\n");
	int lba = 0;
	char sig[] = {0x1f,0x7f,0x2f,0x1f};
	buf = malloc(1024);
	//kprintf("%d\n",sig[2]);
	while(1){
		int err = ata_read_master(buf,lba,drive);
		if(err < 0)
			panic();
		if(buf[0] == sig[0] && buf[1] == sig[1] && buf[2] == sig[2] && buf[3] == sig[3])
			break;
		for(int i = 0; i < 512;i++)
			buf[i] = 0;
		lba++;
	}
	kprintf("[SEARCH] found @%d\n",lba);
	int slba = buf[4] << 24 | buf[5] << 16 | buf[6] << 8 | buf[7];
	kprintf("Starting lba:%d\n",slba);
	int soffset = buf[8] << 16 | buf[9];
	kprintf("Starting offet:%d\n",soffset);
	int elba = buf[10] << 24 | buf[11] << 16 | buf[12] << 8 | buf[13];
	kprintf("Ending lba:%d\n",elba);
	int eoffset = buf[14] << 8 | buf[15];
	kprintf("Ending offet:%d\n",eoffset);
	long b = (elba - slba) * 512 + (512 - soffset) + 512 - eoffset;
	*(int*)0x500 = drive;
	kprintf("Reading...\n");
	for(int i = slba;i <= elba;i++){
		uint8_t *dest = (uint8_t *)(0x01000000 + (i - slba) * 512);
		for(int j = 0; j < 512;j++)
			dest[j] = 0;
		ata_read_master(dest,i,0);
	}
	kprintf("Done reading...\n");
	uint8_t *mem = (uint8_t*)(0x01000000 + soffset);
	uint8_t *pntr= (uint8_t*)0x00100000;
	int (*main)();
	main = exec_elf(pntr,mem);
	t_writevals();
	main();
	//load_elf_from_memory(mem);
	while(1);
	/*uint8_t *buf = (uint16_t*)0x00100000;
	print("Determaning disk!\n");
	uint8_t *pntr = (uint8_t*) 0x0000500;
	uint16_t io;
	int drive = -1;
	if(*pntr == 0){
		print("Floppy not supported\n");
		panic();

	}else if(*pntr == 0x80){
		io = 0x1f0;
		drive = 0;
		print("Using primary hd\n");
	}else if(*pntr = 0x81){
		io = 0x170;
		drive = 1;
		print("Using secondary hd\n");
	}
	//print("Reading disk...\n");
	//if(read_elf_on_disk(buf,35,io) < 0)
	//	panic();
	//print("Finding ELF in memory\n");
	while(1){
		print("zbl>");
		char *buf = (char*)0x00000501;
		erase(buf,1024);
		gets(buf);
		if(strcmp(buf,"ls") == 0){
			int lba = 51;
			r:;int pos = 0;
			uint8_t *_buf = (uint8_t*)0x00100000;
			int err = ata_read_master(_buf,lba,drive);
			if(!err){
				kprintf("I/O Error\n");
				panic();
			}
			print("\n");
			b:while(_buf[pos] != 0x7f && pos < 512)
				pos++;
			if(pos == 512){
				lba++;
				goto r;
				print("ERROR:Couldn't find file list!\n");
				if(drive == 1)
					break;
				print("Trying hdb");
				drive = 1;
				goto r;
			}
			c:;int origpos = pos;
			if(_buf[pos + 1] != 'z' || _buf[pos + 2] != 'B' || _buf[pos + 3] != 'L')
				goto b;
			pos+=4;
			while(1){
				if(_buf[pos] != 1){
					break;
				}
				int size = _buf[pos+1];
				int namelen = _buf[pos + 2];
				for(int i = 0; i < namelen;i++)
					t_putc(_buf[i + pos + 3]);
				int _origpos = pos;
				int nxt_lba = _buf[namelen + pos + 13] << 24 | _buf[namelen + pos + 14] << 16 | _buf[namelen + pos + 15] << 8 | _buf[namelen + pos + 16];
				pos = _buf[namelen + pos + 17] << 8 | _buf[namelen + pos + 18] +origpos;
				if(_buf[namelen + _origpos + 19] == 0x6f)
					break;
				ata_read_master(_buf,nxt_lba + lba,drive);
				if((pos + 100) > 512){
					uint8_t *buf2 = (uint8_t*)0x00100200;
					ata_read_master(buf2,nxt_lba + lba + 1,drive);
				}
				t_putc('\n');
			}
			t_putc('\n');
		}else if(strcmp(buf,"txt") == 0){
			while(1){
				t_putc('\n');
				t_putc('>');
				char *_buf = (char*)0x00000501;
				for(int i = 0; i < 1024;i++)
					_buf[i] = 0;
				gets(_buf);
				if(strcmp(_buf,"exit") == 0)
					break;
				int val = cat(_buf);
				kprintf("Cat returned: %d\n",val);
			}
		}
		else{
			if(ldr(buf,0) < 0){
					kprintf("\nInvalid command:%s\n",buf);
			}
		}
	}
	pntr = (uint8_t*)0x00100000;
	int i = 0;
	a:while(1){
		if(*pntr == 0x7f)
			break;
		*pntr++;
	}
	*pntr++;
	if(*pntr != 'E')
		goto a;
	*pntr++;
	if(*pntr != 'L')
		goto a;
	*pntr++;
	if(*pntr != 'F')
		goto a;
	print("Parsing and loading ELF\n");
	*pntr--;
	*pntr--;
	*pntr--;
	load_elf_from_memory(pntr);
	print("Done!\n");
	while(1)
		;
	while(1){
		print("zbl>");
		char *buf = (char*)0x00000500;
		gets(buf);
		print("\n");
		print(buf);
		erase(buf,1024);
		print("\n");
	}*/
}
