#include "libelf.h"
#include <stdint.h>
#include "zfs.h"
#include "fat.h"
#include "genfs.h"
#include "idt.h"
#include "simpfs.h"
void print(const char *str){
	kprintf("%s",str);
}
#ifdef STANDALONE
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
#endif
void *memcpy(void *dstptr,const void *srcptr,unsigned long size){
	unsigned char *dst = (unsigned char *)dstptr;
	const unsigned char * src = (const unsigned char *)srcptr;
	for(int i = 0; i < size;i++)
		dst[i] = src[i];
	return dstptr;
}
void* memset(void* bufptr, int value, unsigned long size) {
	unsigned char* buf = (unsigned char*) bufptr;
	for (unsigned long i = 0; i < size; i++)
		buf[i] = (unsigned char) value;
	return bufptr;
}
void panic(){
	kprintf("panic()");
	while(1)
		__asm__("hlt");
}
unsigned long strlen(const char *str){
	int ret = 0;
	while(str[ret] != 0)
		ret++;
	return ret;
}
char *strcpy(char *dest,const char *src){
	int i = strlen(dest);
	int j = 0;
	while(j < strlen(src)){
		dest[i] = src[j];
		i++;
		j++;
	}
	dest[strlen(dest)] = 0;
	return dest;
}
char *strcat(char *dest,const char *src){
	return strcpy(&dest[strlen(dest)],src);
}
int main(){
	mem_init();
	t_readvals();
	int l = __prim_getlba() + 1;
        kprintf("[KERNEL]Starting LBA:%d\n",l);
        *(int*)0x501 = l;
	char *memtest = malloc(1024);
        strcpy(memtest,"Malloc doesn't work\n");
        free(memtest);
        char *mem = malloc(1024);
        strcpy(mem,"Malloc works!\n");
        kprintf("%s",mem);
	kprintf("[KERNEL INIT]\n");
	if(!isSimpfs()){
		kprintf("Making simple filesystem\n");
		mkfs();
	}
	mkdir("/test");
	mkdir("/test2");
	mkdir("/test/oi");
	kprintf("[TMP_DONE]\n");
	while(1);
	kprintf("Generating devices...\n");
	init_devs();	
	//t_readvals();
	debug("KERNEL","Kernel successfully loaded!");
	char *init = malloc(1024);
	strcpy(init,"/fs/init.elf");
	char *isr = malloc(1024);
	strcpy(isr,"/fs/kernel.ko");
	uint8_t *pnt = malloc(fsize(isr));
	int fd = open(isr,O_RDONLY);
	read(fd,pnt,fsize(isr));
	char *panic = malloc(1024);
	strcpy(panic,"/fs/panic.ko");
	uint8_t *_pnt = malloc(fsize(panic));
	int pfd = open(panic,O_RDONLY);
	int n = read(pfd,_pnt,fsize(panic));
	uint8_t *dest= (uint8_t*)0x00F00000;
	int *bin = exec_elf(dest,pnt);
	int *exe_poffset = exec_elf(dest,_pnt);
	struct Elf32_Hdr *hdr = (struct Elf32_Hdr *)pnt;
	struct elf32_Phdr *phdr = (struct elf32_Phdr *)pnt + hdr->phdr_pos;
	int start = phdr[0].p_paddr;
	int offset = 0x00F00000;
	int poffset = 0x05000000;
	//int offset = *;
	struct idt_descr *_idt = malloc(sizeof(*_idt));
	_idt->offset_1 = offset;
	_idt->offset_2 = offset >> 16;
	_idt->zero = 0;
	_idt->type_attr = 0b10001110;
	_idt->selector = 0x08;
	struct idt_descr *idt = malloc(sizeof(*idt)*256);
	struct idt_descr *panicidt = malloc(sizeof(*panic));
	panicidt->offset_1 = poffset;
	panicidt->offset_2 = poffset >> 16;
	panicidt->zero = 0;
	panicidt->type_attr = 0b10001111;
	panicidt->selector = 0x08;
	struct idt_descr *null = malloc(sizeof(*null));
	null->type_attr = 0;
	null->offset_1 = 0;
	null->offset_2 = 0;
	null->zero = 0;
	null->selector = 0;
	for(int i = 0; i < 0x20;i++)
		idt[i] = *panicidt;
	for(int i = 0x20; i < 0x80;i++)
		idt[i] = *null;
	for(int i = 0x81;i < 256;i++)
		idt[i] = *null;
	idt[0x80] = *_idt;
	load_idt(idt,sizeof(*idt) * 256 - 1);
//	idtp.limit = sizeof(idt[0])*256-1;
//	idtp.base = (unsigned int)idt;
//	idt_load();
	//while(1);
	__exec(init);
	//opendir("/safas");
	//write_file("/file",str,strlen(str));
	//list("/");
	//list("/fs");
	//list("/fs/users");
//	list("/test/test");
	//read_file("/fs/src/ls.c",buf);
	//kprintf("%s\n",buf);
	__exec("/fs/init.elf");
	debug("KERNEL","Done");
	while(1);
	if(is_zfs() < 0){
		kprintf("No file system detected\n");
		mkfs();
	}
//	struct dirent *ent = parse_dirent("/");
//	kprintf("[alloc] %d [namelen] %d [name] %s [first_fent_lba] %d [first_fent_offset] %d [nxt_dirent_lba] %d [nxt_dirent_offset] %d [curr_ent_lba] %d [curr_ent_offset] %d\n",ent->alloc,ent->namelen,ent->name,ent->first_fent_lba,ent->first_fent_offset,ent->nxt_dirent_lba,ent->nxt_dirent_offset,ent->curr_ent_lba,ent->curr_ent_offset);
	DIR *d = opendir("/");
	mkdir("/test");
//	opendir("/nonexistant");
//	mkdir("/test2");
	//list("/");
	debug("KERNEL","Done");
	while(1){
		gets(0);
		kprintf("\n");
	}
	/*char *malloctest1 = malloc(1024);
	strcpy(malloctest1,"Malloc works!\n");
	char *malloctest2 = malloc(1024);
	strcpy(malloctest2,"Malloc doesnt work!\n");
	kprintf("%s",malloctest1);
	kprintf("Loading...\n");
	free(malloctest1);
	free(malloctest2);
//	if(!has_sysinf()){
//		kprintf("Generating system information...\n");
//		gen_sysinf();
//	}
        struct fs_info *fsinfo = malloc(512);
        fsinfo->alloc = 1;
        uint32_t lba = end_of_prefs();
        fsinfo->start_lba = lba;
        fsinfo->mount_path_len = 1;
        fsinfo->mount_path = malloc(8);
        fsinfo->mount_path[0] = '/';
	mount(fsinfo);
	isfs();
	while(1)
		;
	if(!isfs()){
		kprintf("Making filesystem!\n");
		__mkfs("/");
	}
	mkdir("/test");
	kprintf("Done\n");*/
}
