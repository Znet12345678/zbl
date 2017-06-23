#include <stdint.h>
#include "genfs.h"
#include "lib.h"
#include "mem.h"
int __is_genfs(){
	int lba = 0;
	uint8_t *buf = malloc(1024);
	_ata_read_master(buf,lba,0);
	struct __superblock *sblk = malloc(sizeof(*sblk)*sizeof(sblk));
	memcpy(sblk->sig,buf,sizeof(*sblk));
	if(sblk->sig[0] != 0x7f && sblk->sig[1] != 'G' && sblk->sig[2] != 'E' && sblk->sig[3] != 'N')
		return 0;
	return 1;
}
void __mkfs_genfs(){
	int lba = 0;
	struct __superblock *sblk = malloc(sizeof(*sblk)*sizeof(sblk));
	sblk->sig[0] = 0x7f;
	sblk->sig[1] = 'G';
	sblk->sig[2] = 'E';
	sblk->sig[3] = 'N';
	sblk->root_dir_offset = 0;
	sblk->root_dir_lba = 1;
	uint8_t *buf = malloc(1024);
	_ata_read_master(buf,0,0);
	memcpy(buf,sblk,sizeof(*sblk));
	_ata_write_master(buf,0);
	//kprintf("Writing root dir\n");
	int ret = mkdir("/");
}
struct __superblock *__genfs_parse_superblock(){
	uint8_t *buf = malloc(1024);
	_ata_read_master(buf,0,0);
	struct __superblock *ret = malloc(sizeof(*ret)*sizeof(ret));
	memcpy(ret,buf,sizeof(*ret));
	return ret;
}
struct __ent *__parse_ent(uint8_t *pntr,int offset){
	struct __ent *ret = malloc(sizeof(*ret)*sizeof(ret));
	memcpy(ret,pntr + offset,sizeof(*ret));
	return ret;
}
struct __data *__find_free_blk(){
	//kprintf("Searching disk...\n");
	struct __superblock *sblk = __genfs_parse_superblock();
	int lba = sblk->root_dir_lba;
	int offset = sblk->root_dir_offset;
	uint8_t *buf = malloc(1024);
	a:;
	if(offset > 512){
		offset = 0;
		lba++;
	}
	for(int i = 0; i < 512;i++)
		buf[i] = 0;
	_ata_read_master(buf,lba,0);
	struct __data *ret = malloc(sizeof(*ret)*sizeof(ret));
	if(buf[offset] == 0){
		ret->lba = lba;
		ret->offset = offset;
		return ret;
	}else if(buf[offset + 1] == 0){
		struct __ent *ent = ___parse_ent(lba,offset);
		//kprintf("%d %d\n",offset,sizeof(*ent));
		if(offset +sizeof(*ent) < 512)
			offset+=sizeof(*ent);
		else{
			offset = 0;
			lba++;
		}
	}else if(buf[offset + 1] == 1){
		//kprintf("%d %d\n",offset,sizeof(struct __data_ent));
		struct __data_ent *dent = __parse_dent(lba,offset);
		if(offset + sizeof(*dent) > 512){
			offset = 0;
			lba++;
		}else
			offset+=sizeof(*dent);
	}else if(buf[offset + 1] == 4){
		struct __fdat *fdat = __parse_fdat(lba,offset);
		lba+=fdat->tlba;
	}else{
		offset = 0;
		lba++;
	}
	goto a;
}
struct __data *___find_free_blk(){
	struct __superblock *sblk = __genfs_parse_superblock();
	int lba = sblk->root_dir_lba + 1;
	int offset = 0;
	uint8_t *buf = malloc(1024);
	a:;_ata_read_master(buf,lba,0);

	struct __data *ret = malloc(sizeof(*ret)*sizeof(ret));
	if(buf[offset] == 0){
		ret->lba = lba;
		ret->offset=  offset;
		return ret;
	}
	if(buf[offset + 1] == 0){
		struct __ent *ent = malloc(sizeof(*ent)*sizeof(ent));
		memcpy(ent,buf + offset,sizeof(*ent));
		/*if(ent->nxt_ent_lba != 0){
			lba = ent->nxt_ent_lba;
			offset = ent->nxt_ent_offset;
		}else{*/
		if(offset + sizeof(*ent) < 512)
			offset+=sizeof(*ent);
		else{
			offset = 0;
			lba++;
		}
		goto a;
	}else if (buf[offset + 1] == 1){
		struct __data_ent *ent = malloc(sizeof(*ent) * sizeof(ent));
		memcpy(ent,buf + offset,sizeof(*ent));
		if(offset + sizeof(*ent) < 512)
			offset+=sizeof(*ent);
		else{
			offset = 0;
			lba++;
		}
		goto a;
	}else if(buf[offset + 2] == 2){
		struct __file_ent *fent = malloc(sizeof(*fent));
		memcpy(fent,buf + offset,sizeof(*fent));
		if(offset + sizeof(*fent) < 512)
			offset+=sizeof(*fent);
		else{
			offset = 0;
			lba++;
		}
		goto a;
	}
	kprintf("Couldn't find free\n");
	panic();
}
int write_file(char *fpath,uint8_t *dat, int n){
	/*struct __data_ent *dent = malloc(1024);
	char *path = malloc(1024);
	char **arrpath = sep(fpath,'/');
	int last = 0;
	while(arrpath[last] != 0)
		last++;
	if(fpath[strlen(fpath) - 1] != '/')
		last++;
//	kprintf("%s\n",arrpath[last - 2]);
	strcpy(path,"/");
	for(int i = 0; i < last - 2;i++){
		strcat(path,arrpath[i]);
		strcat(path,"/");
	}
//	kprintf("Opening dir %s\n",path);
	DIR *d = opendir(path);
	if(!d){
		kprintf("Failed to open dir %s!\n",path);
		return 0;
	}
	struct __ent *e = ___parse_ent(d->dent->data_lba,d->dent->data_offset);
	int prev_lba = d->dent->data_lba,prev_offset = d->dent->data_offset;
	struct __data *_d = __find_free_blk();
	while(e->alloc == 1 && e->nxt_ent_lba > 0){
		prev_lba = e->nxt_ent_lba;
		prev_offset = e->nxt_ent_offset;
		struct __ent *e = ___parse_ent(e->nxt_ent_lba,e->nxt_ent_offset);
	}
	e->nxt_ent_lba = _d->lba;
	e->nxt_ent_offset = _d->offset;
	uint8_t *buf = malloc(1024);
	_ata_read_master(buf,prev_lba,0);
	memcpy(buf + prev_offset,e,sizeof(*e));
//	kprintf("%d:%d\n",prev_lba,prev_offset);
	_ata_write_master(buf,prev_lba);
	struct __ent *nent = malloc(sizeof(*nent) * 2);
	nent->alloc = 1;
	nent->ent_type = 0;
	nent->type = TYPE_FILE;
	nent->namelen = strlen(arrpath[last - 2]);
	strcpy(nent->name,arrpath[last - 2]);
	nent->data_ent_lba = 0;
	nent->data_ent_offset = 0;
	nent->nxt_ent_lba = 0;
	nent->nxt_ent_offset = 0;
	_ata_read_master(buf,_d->lba,0);
	memcpy(buf + _d->offset,nent,sizeof(*nent));
//	kprintf("[%d]:[%d]\n",_d->lba,_d->offset);
	_ata_write_master(buf,_d->lba);
	int pre_lba = _d->lba,pre_offset = _d->offset;
	_d = __find_free_blk();
	nent->data_ent_lba = _d->lba;
	nent->data_ent_offset = _d->offset;
	memcpy(buf + pre_offset,nent,sizeof(*nent));
//	kprintf("Writing %s to %d:%d\n",arrpath[last - 2],pre_lba,pre_offset);
	_ata_write_master(buf,pre_lba);
	int left = n;
	struct __data *_dat = __find_free_blk();
	int wrt_lba = _dat->lba,wrt_offset = _dat->offset;
	while(left > 0){
		struct __fdat *fdat = malloc(sizeof(*fdat));
		fdat->alloc = 1;
		fdat->ent_type = 2;
		uint8_t *wrt = malloc(1024);
//		kprintf("Writing to %d:%d\n",wrt_lba,wrt_offset);
		_ata_read_master(wrt,wrt_lba,0);
		memcpy(wrt + wrt_offset,fdat,sizeof(*fdat));
		_ata_write_master(wrt,wrt_lba);
		if(left - (512 - sizeof(*fdat) - wrt_offset) <= 0){
			fdat->nxt_fdat_lba = 0;
			fdat->nxt_fdat_offset = 0;
		}else{
			_dat = __find_free_blk();
			fdat->nxt_fdat_lba = _dat->lba;
			fdat->nxt_fdat_offset =_dat->offset;
		}
		memcpy(wrt + wrt_offset,fdat,sizeof(*fdat));
		memcpy(wrt + wrt_offset,dat,512 - sizeof(*fdat) - wrt_offset);
//		kprintf("Writing to [%d:%d]\n",wrt_lba,wrt_offset);
		_ata_write_master(wrt,wrt_lba);
		left-=512 - sizeof(*fdat) - wrt_offset;
	}
	return 1;*/
}
struct __fdat *__parse_fdat(int lba,int offset){
	uint8_t *buf = malloc(1024);
	_ata_read_master(buf,lba,0);
	struct __fdat *fent = malloc(sizeof(*fent));
	memcpy(fent,buf + offset,sizeof(*fent));
	//kprintf("%d\n",buf[offset]);
	return fent;
}
struct __ent *get_fdat(const char *name){
	char **arr = sep(name,'/');
	int last = 0;
	while(arr[last] != 0)
		last++;
	char *prev = malloc(1024);
	int i = 0;
	strcpy(prev,"/");
	while(i < last - 1){
		strcat(prev,arr[i]);
		strcat(prev,"/");
		i++;
	}
	DIR *d = opendir(prev);
	if(!d)
		return 0;
	struct __ent *ent = ___parse_ent(d->dent->data_lba,d->dent->data_offset);
	int found = 0;
	while(ent->alloc == 1){
		if(strcmp(ent->name,arr[last - 1]) == 0){
			found = 1;
			break;
		}
		ent = ___parse_ent(ent->nxt_ent_lba,ent->nxt_ent_offset);
	}
	if(!found){
		kprintf("No such file or directory! (%s)\n",arr[last - 1]);
		return 0;
	}
	return ent;
}
uint32_t fsize(const char *name){
	struct __ent *ent = get_fdat(name);
	if(!ent){
		kprintf("Couldn't open %s\n",name);
		return 0;
	}
	struct __fdat *fdat = __parse_fdat(ent->data_ent_lba,ent->data_ent_offset);
	//kprintf("%d %d %d\n",fdat->alloc,fdat->ent_type,fdat->tlba);
	return fdat->tlba * 512;

}
int read_file(const char *name,uint8_t *pntr){
	struct __ent *ent = get_fdat(name);
	struct __fdat *fdat = __parse_fdat(ent->data_ent_lba,ent->data_ent_offset);
	unsigned int i = 0;
	uint8_t *buf;
	unsigned int start = ent->data_ent_lba + 1;
	while(i < fdat->tlba){
		buf = (uint8_t*)(pntr + (i * 512));
		for(int i = 0; i < 512;i++)
			buf[i] = 0;
		_ata_read_master(buf,(start + i),0);
		i++;
	}
	return 1;
	/*int bytes = 0,lba = ent->data_ent_lba,offset = ent->data_ent_offset;
	int i = 0;
	uint8_t *u8pntr = (uint8_t*)pntr;
	uint8_t *buf = malloc(1024);
	while(fdat->alloc == 1){
		for(int i = 0; i < 512;i++)
			buf[i] = 0;
		int err = _ata_read_master(buf,lba,0);
		if(!err){
			kprintf("Error reading lba %d\n",lba);
			return 0;
		}
		memmove((uint8_t*)(pntr + bytes),(uint8_t*)(buf + (sizeof(*fdat) + offset)),512 - sizeof(*fdat) - offset);
		bytes+=(512 - (sizeof(*fdat) + offset));
		offset = fdat->nxt_fdat_offset;
		lba = fdat->nxt_fdat_lba;
		fdat = __parse_fdat(fdat->nxt_fdat_lba,fdat->nxt_fdat_offset);
		i++;
	}
	return 1;*/
}
struct fd *alloc_fd(){
	struct fd *f = (struct fd *)0x00007E00;
	int ret = 0;
	while(f->alloc == 1){
		f+=sizeof(*f);
		ret++;
	}
	f->alloc = 1;
	return ret;
}
int open(const char *fname,int flags,int mode){
	int ret = alloc_fd();
	struct fd *f = (struct fd *)(0x00007E00 + ret * sizeof(struct fd));
	//kprintf("%s\n",fname);
	memcpy(f->name,fname,80);
	f->pos_lba = 0;
	f->pos_offset = 0;
	f->flags = flags;
	return ret;
	/*int *buf = (int*)0x00007E00;
	memcpy(buf,fname,80);
	int *pos = malloc(1024);
	uint32_t pos_lba = 0;
	uint16_t pos_offset = 0;
	memcpy(buf + strlen(fname),&pos_lba,sizeof(uint32_t));
	memcpy(buf + strlen(fname) + sizeof(uint32_t),&pos_offset,sizeof(uint16_t));
	memcpy(buf + strlen(fname) + sizeof(uint32_t) + sizeof(uint16_t),&flags,sizeof(uint8_t));
	return *buf;
	struct KFILE *kf = malloc(1024);
	if(flags | O_RDONLY == flags ){

		kf->fent = get_fdat(fname);
		kf->fdat = __parse_fdat(kf->fent->data_ent_lba,kf->fent->data_ent_offset);
		kf->finfo = malloc(sizeof(*kf->finfo));
		pos_lba = 0;
		pos_offset = 0;
		flags = flags;
		kprintf("%d %d\n",pos_lba,kf->fent->data_ent_lba);
		return *(int*)kf;
	}*/
}
KFILE *_open(const char *fname){
	/*struct KFILE *kf = malloc(sizeof(*kf));
	kf->fent = get_fdat(fname);
	kf->fdat = __parse_fdat(kf->fent->data_ent_lba,kf->fent->data_ent_offset);
	pos_lba = 0;
	pos_offset = 0;
        flags = 0;
	return kf;*/
}
int read(int fd,void *buf,int n){
	//struct fd *f = (struct fd *)(0x00007E00 + fd * sizeof(*f));
	struct fd *f = (struct fd *)(0x00007E00 + fd * sizeof(*f));
//	kprintf("%s\n",f->name);
	struct KFILE *kf = malloc(1024);
	kf->fent = get_fdat(f->name);
	kf->fdat = __parse_fdat(kf->fent->data_ent_lba,kf->fent->data_ent_offset);
	//kf->fent = get_fdat(&fd);
	//kf->fdat = __parse_fdat(kf->fent->data_ent_lba,kf->fent->data_ent_offset);
	//uint32_t pos_lba = (uint32_t)((&fd) + strlen(fname));
	//uint16_t pos_offset = (uint16_t)((&fd) + strlen(fname) + sizeof(uint32_t));
	//uint8_t flags = (uint8_t)((&fd) + strlen(fname) + sizeof(uint32_t) + sizeof(uint16_t));
	uint32_t pos_lba = f->pos_lba;
	uint16_t pos_offset = f->pos_offset;
	uint8_t flags = f->flags;
//	kprintf("%d %d %d %d\n",pos_lba,pos_offset,kf->fent->data_ent_lba,kf->fent->data_ent_offset);
	if((n) > (kf->fdat->tlba * 512 - pos_lba * 512 - pos_offset)){
		//kprintf("1\n");
	//	kprintf("R:%d->%d\n",pos_lba + kf->fent->data_ent_lba,kf->fent->data_ent_lba + pos_lba + kf->fdat->tlba * 512);

		int written = kf->fdat->tlba * 512 - pos_lba * 512 - pos_offset;
		if(written <= 0){
			kprintf("Less than zero\n");
			return 0;
		}
		uint8_t *tmp = malloc(1024);
		for(int i = pos_lba + kf->fent->data_ent_lba,j = 0;i < kf->fent->data_ent_lba + pos_lba + (kf->fdat->tlba) * 512;j++,i++){
//			if(i != pos_lba)
//				_ata_read_master(buf + (j * 512),kf->fent->data_ent_lba + j + 1,0);
//			else
//				_ata_read_master(buf + (j * 512),kf->fent->data_ent_lba + j + 1,0);
			ata_read_master(tmp,kf->fent->data_ent_lba +j + 1,0);
			if(i != pos_lba)
				memcpy(buf + j * 512,tmp,512);
			else
				memcpy(buf + j * 512,tmp + pos_offset,512 - pos_offset);
		}
		pos_lba+=kf->fdat->tlba * 512 - pos_lba;
		pos_offset = 0;
		return written;
	}else{
		//kprintf("2\n");
		uint8_t *tmp = malloc(1024);
	//	kprintf("R:%d:%d->%d:%d\n",pos_lba + kf->fent->data_ent_lba,pos_offset,kf->fent->data_ent_lba + pos_lba + n/512,0);
		int bytestowrite = n;
		int add;
		if(n/512 == 0)
			add = 1;
		else
			add = n/512;
		for(int i = pos_lba + kf->fent->data_ent_lba,j = 0; i < (kf->fent->data_ent_lba + pos_lba + add);j++,i++){
			_ata_read_master(tmp,kf->fent->data_ent_lba + j + 1,0);
			//kprintf("%d\n",bytestowrite);
//			if(i != pos_lba)
//				_ata_read_master(buf + (j * 512),j + pos_lba +kf->fent->data_ent_lba,0);
//			else
//				_ata_read_master(buf + (j * 512),j + pos_lba + kf->fent->data_ent_lba
			if(bytestowrite < 512 && i != (kf->fent->data_ent_lba + pos_lba + n/512)){
				memcpy(buf + j * 512,tmp,bytestowrite);
				bytestowrite-=bytestowrite;
			}else if (bytestowrite < 512){
				memcpy(buf + j * 512,tmp + pos_offset,bytestowrite);
				bytestowrite-=bytestowrite;
			}
			else if(i == pos_lba){
				memcpy(buf + j * 512,tmp + pos_offset,512 - pos_offset);
				bytestowrite-=512 - pos_offset;
			}else
				memcpy(buf + j * 512,tmp,512);
		}
		pos_lba+=n/512;
		pos_offset=n%512;
		return n;
	}

}
int close(int fd){
	struct KFILE *kf = &fd;
	free(kf);
	return 1;
}
int _read_file(const char *name,void *pntr){
/*
	int last = 0;
	char **arr = sep(name,'/');
	while(arr[last] != 0)
		last++;
//	if(name[strlen(name) - 1] != '/')
//		last++;
	//kprintf("%d\n",last);
	char *prev = malloc(1024);
	int i = 0;
	strcpy(prev,"/");
	while(i < (last - 1)){
		strcat(prev,arr[i]);
		strcat(prev,"/");
		i++;
	}
	//kprintf("Opening %s\n",prev);
	DIR *d = opendir(prev);
	if(!d){
		kprintf("Failed to open dir %s\n",prev);
		return 0;
	}
	struct __ent *ent = ___parse_ent(d->dent->data_lba,d->dent->data_offset);
	int prev_lba = d->dent->data_lba,prev_offset = d->dent->data_offset;
	int found = 0;
	//kprintf("%d %s %s\n",last,arr[last - 1],name);
	while(ent->alloc == 1 && !found){

		prev_lba =ent->nxt_ent_lba;
		prev_offset = ent->nxt_ent_offset;
		//kprintf("%s\n",ent->name);
		if(strcmp(ent->name,arr[last - 1]) == 0){
			found = 1;
			break;
		}
		ent = ___parse_ent(ent->nxt_ent_lba,ent->nxt_ent_offset);
	}
	if(!found){
		kprintf("No such file or directory (%s)!\n",arr[last - 1]);
		return 0;
	}
	struct __fdat *fdat = __parse_fdat(ent->data_ent_lba,ent->data_ent_offset);
//	kprintf("%d\n",fdat->alloc);
	int bytes = 0;
	int offset = ent->data_ent_offset,lba = ent->data_ent_lba;
//	uint8_t *buf = malloc(1024);
	uint8_t buf[1024];
	while(fdat->alloc == 1){
		//kprintf("%d %d \n",lba,offset);
		int err = _ata_read_master(buf,lba,0);
		if(!err){
			kprintf("Error reading lba %d\n",lba);
			panic();
		}
//		kprintf("%d\n",buf[offset + sizeof(*fdat)]);
		memcpy(pntr + bytes,buf + sizeof(*fdat) + offset,512 - sizeof(*fdat) - offset);
		bytes+=512 - sizeof(*fdat) - offset;
		lba = fdat->nxt_fdat_lba;
		offset = fdat->nxt_fdat_offset;
		fdat = __parse_fdat(lba,offset);
	}
*/
}
int __exec(const char *path){
	int _size = fsize(path);
	kprintf("Allocating %d bytes\n",_size);
	uint8_t *buf = (uint8_t*)malloc(_size);
	kprintf("Reading...\n");
	read_file(path,buf);
	int size = elf_get_size(buf);
	kprintf("Allocating %d bytes and Executing...\n",size);
	//uint8_t *mem = (uint8_t*)0x100000;
	uint8_t *mem = (uint8_t*)0x00800000;
	int (*main)(void) = exec_elf(mem,buf);
	kprintf("Jumping\n");
	t_writevals();
	return main();
}
int __find_free_blk_lba(){
	struct __data *dat = __find_free_blk();
	return dat->lba;
}
int __find_free_blk_offset(){
	struct __data *dat = __find_free_blk();
	return dat->offset;
}
struct __ent * ___parse_ent(int lba,int offset){
	uint8_t *buf = malloc(1024);
	_ata_read_master(buf,lba,0);
	struct __ent *ret = malloc(sizeof(*ret)*sizeof(ret));
	memcpy(ret,buf + offset,sizeof(*ret));
	return ret;
}
struct __data_ent *__parse_dent(int lba,int offset){
	uint8_t *buf = malloc(1024);
	_ata_read_master(buf,lba,0);
	struct __ent *ret = malloc(sizeof(*ret)*sizeof(ret));
	memcpy(ret,buf + offset,sizeof(*ret));
}
FILE *fopen(const char *path,const char *opperation){
	if(strcmp(opperation,"rb") == 0){
		char **arr = sep(path,'/');
		int size = 0;
		while(arr[size] != 0)
			size++;
		char *dpath = malloc(1024);
		strcpy(dpath,"/");
		for(int i = 0; i < size - 1;i++)
			strcat(dpath,arr[i]);
		DIR *dir = opendir(dpath);
	}else if(strcmp(opperation,"wb") == 0){

	}else{
		kprintf("Unsupported opperaation %s\n",opperation);
		return 0;
	}
}
DIR *opendir(const char *path){
//	kprintf("Opening %s\n",path);
	DIR *ret = malloc(sizeof(*ret)*sizeof(ret));
	ret->ent = malloc(sizeof(*ret->ent)*sizeof(ret->ent));
	ret->dent = malloc(sizeof(*ret->dent)*sizeof(ret->dent));
	char **name = sep(path,'/');
	int last = 0;
	struct __superblock *sblk = __genfs_parse_superblock();
	while(name[last] != 0)
		last++;
	if(path[strlen(path) - 1] != '/')
		last++;
//	last--;
	//kprintf("[%d]\n",last);
	if(strcmp(path,"/") == 0){
		ret->ent = ___parse_ent(sblk->root_dir_lba,sblk->root_dir_offset);
		ret->dent = __parse_dent(ret->ent->data_ent_lba,ret->ent->data_ent_offset);
		return ret;
	}
	//kprintf("Opening root dir\n");
	DIR *root = opendir("/");
	//kprintf("%d %d\n",root->dent->data_lba,root->dent->data_offset);
	struct __ent *ent = ___parse_ent(root->dent->data_lba,root->dent->data_offset);
	int i = 0;
	//kprintf("Last: %d\n",last);
	while(i < last - 1){
		while(strcmp(ent->name,name[i]) != 0){
			if(ent->alloc != 1){
				kprintf("\nNo such file or directory! (%s)\n",name[i]);
				return 0;
			}
			//kprintf("%d %d\n",ent->nxt_ent_lba,ent->nxt_ent_offset);
			ent = ___parse_ent(ent->nxt_ent_lba,ent->nxt_ent_offset);
		}
//		struct __data_ent *dent = __parse_dent(ent->data_ent_lba,ent->data_ent_offset);
		ret->dent = __parse_dent(ent->data_ent_lba,ent->data_ent_offset);
		//kprintf("{%d}:{%d}\n",ret->dent->data_lba,ret->dent->data_offset);
		ent = ___parse_ent(ret->dent->data_lba,ret->dent->data_offset);
		i++;
	}
//	kprintf("Done\n");
	//kprintf("%d\n",ret->dent->data_lba);
	ret->ent = ent;
//	ret->dent = __parse_dent(ent->data_ent_lba,ent->data_ent_offset);
	return ret;
}
int mkdir(const char *path){
	//kprintf("Parsing superblock\n");
	struct __superblock *sblk = __genfs_parse_superblock();
	//kprintf("%d\n",sblk->root_dir_lba);
//	while(1);
	//kprintf("Allocating %d bytes of memory\n",sizeof(struct __ent)*sizeof(struct __ent *));
	struct __ent *ent= malloc(sizeof(*ent)*sizeof(ent));
	//kprintf("Searching\n");
	struct __data *dat = __find_free_blk();
	//kprintf("%d\n",dat->lba);
	//kprintf("Making dir %s\n",path);
//	uint8_t *buf = malloc(1024);
//	_ata_read_master(buf,sblk->root_dir_lba,0);
	ent->alloc = 1;
	ent->ent_type = 0;
	ent->type = TYPE_DIR;
	char **name = sep(path,'/');
	int last = 0;
	//kprintf("Finding last\n");
	while(name[last] != 0)
		last++;
//	uint8_t *buf = malloc(1024);
//	_ata_read_master(buf,sblk->root_dir_lba,0);
	if(strcmp(path,"/") == 0){
		//kprintf("Making root dir...\n");
		ent->namelen = 1;
		strcpy(ent->name,"/");
		ent->ent_type = 0;
		ent->nxt_ent_lba = 0;
		ent->nxt_ent_offset = 0;
		//ent->data_ent_lba = dat->lba;
		//ent->data_ent_offset = dat->offset;
		//kprintf("Allocating memory\n");
		uint8_t *wrt = malloc(1024);
		//kprintf("Reading\n");
		_ata_read_master(wrt,sblk->root_dir_lba,0);
		memcpy(wrt + sblk->root_dir_offset,ent,sizeof(*ent));
		//kprintf("Writing \n");
		_ata_write_master(wrt,sblk->root_dir_lba);
		dat = __find_free_blk();
		ent->data_ent_lba = dat->lba;
		ent->data_ent_offset = dat->offset;
		memcpy(wrt + sblk->root_dir_offset,ent,sizeof(*ent));
		_ata_write_master(wrt,sblk->root_dir_lba);
		wrt = malloc(1024);
		//kprintf("Reading\n");
		_ata_read_master(wrt,dat->lba,0);
		struct __data_ent *dent = malloc(sizeof(*dent)*sizeof(dent));
		dent->alloc = 1;
		dent->ent_type = 1;
		dent->type = TYPE_DIR;
		memcpy(wrt + ent->data_ent_offset,dent,sizeof(*dent));
		int old = dat->offset;
		int old_lba = dat->lba;
		//kprintf("Writing dent to %d\n",ent->data_ent_lba);
		_ata_write_master(wrt,ent->data_ent_lba);
		dat = __find_free_blk();
		dent->nxt_lba = 0;
		dent->nxt_offset = 0;
		dent->data_lba = dat->lba;
		dent->data_offset = dat->offset;
		memcpy(wrt + old,dent,sizeof(*dent));
		_ata_write_master(wrt,ent->data_ent_lba);
		return 1;
	}
	char *prevname = malloc(1024);
	int i = 0;
	for(int i = 0; i < 1024;i++)
		prevname[i] = 0;
	strcpy(prevname,"/");
	//kprintf("Cat name\n");
	//kprintf("Last: %d\n",last);
	while(i < last - 1){
		strcat(prevname,name[i]);
//		kprintf("%s\n",name[i]);
		strcat(prevname,"/");
		i++;
	}
	//kprintf("Opening dir... %s\n",prevname);
	DIR *d = opendir(prevname);
	if(!d){
		kprintf("Failed to open dir %s!\n",prevname);
		return 0;
	}
	//kprintf("Opened %s\n",prevname);
	struct __ent *_ent = malloc(sizeof(*ent)*sizeof(ent));
	_ent = ___parse_ent(d->dent->data_lba,d->dent->data_offset);
	int prev_lba = d->dent->data_lba,prev_offset = d->dent->data_offset;
	//kprintf("%d %d\n",prev_lba,prev_offset);
	while(_ent->nxt_ent_lba > 0 && _ent->alloc == 1){
		prev_lba = _ent->nxt_ent_lba;
		//kprintf("[%d] %d\n",_ent->alloc,prev_lba);
		prev_offset = _ent->nxt_ent_offset;
		if(strcmp(_ent->name,name[last - 1]) == 0){
			kprintf("File exists!\n");
			return 0;
		}
		_ent = ___parse_ent(_ent->nxt_ent_lba,_ent->nxt_ent_offset);
	}
	struct __data *nd= __find_free_blk();
	_ent->nxt_ent_lba = nd->lba;
	_ent->nxt_ent_offset = nd->offset;
	uint8_t *pntr = malloc(1024);
	_ata_read_master(pntr,prev_lba,0);
	memcpy(pntr + prev_offset,_ent,sizeof(*_ent));
	_ata_write_master(pntr,prev_lba);
	prev_lba = nd->lba;
	prev_offset = nd->offset;
	ent->alloc = 1;
	ent->ent_type = 0;
	ent->type = TYPE_DIR;
	ent->namelen = strlen(name[last - 1]);
	strcpy(ent->name,name[last - 1]);
	ent->nxt_ent_lba = 0;
	ent->nxt_ent_offset = 0;
	ent->data_ent_lba = 0;
	ent->data_ent_offset = 0;
	uint8_t *buf = malloc(1024);
	buf = malloc(1024);
	_ata_read_master(buf,prev_lba,0);
	memcpy(buf + prev_offset,ent,sizeof(*ent));
	_ata_write_master(buf,prev_lba);
	/*struct __data *d3 = __find_free_blk();
	ent->nxt_ent_lba = d3->lba;
	ent->nxt_ent_offset = d3->offset;
	kprintf("!%d!\n",ent->nxt_ent_lba);
	_ata_read_master(buf,prev_lba,0);
	memcpy(buf + prev_offset,ent,sizeof(*ent));
	_ata_write_master(buf,prev_lba);*/
	//kprintf("Writing to lba %d:%d nxt %d:%d\n",prev_lba,prev_offset,ent->nxt_ent_lba,ent->nxt_ent_offset);
//	dat = __find_free_blk();
//	ent->nxt_ent_lba = dat->lba;
//	ent->nxt_ent_offset = dat->offset;
	//memcpy(buf + prev_offset,ent,sizeof(*ent));
	//_ata_write_master(buf,prev_lba);
	struct __data *d2 = __find_free_blk();
	//kprintf("%d %d\n",d2->lba,d2->offset);
	ent->data_ent_lba = d2->lba;
	ent->data_ent_offset = d2->offset;
	//kprintf("[%d]\n",ent->data_ent_lba);
	memcpy(buf + prev_offset,ent,sizeof(*ent));
	_ata_write_master(buf,prev_lba);
//	kprintf("Allocating memory\n");
	struct __data_ent *dent = malloc(sizeof(*dent));
	dent->alloc = 1;
	dent->type = TYPE_DIR;
	dent->ent_type = 1;
	dent->nxt_lba = 0;
	dent->nxt_offset = 0;
	dent->data_lba = 0;
	dent->data_offset = 0;
	//kprintf("Alocating...\n");
	buf = malloc(1024);
	//kprintf("Reading\n");
	_ata_read_master(buf,d2->lba,0);
	//kprintf("Writing to lba %d\n",d2->lba);
	memcpy(buf + d2->offset,dent,sizeof(*dent));
	_ata_write_master(buf,d2->lba);
	struct __data *d4 = __find_free_blk();
//	//kprintf("%d %d\n",d4->lba,d4->offset);
	dent->data_lba = d4->lba;
	dent->data_offset = d4->offset;
	//buf = malloc(1024);
	_ata_read_master(buf,d2->lba,0);
	memcpy(buf + d2->offset,dent,sizeof(*dent));
	//kprintf("Writing %d %d to %d:%d\n",dent->data_lba,dent->data_offset,d2->lba,d2->offset);
	_ata_write_master(buf,d2->lba);
	struct __data *d3 = __find_free_blk();
        ent->nxt_ent_lba = 0;
        ent->nxt_ent_offset = 0;
        //kprintf("!%d!\n",ent->nxt_ent_lba);
        _ata_read_master(buf,prev_lba,0);
        memcpy(buf + prev_offset,ent,sizeof(*ent));
        _ata_write_master(buf,prev_lba);
	debug("MKDIR","done");
	return 1;
}
int __gen_mkdir(const char *path){
	struct __superblock *sblk = __genfs_parse_superblock();
	uint8_t *buf = malloc(1024);
	struct __ent *ent = malloc(sizeof(*ent)*sizeof(ent));
	struct __data *dat = __find_free_blk();
	ent->alloc = 1;
	ent->ent_type = 0;
	ent->type = TYPE_DIR;
	char **name = sep(path,'/');
	int last = 0;
	while(name[last] != 0)
		last++;
	ent->namelen = strlen(name[last]);
	if(strcmp(path,"/") == 0){
		ent->namelen = 1;
		strcpy(ent->name,"/");
	}else
		strcpy(ent->name,name[last - 1]);
	//memcpy(ent->name,name[last],strlen(name[last]));
	ent->nxt_ent_lba = dat->lba;
	ent->nxt_ent_offset = dat->offset;
	//kprintf("%d\n",ent->alloc);
	_ata_read_master(buf,sblk->root_dir_lba,0);
//	kprintf("%d\n",ent->alloc);
	struct __ent *rootent = __parse_ent(buf,sblk->root_dir_offset);
	if(rootent->alloc == 0){
		if(strcmp(path,"/") == 0){
			kprintf("[%d] [%d] [%d] [%d] [%s] [%d] [%d] [%d] [%d]\n",ent->alloc,ent->ent_type,ent->type,ent->namelen,ent->name,ent->nxt_ent_lba,ent->nxt_ent_offset,ent->data_ent_lba,ent->data_ent_offset);
			kprintf("Writing root dir to lba %d:%d\n",sblk->root_dir_lba,sblk->root_dir_offset);
			uint8_t *wrt = malloc(512);
			_ata_read_master(wrt,sblk->root_dir_lba,0);
			memcpy(wrt + sblk->root_dir_offset,ent,sizeof(*ent));
			_ata_write_master(wrt,sblk->root_dir_lba);
			return 1;
		}
	}else if(rootent->alloc == 0){
		kprintf("Bad file system\n");
		panic();
	}else if(strcmp(path,"/") == 0){
		kprintf("File exists!\n");
		return -1;
	}
	int i = 0;
	int lba = rootent->nxt_ent_lba;
	int offset = rootent->nxt_ent_offset;
	/*if(lba == 0 && last == 1){
		struct __data *dat = __find_free_blk();
		kprintf("[%d] [%d]\n",dat->lba,dat->offset);
		uint8_t *buf = malloc(512);
		_ata_read_master(buf,dat->lba,0);
		memcpy(buf + dat->offset,ent,sizeof(*ent));
		_ata_write_master(buf,dat->lba);
	}else{
		kprintf("No such file or directory\n",last);
		return -1;
	}*/
	i = 0;
	int prev_lba,prev_offset;
	struct __ent *current = ___parse_ent(lba,offset);
	while(i < last - 1){
		while(strcmp(current->name,name[i]) != 0){
			if(current->nxt_ent_lba == 0 || current->alloc == 0){
				kprintf("No such file or directory (%s)\n",name[i]);
				return -1;
			}
			current = ___parse_ent(current->nxt_ent_lba,current->nxt_ent_offset);
		}
		struct __data_ent *dent = __parse_dent(current->data_ent_lba,current->data_ent_offset);
		current = ___parse_ent(dent->data_lba,dent->data_offset);
		i++;
	}
	struct __ent *nent = ___parse_ent(current->nxt_ent_lba,current->nxt_ent_offset);
	while(nent->alloc == 1){
		if(strcmp(nent->name,name[last - 1]) == 0){
			kprintf("File exists!\n");
			return -1;
		}
		kprintf("%s\n",nent->name);
		nent = ___parse_ent(nent->nxt_ent_lba,nent->nxt_ent_offset);
	}
	if(strcmp(current->name,name[last - 1]) == 0){
		kprintf("File exists!\n");
		return -1;
	}
	uint8_t *prev = malloc(1024);
	_ata_read_master(prev,dat->lba,0);
	if(dat->lba == 0){
		kprintf("Zero error\n");
		panic();
	}
	memcpy(prev + dat->offset,ent,sizeof(*ent));
	_ata_write_master(prev,dat->lba);
	return 1;
}
int list(const char *path){
	kprintf("LIST %s:\n",path);
	int last = 0;
	DIR *d = opendir(path);
	if(!d){
		kprintf("Failed to open %s\n",path);
		return 0;
	}
	//struct __ent *e = d->ent;
	struct __ent *e = malloc(sizeof(*e));
	//kprintf("%d %d\n",d->dent->data_lba,d->dent->data_offset);
	e = ___parse_ent(d->dent->data_lba,d->dent->data_offset);
//	kprintf("%d %d\n",d->dent->data_lba,d->dent->data_offset);
	while(e->alloc == 1){
		kprintf("%s ",e->name);
	//	kprintf("%d %d\n",e->nxt_ent_lba,e->nxt_ent_offset);
		e = ___parse_ent(e->nxt_ent_lba,e->nxt_ent_offset);
	}
	kprintf("\n");
	return 1;
}
