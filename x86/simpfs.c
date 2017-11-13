/*
*simple FS Driver for zBL
*(c) 2017 Zachary James Schlotman
*/
#include "lib.h"
#include "mem.h"
#include "simpfs.h"
#include <stdint.h>
/*
*Finds free block and returns the lba of that block
*/
uint32_t find_free(){
	uint32_t ret = 1;
	uint8_t *buf = malloc(1024);
	_ata_read_master(buf,ret,0);
	while(buf[0]){
		ret++;
		_ata_read_master(buf,ret,0);
	}
	return ret;
}
/*
*Opens a file returns a file descriptor
*ONLY SUPPORTS READING AS OF NOW
*/
int open(const char *fname,int options){
	struct fd *tbl = (struct fd *)0x0A000000;
	int i = 0;
	while(tbl->alloc){
		i++;
		tbl+=sizeof(*tbl);
	}
	if(!(options >> 1 & 1)){
		struct fd *n = getInfo(fname);
		if(!n)
			return -1;
		n->type = options;
		memcpy(tbl,n,sizeof(*tbl));
		
		return i;
	}
}
/*
*Gets file descriptor pointer from fd
*/
struct fd *getFd(int n){
	struct fd *tbl = (struct fd *)0x0A000000;
	int i = 0;
	while(i < n){
		i++;
		tbl+=sizeof(*tbl);
	}
	return tbl;
}
/*
*Gets info from disk to store in a pointer that can be accessed through a file descriptor
*/
struct fd *getInfo(const char *str){
	char **s = sep(str,'/');
	char *dpath = malloc(strlen(str));
	int i = 0;
	strcat(dpath,"/");
	while(s[i + 1] != 0){
		strcat(dpath,s[i]);
		strcat(dpath,"/");
	}
	DIR *d = opendir(dpath);
	if(!d){
		kprintf("Error opening dir");
		return -1;
	}
	struct tree_ent *ent = malloc(512);
	uint8_t *buf = malloc(1024);
	struct tree_dirhdr *dhdr = malloc(sizeof(*dhdr));
	_ata_read_master(buf,d->dhdr->nxtTreeLba,0);
	memcpy(ent,buf + 5,sizeof(*ent));
	int prevLba = d->dhdr->nxtTreeLba;
	while(ent->alloc){
		memcpy(dhdr,ent + sizeof(*ent),sizeof(*dhdr));
		if(ent->type == __TYPE_FILE && strcmp(d->fhdr->name,s[i]) == 0){
			struct fd *ret = malloc(sizeof(*ret));
			ret->alloc = 1;
			ret->tLba = prevLba;
			ret->pos = 0;
			memcpy(ret->name,str,strlen(str));
			return ret;
		}
		prevLba = ent->nxtLba;
		_ata_read_master(ent,ent->nxtLba,0);
	}
	return 0;
}
/*
*Returns 1 if the file system is present on the disk and 0 when it is not
*/
int isSimpfs(){
	uint8_t * buf = malloc(512);
	_ata_read_master(buf,0,0);
	return buf[0] == 0x7f && buf[1] == 'S' && buf[2] == 'I' && buf[3] == 'M' && buf[4] == 'P';
}
/*
*Creates the file system
*/
int mkfs(){
	uint8_t *buf = malloc(512);
	buf[0] = 0x7f;
	buf[1] = 'S';
	buf[2] = 'I';
	buf[3] = 'M';
	buf[4] = 'P';
	struct tree_ent *head = malloc(512);
	head->alloc = 1;
	head->type = __TYPE_DIR;
	head->nxtLba = 1;
	memcpy(buf + 5,head,sizeof(*head));
	_ata_write_master(buf,0);
	mkdir("/");
}
/*
*Creates dir by the name of name
*returns 0 on failure and 1 on success
*/
int mkdir(const char *name){
	uint8_t **s = sep(name,'/');
	int i = 0;
	uint8_t *buf = malloc(512);
	struct tree_ent *ent = malloc(512);
	_ata_read_master(buf,0,0);
	memcpy(ent,buf + 5,512-5);
	int prevLba = ent->nxtLba;
	struct tree_dirhdr *dhdr = malloc(512);
	while(s[i+1] != 0){
		_ata_read_master(buf,ent->nxtLba,0);
		memcpy(ent,buf,sizeof(*ent));
		while(ent->alloc){
			if(ent->type == __TYPE_DIR){
				struct tree_filehdr *fhdr = malloc(sizeof(*fhdr));
				memcpy(fhdr,buf + sizeof(*ent),sizeof(*fhdr));
				memcpy(dhdr,buf + sizeof(*ent) + sizeof(*fhdr),sizeof(*dhdr));
				if(strcmp(fhdr->name,s[i]) == 0){
					prevLba = ent->nxtLba;
					_ata_read_master(buf,dhdr->nxtTreeLba,0);
					break;
				}
			}
			_ata_read_master(buf,ent->nxtLba,0);
			memcpy(ent,buf,sizeof(*ent));
		}
		if(!ent->alloc)
			return 0;
		i++;
	}
	struct tree_filehdr *tfhdr = ent + sizeof(*ent);
	if(strcmp(tfhdr->name,s[i]) == 0 && strlen(s[i]) > 0){
		kprintf("Error: Directory exists!\n");
		return 0;
	}
	while(ent->nxtLba != 0){
		prevLba = ent->nxtLba;
		struct tree_filehdr *fhdr = buf + sizeof(*ent);
		if(strcmp(fhdr->name,s[i]) == 0 && strlen(s[i]) > 0){
			kprintf("Error: Directory exists!\n");
			return 0;
		}
		_ata_read_master(buf,ent->nxtLba,0);
		memcpy(ent,buf,sizeof(*ent));
	}
	ent->nxtLba = find_free();
	_ata_write_master(ent,prevLba);
	free(ent);
	ent = malloc(sizeof(*ent));
	ent->alloc = 1;
	ent->type = __TYPE_DIR;
	ent->nxtLba = 0;
	free(dhdr);
	dhdr = malloc(sizeof(*dhdr));
	struct tree_filehdr *fhdr = malloc(1024);
	fhdr->alloc = 1;
	if(strcmp(name,"/") != 0){
		fhdr->namelen = strlen(s[i]);
		memcpy(fhdr->name,s[i],fhdr->namelen);
	}else{
		fhdr->namelen = strlen(name);

		memcpy(fhdr->name,name,strlen(name));
	}
	dhdr->nxtTreeLba = 0;
	buf = malloc(512);
	uint32_t lba = find_free();
	memcpy(buf,ent,sizeof(*ent));
	_ata_write_master(buf,find_free());
	ent->nxtLba = find_free();
	memcpy(buf,ent,sizeof(*ent));
	memcpy(buf + sizeof(*ent),fhdr,sizeof(*fhdr));
	memcpy(buf + sizeof(*ent) + sizeof(*fhdr),dhdr,sizeof(*dhdr));
	_ata_write_master(buf,lba);
	dhdr->nxtTreeLba = find_free();
	memcpy(buf + sizeof(*ent) + sizeof(*fhdr),dhdr,sizeof(*dhdr));
	_ata_write_master(buf,lba);

	return 1;
}
/*
*Returns DIR pointer that holds all the info currently needed for I/O on directories or 0 on error
*/
DIR *opendir(const char *name){
	char **s = sep(name,'/');
	struct tree_ent *ent = malloc(512);
	uint8_t *buf = malloc(1024);
	_ata_read_master(buf,0,0);
	memcpy(ent,buf + 5,sizeof(*ent));
	struct tree_dirhdr *dhdr = malloc(512);
       	int i = 0;
	int prevlba = 0;
	struct tree_filehdr *fhdr = malloc(sizeof(*fhdr));
	while(s[i] != 0){
                _ata_read_master(buf,ent->nxtLba,0);
               	memcpy(ent,buf,sizeof(*ent));
		while(ent->alloc){
                        if(ent->type == __TYPE_DIR){
                                memcpy(fhdr,buf + sizeof(*ent),sizeof(*fhdr));
				memcpy(dhdr,buf + sizeof(*ent) + sizeof(*fhdr),sizeof(*dhdr));
                                if(strcmp(fhdr->name,s[i]) == 0){
                                        prevlba = ent->nxtLba;
                                        _ata_read_master(buf,dhdr->nxtTreeLba,0);
                                        break;
                                }
                        }
                        _ata_read_master(buf,ent->nxtLba,0);
			memcpy(ent,buf,sizeof(*ent)); 
               }
                if(!ent->alloc)
                        return 0;
                i++;
        }
	DIR *ret = malloc(sizeof(*ret));
	*ret->ent = *ent;
	*ret->fhdr = *fhdr;
	*ret->dhdr = *dhdr;
	return ret;
}
/*
*Returns the size of the file
*/
int fsize(const char *name){

}
/*
*Reads from n bytes from fd into buf
*Returns bytes read
*/
int read(int fd,void *pntr,int n){
	struct fd *f = getFd(fd);
	if(f->type & 1){
		char **arr = sep(f->name,'/');
		char *path = malloc(1024);
		int i = 0;
		strcpy(path,"/");
		while(arr[i+1] != 0){
			strcat(path,arr[i]);
			strcat(path,"/");
			i++;
		}
		DIR *d = opendir(path);
		if(!d)
			return 0;
		uint32_t lba = d->dhdr->nxtTreeLba;
		struct tree_ent *ent = malloc(512);
		_ata_read_master(ent,lba,0);
		while(ent->alloc){
			if(ent->type == __TYPE_FILE){
				struct tree_filehdr *fhdr = malloc(sizeof(*fhdr));
				memcpy(fhdr,ent + sizeof(*ent),sizeof(*fhdr));
				if(strcmp(fhdr->name,arr[i]) == 0){
					break;
				}
			}
			lba = ent->nxtLba;
			_ata_read_master(ent,ent->nxtLba,0);
		}
		if(!ent->alloc)
			return 0;
		i = 0;
		uint8_t *buf = malloc(512);
		lba = ent->nxtLba;
		uint32_t offset = 0;
		/*
		*This loop makes it so that we start at the correct lba and offset
		*by looping through until we meet or exceed the offset set by the
		*file descriptor
		*/
		while(i < f->pos){
			_ata_read_master(buf,lba,0);
			struct tree_ent *tent = malloc(sizeof(*tent));
			memcpy(tent,buf,sizeof(*tent));
			lba = tent->nxtLba;
			i+=512-sizeof(*ent);
			if(i > f->pos){
				memcpy(pntr,buf + 512-(i-f->pos),i-f->pos);
				offset+=512-(i-f->pos);
			}
		}
		/*
		*Reads the data from the file into the pointer
		*/
		i = 0;
		while(i < n){
			_ata_read_master(buf,lba,0);
			struct tree_ent *tent = malloc(sizeof(*tent));
			memcpy(tent,buf,sizeof(*tent));
			lba = tent->nxtLba;
			
			if(offset+512-sizeof(*ent) > n)
				memcpy(pntr + offset,buf + sizeof(*tent),n-offset);
			else
				memcpy(pntr + offset,buf + sizeof(*tent),512-sizeof(*tent));
			offset+=512-sizeof(*ent);
		}	
		return offset;
	}else
		return 0;
}
int write(int fd,void *buf,int flags){
	struct fd *f = getFd(fd);
	if(f->type >> 1 & 1){
		char **s = sep(f->name,'/');
		char *path = malloc(1024);
		int i = 0;
		strcat(path,"/");
		while(s[i + 1] != 0){
			strcat(path,s[i]);
			strcat(path,"/");
			i++;
		}
		DIR *d = opendir(path);
		if(!d)
			return -1;
	}
}
int close(int fd){

}
int __exec(const char *path){

}
int exec(const char *path,const char **argv){

}
int lseek(int fd,int pos, int mode){

}
void list(const char *path){

}
