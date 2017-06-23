/*
*Expiremental File System driver for my operating system
*(c) 2017 Zachary James Schlotman
*/
#ifdef __BUILD_FS
#include <mem.h>
#include "str.h"
#include "fs.h"
#include <lib.h>
#include <stdint.h>
int libkern_getlba(const char *mount_path){
	struct fs_info *pntr = (struct fs_info *)0x500;
	a:if(pntr->alloc == 1 && strcmp(pntr->mount_path,mount_path) == 0){
		return pntr->start_lba;
	}else{
		if(pntr->alloc == 0){
			kprintf("Couldn't find mountpoint %s\n",mount_path);
			return -1;
		}
		pntr = pntr + sizeof(*pntr);
		goto a;
	}
}
struct fs_info *libkern_getfs(const char *mount_path){
	struct fs_info *pntr = (struct fs_info *)0x500;
	a:if(pntr->alloc == 1 && strcmp(pntr->mount_path,mount_path) == 0)
		return pntr;
	else{
		if(pntr->alloc == 0){
			kprintf("Couldn't find mount point %s\n",mount_path);
			return -1;
		}
		pntr = pntr + sizeof(*pntr);
		goto a;
	}
}
int mount(struct fs_info *fs){
	struct fs_info *pntr = (struct fs_info *)0x000;
	a:if(pntr->alloc == 0){
		pntr->alloc = fs->alloc;
		pntr->start_lba = fs->start_lba;
		pntr->mount_path_len = fs->mount_path_len;
		pntr->mount_path = fs->mount_path;
	}else{
		pntr = pntr + sizeof(*pntr);
		goto a;
	}
	return 1;
}
int __mkfs(const char *mnt){
//	char sig[4] = {0x6f,'z','f','s'};
	struct fs_info *fs = libkern_getfs(mnt);
	sblk *superblk = malloc(512);
	strcpy(superblk->sig,sig);
	superblk->size = sizeof(sblk);
	superblk->root_lba = fs->start_lba + 1;
	superblk->root_offset = 0;
	char *raw = malloc(512);
	for(int i = 0; i < 512;i++)
		raw[i] = 0;
	for(int i = 0; i < 4;i++)
		raw[i] = superblk->sig[i];
	raw[4] = superblk->size;
	raw[5] = superblk->root_lba;
	raw[6] = superblk->root_lba >> 8;
	raw[7] = superblk->root_lba >> 16;
	raw[8] = superblk->root_lba >> 24;
	raw[9] = superblk->root_offset >> 8;
	raw[10] = superblk->root_offset;
	raw[11] = superblk->root_offset >> 16;
	#ifdef DEBUG
	kprintf("Writing to lba %d\n",fs->start_lba);
	#endif
	ata_write_master(raw,fs->start_lba);
	__libk_dir_ent *ent = malloc(512);
	ent->alloc = 1;
	ent->size = sizeof(*ent);
	ent->namelen = 1;
	ent->name = malloc(8);
	ent->name[0] = '/';
	ent->perms = 0;
	ent->nxt_ent_lba = 0;
	ent->nxt_ent_offset = 0;
	ent->nxt_dirent_lba;
	ent->nxt_dirent_offset = 0;
	raw = malloc(512);
	for(int i = 0; i < 512;i++)
		raw[i] = 0;
	raw[0] = ent->alloc;
	raw[1] = ent->size;
	raw[2] = ent->namelen;
	raw[3] = ent->name[0];
	raw[4] = ent->perms;
	#ifdef DEBUG
	kprintf("Writing to lba %d\n",superblk->root_lba);
	#endif
	ata_write_master(raw,superblk->root_lba);
	return 1;
}
int isfs(){
	uint8_t *buf = malloc(512);
	ata_read_master(buf,libkern_getlba("/"),0);
	if(buf[0] == 0x6f && buf[1] == 'z' && buf[2] == 'f' && buf[3] == 's')
		return 1;
	return 0;
}
sblk *parse_sblk(const char *mount_point){
	int lba = libkern_getlba(mount_point);
	if(lba == -1){
		error("0001");
		panic();
	}
	//static const uint8_t sig[4] = {0x6f,'z','b','l'};
	kprintf("Reading from lba %d\n",lba);
	sblk *ret = malloc(1024);
	uint8_t *buf = malloc(512);
	ata_read_master(buf,lba,0);
	strncpy(ret->sig,buf,4);
	if(strcmp(ret->sig,sig) != 0){
		kprintf("Invalid superblock expected bytes %s got bytes %s\n",sig,ret->sig);
		panic();
	}
	ret->size = buf[4];
	ret->root_lba = buf[8] << 24 | buf[7] << 16 | buf[6] << 8 | buf[5];
	ret->root_offset = buf[10] << 8  | buf[9];
	#ifdef DEBUG
	kprintf("ret->size = %d ret->root_lba = %d offset = %d\n",ret->size,ret->root_lba,ret->root_offset);
	#endif
	return ret;
}
FILE *__fopen(const char *path,char *atrib){

}
int fclose(FILE *f){

}
__libk_file_ent *get_file_ent(const char *name,__libk_ent *ent){

}
__libk_dir_ent *get_dir_ent(const char *name,__libk_dir_ent *prevdir){
	uint32_t lba,offset;
	if(prevdir == 0){
		struct fs_info *fs = libkern_getfs("/");
		if(fs == -1){
			error("0000");
			panic();
		}
		sblk *superblock = parse_sblk("/");
		lba = superblock->root_lba;
		offset = superblock->root_offset;
	}else{
		lba = prevdir->nxt_dirent_lba;
		offset = prevdir->nxt_dirent_offset;
	}
	__libk_dir_ent *ret = malloc(512);
	a:;uint8_t *buf = malloc(512);
	ata_read_master(buf,lba,0);
	int equals = 1;
	if(buf[2] == strlen(name))
		for(int i = 0; i < strlen(name);i++)
			if(buf[3 + i + offset] != name[i])
				equals = 0;
	if(equals){
		ret->alloc = buf[offset];
		ret->size = buf[offset + 1];
		ret->namelen = buf[offset + 2];
		ret->name = malloc(ret->namelen);
		for(int i = 0; i < ret->namelen;i++)
			ret->name[i] = buf[3 + offset + i];
		ret->perms = buf[6 + offset + ret->namelen] << 16 | buf[5 + offset + ret->namelen]  << 8| buf[4 + offset + ret->namelen];
		ret->nxt_ent_lba = buf[10 + ret->namelen + offset] << 24 | buf[9 + ret->namelen + offset] << 16 | buf[ret->namelen + 8 + offset] << 8 | buf[7 + ret->namelen + offset];
		ret->nxt_ent_offset = buf[12 + ret->namelen + offset] << 8 | buf[11 + ret->namelen + offset];
		ret->nxt_dirent_lba = buf[16 + ret->namelen + offset] << 24 | buf[15 + ret->namelen + offset] << 16 | buf[14 + ret->namelen + offset] << 8 | buf[13 + ret->namelen + offset];
		ret->nxt_dirent_offset = buf[18 + ret->namelen + offset] << 8 | buf[17 + ret->namelen + offset];
	}else{
		lba = buf[16 + buf[2] + offset] << 24 | buf[15 + buf[2] + offset] << 18 | buf[14 + buf[2] + offset] << 8 | buf[13 + buf[2] + offset];
		offset = buf[18 + buf[2] + offset] << 8 | buf[18 + buf[2] + offset];
		if(lba == 0){
			kprintf("Directory not found!\n");
			return -1;
		}
		goto a;
	}
}
__libk_ent *get_ent(const char *name,__libk_dir_ent *dent,int type){

}
int write_file_ent(const char *name,__libk_ent *ent){

}
uint32_t find_avaliable_lba(int size){
	uint32_t ret;
	sblk *superblk = parse_sblk("/");
	ret = superblk->root_lba;
	int offset = 0;
	uint8_t *buf = malloc(512);
	a:;ata_read_master(buf,ret,0);
	b:;if(buf[offset] == 0 && offset <= (512 - size))
		return ret;
	else
		return ++ret;
	if(offset >= 512){
		ret++;
		offset = 0;
		goto a;
	}
	offset = buf[offset + 18] << 8 | buf[offset + 17];
	if(offset = 0){
		ret++;
		goto a;
	}
	goto b;
}
int offsetAt(uint32_t lba){
	uint8_t *buf = malloc(512);
	ata_read_master(buf,lba,0);
	int ret = 0;
	a:;if(buf[ret] == 0)
		return ret;
	if(ret >= 512)
		return -1;
	ret = buf[ret + 18] << 8 | buf[ret + 17];
	if(ret == 0)
		return -1;
	goto a;
}
#ifdef OLD
int write_ent(const char *name,__libk_dir_ent *prev,__libk_ent *went){
	if(prev->nxt_ent_lba != 0){
		error("004");
		return -1;
	}
	prev->nxt_ent_lba = find_avaliable_lba(sizeof(*went));
	prev->nxt_ent_offset = offsetAt(prev->nxt_ent_lba);
	if(prev->nxt_ent_offset < 0){
		error("005");
		return -1;
	}
	uint8_t *buf = malloc(512);
	ata_read_master(buf,prev->nxt_ent_lba,0);
	int offset;
	if((512 - prev->nxt_ent_offset - sizeof(struct ent)) <= 0)
		offset = 0;
	else
		offset = prev->nxt_ent_offset;
	buf[offset] = went->alloc;
	buf[offset + 1] = went->size;
	buf[offset + 2] = went->namelen;
	for(int i = 0; i < went->namelen;i++)
		buf[offset + 3 + i] = went->name[i];
	buf[offset + 4 + went->namelen] = went->perms;
	buf[offset + 5 + went->namelen] = went->perms >> 8;
	buf[offset + 6 + went->namelen] = went->perms >> 16;
	buf[offset + 7 + went->namelen] = went->perms >> 24;
	buf[offset + 8 + went->namelen] = went->type;
	buf[offset + 9 + went->namelen] = went->lba;
	buf[offset + 10 + went->namelen] = went->lba >> 8;
	buf[offset + 11 + went->namelen] = went->lba >> 16;
	buf[offset + 12 + went->namelen] = went->lba >> 24;
	buf[offset + 13 + went->namelen] = went->offset;
	buf[offset + 14 + went->namelen] = went->offset >> 8;
	buf[offset + 15 + went->namelen] = went->nxt_ent_lba;
	buf[offset + 16 + went->namelen] = went->nxt_ent_lba >> 8;
	buf[offset + 17 + went->namelen] = went->nxt_ent_lba >> 16;
	buf[offset + 18 + went->namelen] = went->nxt_ent_lba >> 24;
	buf[offset + 19 + went->namelen] = went->nxt_ent_offset;
	buf[offset + 20 + went->namelen] = went->nxt_ent_offset >> 8;
}
#endif
int modify_dir_ent(const char *name,__libk_dir_ent *ent){
	sblk *superblk = parse_sblk("/");
	int lba = superblk->root_lba;
	int offset = superblk->root_offset;
	uint8_t *buf = malloc(512);
	a:;ata_read_master(buf,lba,0);
	int namelen = buf[offset + 2];
	if(namelen != strlen(name)){
		offset = buf[offset + 18] << 8 | buf[offset + 17];
		lba = buf[offset + 16] << 24 | buf[offset + 15] << 16 | buf[offset + 14] << 8 | buf[offset + 13];
		goto a;
	}
	char *_name = malloc(namelen * 8);
	for(int i = 0;i < namelen;i++)
		_name[i] = buf[offset + 3 + i];
	if(strcmp(name,_name) == 0 && buf[offset] == 1){
		buf[offset] = ent->alloc;
		buf[offset + 1] = ent->size;
		buf[offset + 2] = ent->namelen;
		for(int i = 0; i < buf[offset + 2];i++)
			buf[offset + 3 + i] = ent->name[i];
		buf[offset + 4 + ent->namelen] = ent->perms;
		buf[offset + 5 + ent->namelen] = ent->perms >> 8;
		buf[offset + 6 + ent->namelen] = ent->perms >> 16;
		buf[offset + 7 + ent->namelen] = ent->nxt_ent_lba;
		buf[offset + 8 + ent->namelen] = ent->nxt_ent_lba >> 8;
		buf[offset + 9 + ent->namelen] = ent->nxt_ent_lba >> 16;
		buf[offset + 10 + ent->namelen] = ent->nxt_ent_lba >> 24;
		buf[offset + 11 + ent->namelen] = ent->nxt_ent_offset;
		buf[offset + 12 + ent->namelen] = ent->nxt_ent_offset >> 8;
		buf[offset + 13 + ent->namelen] = ent->nxt_dirent_lba;
		buf[offset + 14 + ent->namelen] = ent->nxt_dirent_lba >> 8;
		buf[offset + 15 + ent->namelen] = ent->nxt_dirent_lba >> 16;
		buf[offset + 16 + ent->namelen] = ent->nxt_dirent_lba >> 24;
		buf[offset + 17 + ent->namelen] = ent->nxt_dirent_offset;
		buf[offset + 18 + ent->namelen] = ent->nxt_dirent_offset >> 8;
		ata_write_master(buf,lba);
		return 1;
	}
	lba = buf[offset + 16 + buf[offset + 2]] << 24 | buf[offset + 15 + buf[offset + 2]] << 16 | buf[offset + 14 + buf[offset + 2]] << 8 | buf[offset + 13 + buf[offset + 2]];
	if(lba == 0){
		kprintf("Error: No such file or directory!\n");
		return -1;
	}
	offset = buf[offset + 18 + buf[offset + 2]] << 8 | buf[offset + 17 + buf[offset + 2]];
	goto a;
}
int write_dir_ent(const char *name,__libk_dir_ent *prev,__libk_dir_ent *went){
	if(prev->nxt_dirent_lba != 0){
		error("002");
		return -1;
	}
	prev->nxt_dirent_lba = find_avaliable_lba(sizeof(*prev));
	prev->nxt_dirent_offset = offsetAt(prev->nxt_dirent_lba);
	if(prev->nxt_dirent_offset < 0){
		error("003");
		return -1;
	}
	modify_dir_ent(prev->name,prev);
	uint8_t *buf = malloc(512);
	ata_read_master(buf,prev->nxt_dirent_lba,0);
	int offset;
	if((512 - prev->nxt_dirent_offset - sizeof(struct dirent)) <= 0)
		offset = 0;
	else
		offset = prev->nxt_dirent_offset;
	buf[offset] = went->alloc;
	buf[offset + 1] = went->size;
	buf[offset + 2] = went->namelen;
	for(int i = 0; i < went->namelen;i++)
		buf[offset + 3 + i] = went->name[i];
	buf[offset + 4 + went->namelen] = went->perms;
	buf[offset + 5 + went->namelen] = went->perms >> 8;
	buf[offset + 6 + went->namelen] = went->perms >> 16;
	buf[offset + 7 + went->namelen] = went->nxt_ent_lba;
	buf[offset + 8 + went->namelen] = went->nxt_ent_lba >> 8;
	buf[offset + 9 + went->namelen] = went->nxt_ent_lba >> 16;
	buf[offset + 10 + went->namelen] = went->nxt_ent_lba >> 24;
	buf[offset + 11 + went->namelen] = went->nxt_ent_offset;
	buf[offset + 12 + went->namelen] = went->nxt_ent_offset >> 8;
	buf[offset + 13 + went->namelen] = went->nxt_dirent_lba;
	buf[offset + 14 + went->namelen] = went->nxt_dirent_lba >> 8;
	buf[offset + 15 + went->namelen] = went->nxt_dirent_lba >> 16;
	buf[offset + 16 + went->namelen] = went->nxt_dirent_lba >> 24;
	buf[offset + 17 + went->namelen] = went->nxt_dirent_offset;
	buf[offset + 18 + went->namelen] = went->nxt_dirent_offset >> 8;
	ata_write_master(buf,prev->nxt_dirent_lba);
}
int has_dir(__libk_dir_ent *ent,const char *name){
	while(ent->nxt_dirent_lba != 0){
                char *buf = malloc(1024);
                ata_read_master(buf,ent->nxt_dirent_lba,0);
                __libk_dir_ent *nxt = malloc(1024);
                nxt->alloc = buf[ent->nxt_dirent_offset];
                nxt->size = buf[ent->nxt_dirent_offset + 1];
                nxt->namelen = buf[ent->nxt_dirent_offset + 2];
                nxt->name = malloc(80);
                for(int i = 0; i < nxt->namelen;i++)
                        nxt->name[i] = buf[ent->nxt_dirent_offset + 3 + i];
		if(strcmp(nxt->name,name) == 0)
			return 1;
                nxt->perms = buf[ent->nxt_dirent_offset + 6] << 16 | buf[ent->nxt_dirent_offset + ent->namelen + 5] << 8 | buf[ent->nxt_dirent_offset + ent->namelen + 4];
                nxt->nxt_ent_lba = buf[ent->nxt_dirent_offset + 10 + ent->namelen] << 24 | buf[ent->nxt_dirent_offset + 9 + ent->namelen] << 16 | buf[ent->nxt_dirent_offset + 8 + ent->namelen];
                nxt->nxt_ent_offset = buf[ent->nxt_dirent_offset + 12 + ent->namelen] << 8 | buf[ent->nxt_dirent_offset + ent->namelen + 11];
                nxt->nxt_dirent_lba = buf[ent->nxt_dirent_offset + 16 + ent->namelen] << 24 | buf[ent->nxt_dirent_offset + 15 + ent->namelen] << 16 | buf[ent->namelen + ent->nxt_dirent_offset + 14];
                nxt->nxt_dirent_offset = buf[ent->nxt_dirent_offset + ent->namelen + 18] << 8 | buf[ent->nxt_dirent_offset + ent->namelen + 17];
                ent = nxt;
	}
	return 0;
}
__libk_dir_ent *get_last_dirent(__libk_dir_ent *ent){
	while(ent->nxt_dirent_lba != 0){
		char *buf = malloc(1024);
		ata_read_master(buf,ent->nxt_dirent_lba,0);
		__libk_dir_ent *nxt = malloc(1024);
		nxt->alloc = buf[ent->nxt_dirent_offset];
		nxt->size = buf[ent->nxt_dirent_offset + 1];
		nxt->namelen = buf[ent->nxt_dirent_offset + 2];
		nxt->name = malloc(80);
		for(int i = 0; i < nxt->namelen;i++)
			nxt->name[i] = buf[ent->nxt_dirent_offset + 3 + i];
		nxt->perms = buf[ent->nxt_dirent_offset + 6] << 16 | buf[ent->nxt_dirent_offset + ent->namelen + 5] << 8 | buf[ent->nxt_dirent_offset + ent->namelen + 4];
		nxt->nxt_ent_lba = buf[ent->nxt_dirent_offset + 10 + ent->namelen] << 24 | buf[ent->nxt_dirent_offset + 9 + ent->namelen] << 16 | buf[ent->nxt_dirent_offset + 8 + ent->namelen] << 8 | buf[ent->nxt_dirent_offset + ent->namelen + 7];
		nxt->nxt_ent_offset = buf[ent->nxt_dirent_offset + 12 + ent->namelen] << 8 | buf[ent->nxt_dirent_offset + ent->namelen + 11];
		nxt->nxt_dirent_lba = buf[ent->nxt_dirent_offset + 16 + ent->namelen] << 24 | buf[ent->nxt_dirent_offset + 15 + ent->namelen] << 16 | buf[ent->namelen + ent->nxt_dirent_offset + 14] << 8 | buf[ent->nxt_dirent_offset + 13 + ent->namelen];
		nxt->nxt_dirent_offset = buf[ent->nxt_dirent_offset + ent->namelen + 18] << 8 | buf[ent->nxt_dirent_offset + ent->namelen + 17];
		ent = nxt;
	}
	return ent;
}
#endif
