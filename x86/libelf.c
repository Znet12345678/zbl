#include "lib.h"
#include "mem.h"
#include "libelf.h"
int *exec_elf(void *dest,void *src){
	uint8_t *u8src = (uint8_t*)src;
	uint8_t *u8dest = (uint8_t*)dest;
	struct Elf32_Hdr *hdr = (struct Elf32_Hdr*)src;
	int start = 0;
	kprintf("Parsing elf...\n");
	if(hdr->e_ident[0] != 0x7f || hdr->e_ident[1] != 'E' || hdr->e_ident[2] != 'L' || hdr->e_ident[3] != 'F'){
		print("Invalid sig\n");
		kprintf("{[%d],[%c],[%c],[%c]}\n",hdr->e_ident[0],hdr->e_ident[1],hdr->e_ident[2],hdr->e_ident[3]);
		return -1;
	}
	if(hdr->e_ident[4] != 1){
		print("Invalid elf class\n");
		return -1;
	}
	if(hdr->e_ident[5] != 1){
		print("Not little endian\n");
		return -1;
	}
	struct elf32_Phdr *phdr = (struct elf32_Phdr *)(src + hdr->phdr_pos);
	for(int i = 0; i < hdr->num_ent_phdr;i++){
		if(!start)
			start = phdr[i].p_paddr;
		//if(phdr[i].type == 1)
		//	memmove((unsigned char *)(dest + phdr[i].p_vaddr - start),(unsigned char *)(src + phdr[i].p_offset),phdr[i].p_filesz);
		if(phdr[i].type == 1)
			memcpy(phdr[i].p_vaddr,src + phdr[i].p_offset,phdr[i].p_filesz);
	}
	//t_writevals();
	//int (*main)() = (int*)(dest + hdr->entry - start);
//	int *pntr = (int*)(dest + hdr->entry - start);
	int *pntr = hdr->entry;
	return pntr;
}
/*int exec(char *name,char **argv,char **env){
	int size = fsize(name);
	uint8_t *file = malloc(size);
	int fd = open(name,O_RDONLY,0);
	int val = read(fd,file,size);
	if(val <= 0)
		return -1;
	uint8_t *dest = malloc(size);
	exec_elf(dest,file);
	return 1;
}*/
int exec_elf_args(void *dest,void *src,int argc,char **argv){
	uint8_t *u8src = (uint8_t*)src;
	uint8_t *u8dest = (uint8_t*)dest;
	struct Elf32_Hdr *hdr = (struct Elf32_Hdr*)src;
	struct elf32_Phdr *phdr = (struct elf32_Phdr*)(src + hdr->phdr_pos);
	int start;
	if(hdr->e_ident[0] != 0x7f || hdr->e_ident[1] != 'E' || hdr->e_ident[2] != 'L' || hdr->e_ident[3] != 'F'){
                print("Invalid sig\n");
                return -1;
        }

	for(int i = 0; i < hdr->num_ent_phdr;i++){
		if(!start)
			start = phdr[i].p_vaddr;
		if(phdr[i].type == 1)
			memmove((dest + phdr[i].p_vaddr - start),(src + phdr[i].p_offset),phdr[i].p_filesz);
	}
	t_writevals();
	int (*main)(int argc,char *argv[]) = (int*)(dest + hdr->entry - start);
	return main(argc,argv);
}
int elf_get_size(void *mem){
	struct Elf32_Hdr *hdr = (struct Elf32_Hdr *)mem;
	if(hdr->e_ident[0] != 0x7f || hdr->e_ident[1] != 'E' || hdr->e_ident[2] != 'L' || hdr->e_ident[3] != 'F'){
                print("Invalid sig\n");
                kprintf("{[%d],[%c],[%c],[%c]}\n",hdr->e_ident[0],hdr->e_ident[1],hdr->e_ident[2],hdr->e_ident[3]);
                while(1);
        }
	if(hdr->e_ident[4] != 1){
                print("Invalid elf class\n");
                while(1);
        }
        if(hdr->e_ident[5] != 1){
                print("Not little endian\n");
                while(1);
        }
	struct elf32_Phdr *phdr = (struct elf32_Phdr*)(mem + hdr->phdr_pos);
	uint8_t *buf = (uint8_t*)mem + hdr->phdr_pos;
	int ret = 0;
	for(int i = 0; i < hdr->num_ent_phdr;i++){
		if(phdr[i].type == 1)
			ret+=phdr[i].p_filesz;
	}

	return ret;

}
