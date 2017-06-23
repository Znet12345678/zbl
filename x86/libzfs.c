
#include "lib.h"
#include "mem.h"
#include "zfs.h"
#define ERR -1
#ifdef __BUILD_ZFS
const uint8_t sig[5] = {0x7F,'z','f','s',0};
struct dirent *parse_dent(int lba,int offset){
	struct dirent *ret = malloc(1024);
	uint8_t *buf = malloc(1024);
	ata_read_master(buf,lba,0);
	if((offset + sizeof(*ret) + buf[offset + 1]) > 512)
		ata_read_master(&buf[512],lba + 1,0);
	ret->alloc = buf[offset];
	ret->size = buf[offset + 1];
	ret->namelen = buf[offset + 2];
	for(int i = 0; i < ret->namelen;i++){
		ret->name[i] = buf[offset + 3 + i];
	}
	for(int i = 24; i >= 0;i-=8)
		ret->first_fent_lba|=buf[offset + ret->namelen + 4 + (24 - i)/8] << i;
	for(int i = 8; i >= 0;i-=8)
		ret->first_fent_offset|=buf[offset + 8 + ret->namelen + (8-i)/8] << i;
	for(int i = 24; i >= 0;i-=8)
		ret->nxt_dirent_lba |= buf[offset + 10 + ret->namelen + (24 - i)/8] << i;
	for(int i = 8; i >= 0;i-=8)
		ret->nxt_dirent_offset |= buf[offset + 14 + ret->namelen + (8 - i)/8] << i;
	for(int i = 24; i >= 0;i-=8)
		ret->curr_ent_lba |= buf[offset + 16 + ret->namelen + (24 - i)/8] << i;
	for(int i = 8; i >= 0;i-=8)
		ret->curr_ent_offset|= buf[offset + 20 + ret->namelen + (8 - i)/8] << i;
	return ret;
}
struct dirent *parse_dirent(const char *str){
	if(strcmp(str,"/") == 0){
		struct dirent *ret = malloc(1024);
		uint8_t *buf = malloc(512);
		struct superblock *sblk = parse_superblk(*(int*)0x501);
		ata_read_master(buf,sblk->root_dirent_lba,0);
		int offset = sblk->root_dirent_offset;
		ret->alloc = buf[offset];
		ret->size = buf[offset + 1];
		ret->namelen = buf[offset + 2];
		for(int i = 0; i < ret->namelen;i++)
			ret->name[i] = buf[offset + 3 + i];
		ret->first_fent_lba = buf[offset + 4 + ret->namelen] << 24 | buf[offset + 5 + ret->namelen] << 16 | buf[offset + 6 + ret->namelen] << 8 | buf[offset + 7 + ret->namelen];
		ret->first_fent_offset = buf[offset + 8 + ret->namelen] << 8 | buf[offset + 9 + ret->namelen];
		ret->nxt_dirent_lba = buf[offset + 10 + ret->namelen] << 24 | buf[offset + 11 + ret->namelen] << 16 | buf[offset + 12 + ret->namelen] << 8 | buf[offset + 13 + ret->namelen];
		ret->nxt_dirent_offset = buf[offset + 14 + ret->namelen] << 8 | buf[offset + 15 + ret->namelen];
		ret->curr_ent_lba = buf[offset + 16 + ret->namelen] << 24 | buf[offset + 17 + ret->namelen] << 16 | buf[offset + 18 + ret->namelen] << 8 | buf[offset + 19 + ret->namelen];
		ret->curr_ent_offset = buf[offset + 20 + ret->namelen] << 8 | buf[offset + 21 + ret->namelen];
		return ret;
	}else{
		struct dirent *dent = parse_dirent("/");
		while(dent->nxt_dirent_lba > 0){
			if(strcmp(dent->name,str) == 0)
				return dent;
			uint8_t *buf = malloc(1024);
			ata_read_master(buf,dent->nxt_dirent_lba,0);
			dent = parse_dent(dent->nxt_dirent_lba,dent->nxt_dirent_offset);
		}
		return (struct dirent *)-1;
	}
}
struct free_blk *find_freedent(int size){
	struct dirent *dent = parse_dirent("/");
	uint32_t currlba = 0;
	uint16_t curroffset = 0;
	if(dent->nxt_dirent_lba == 0){
		struct superblock *sblk = parse_superblk(*(int*)0x501);
		struct dirent *root = parse_dirent("/");
		struct free_blk *ret = malloc(sizeof(*ret) * sizeof(ret));
		ret->offset = sizeof(*root);
		ret->lba = sblk->root_dirent_lba;
		return ret;
	}
	struct superblock *sblk = parse_superblk(*(int*)0x501);
	currlba = sblk->root_dirent_lba;
	curroffset = sblk->root_dirent_offset + sizeof(*dent);
	int currsize = sizeof(*dent);
	while(dent->nxt_dirent_lba > 0){
		//kprintf("%d\n",currlba);
		if(dent->alloc == 0 || dent->nxt_dirent_lba == 0){
			if(dent->alloc == 0){
				if(size > (dent->nxt_dirent_offset - curroffset)){
					uint8_t *buf = malloc(1024);
					ata_read_master(buf,dent->nxt_dirent_lba,0);
					currlba = dent->nxt_dirent_lba;
					curroffset = dent->nxt_dirent_offset;
					dent = parse_dent(dent->nxt_dirent_lba,dent->nxt_dirent_offset);
					free(buf);
					continue;
				}else if(((512 - curroffset) + dent->nxt_dirent_offset) < size){
					uint8_t *buf = malloc(1024);
                                        ata_read_master(buf,dent->nxt_dirent_lba,0);
                                        currlba = dent->nxt_dirent_lba;
                                        curroffset = dent->nxt_dirent_offset;
                                        dent = parse_dent(dent->nxt_dirent_lba,dent->nxt_dirent_offset);
					free(buf);
                                        continue;
				}
				struct free_blk *ret = malloc(sizeof(*ret)*sizeof(ret));
        	                if((curroffset + size) > 512)
                        	        ret->lba = currlba + 1;
                	        else
                                	ret->lba = currlba;
                        	ret->offset = curroffset;
                        	return ret;
			}
			struct free_blk *ret = malloc(sizeof(*ret)*sizeof(ret));
			if(currsize + curroffset > 512){
				ret->lba = currlba + 1;
				ret->offset = 0;
				uint8_t *buf = malloc(1024);
				ata_read_master(buf,ret->lba,0);
				while(buf[0] != 0){
					ret->lba++;
					ata_read_master(buf,ret->lba,0);
				}
			}else{
				ret->lba = currlba;
				ret->offset = currsize;
			}

			return ret;
		}
		uint8_t *buf = malloc(1024);
		ata_read_master(buf,dent->nxt_dirent_lba,0);
		currlba = dent->nxt_dirent_lba;
		curroffset = dent->nxt_dirent_offset;
		currsize = sizeof(*dent);
		dent = parse_dent(dent->nxt_dirent_lba,dent->nxt_dirent_offset);
		free(buf);
	}
	struct free_blk *ret = malloc(sizeof(*ret)*sizeof(ret));
        if(currsize + curroffset > 512){
        	ret->lba = currlba + 1;
                ret->offset = 0;
		uint8_t *buf = malloc(512);
		ata_read_master(buf,ret->lba,0);
		while(buf[0] != 0){
			ret->lba++;
			ata_read_master(buf,ret->lba,0);
		}
        }else{
            	ret->lba = currlba;
                ret->offset = currsize + curroffset;
        }

	return ret;
}
char *part(const char *str,unsigned int n,unsigned char c){
	char *ret = malloc(1024);
	int j = 0;
	int k = 0;
	for(int i = 0; i < strlen(str);i++){
		if(str[i] == c)
			j++;
		else if(j == n){
			ret[k] = str[i];
			k++;
		}
	}
	return ret;
}
int is_zfs(){
	struct superblock *sblk = parse_superblk(*(int*)0x501);
	if(strcmp(sblk->sig,sig) == 0)
		return 1;
	debug("IS_ZFS","Invalid sig not zfs");
	kprintf("Correct sig:%s\nsig:%s\n",sig,sblk->sig);
	return ERR;
}
int write_dirent(struct dirent *ent,int lba,int offset){
//	kprintf("Writing:%s:%d\n",ent->name,ent->namelen);
//	kprintf("WRITE TO LBA:OFFSET %d:%d\n",lba,offset);
	uint8_t *buf = malloc(1024);
	int err = ata_read_master(buf,lba,0);
	uint8_t *buf2 = malloc(512);
	if(err < 0){
		kprintf("Initial read of lba %d failed\n",lba);
		debug("WRITE_DIRENT","Error 1");
		return -1;
	}
	if((offset + sizeof(*ent)) > 512){
		debug("WRITE_DIRENT","Read 2 blocks");
		err = ata_read_master(buf2,lba + 1,0);
		if(err < 0){
			debug("WRITE_DIRENT","Error 2");
			return -1;
		}
		for(int i = 0; i < 513;i++)
			buf[513 + i] = buf2[i];
	}
	buf[offset] = ent->alloc;
	buf[offset + 1] = ent->size;
	buf[offset + 2] = ent->namelen;
	for(int i = 0; i < ent->namelen;i++)
		buf[offset + 3 + i] = ent->name[i];
	for(int i = 24; i >= 0;i-=8)
		buf[offset + (24 - i)/8 + ent->namelen + 4] = ent->first_fent_lba >> i;
	for(int i = 8; i >= 0;i-=8)
		buf[offset + (8 - i)/8 + ent->namelen + 9] = ent->first_fent_offset >> i;
	for(int i = 24; i >= 0;i-=8)
		buf[offset + (24 - i)/8 + ent->namelen + 10] = ent->nxt_dirent_lba >> i;
	for(int i = 8; i >= 0;i-=8)
		buf[offset + (8 - i)/8 + ent->namelen + 14] = ent->nxt_dirent_offset >> i;
	for(int i = 24; i >= 0;i-=8)
		buf[offset + (24 - i)/8 + ent->namelen + 16] = ent->curr_ent_lba >> i;
	for(int i = 8; i >= 0;i-=8)
		buf[offset + (24 - i)/8 + ent->namelen + 20] = ent->curr_ent_offset >> i;
	if((offset + sizeof(*ent)) > 512){
		debug("WRITE_DIRENT","Writing 2 blocks");
		err = ata_write_master(buf,lba);
		if(err < 0){
			debug("WRITE_DIRENT","Error 3");
			return -1;
		}
		uint8_t *tmp = malloc(512);
		for(int i = 0; i < 512;i++)
			tmp[i] = buf[512 + i];
		err = ata_write_master(tmp,lba + 1);
		if(err < 0){
			debug("WRITE_DIRENT","Error 4");
			return -1;
		}
	}else{
		err = ata_write_master(buf,lba);
		if(err < 0){
			debug("WRITE_DIRENT","Error 5");
			return -1;
		}
	}
	free(buf);
	free(buf2);
	return 1;
}
int __broken_mkdir(const char *path){
	struct superblock *sblk = parse_superblk(*(int*)0x501);
	if(strcmp(path,"/") == 0){
		uint32_t lba = sblk->root_dirent_lba;
		kprintf("[MKDIR]Writing root directory to lba %d\n",lba);
		struct dirent *dent = malloc(512);
		dent->alloc = 1;
		dent->namelen = strlen(path);
		strcpy(dent->name,path);
		dent->first_fent_lba = 0;
		dent->first_fent_offset = 0;
		dent->nxt_dirent_lba = 0;
		dent->nxt_dirent_offset = 0;
		dent->curr_ent_lba = lba + 1;
		dent->curr_ent_offset = 0;
		uint8_t *buf = malloc(512);
		buf[0] = dent->alloc;
		buf[1] = dent->namelen;
		int i = 0;
		kprintf("[MKDIR]Writing %s to %d\n",dent->name,lba);
		int namelen = dent->namelen;
		while(i < dent->namelen){
			buf[2 + i] = dent->name[i];
			i++;
		}
		i = 0;
		for(int j = 24; j >= 0;j-=8,i++)
			buf[3 + namelen + i] = dent->first_fent_lba >> j;
		buf[7 + namelen] = dent->first_fent_offset >> 8;
		buf[8 + namelen] = dent->first_fent_offset;
		i = 0;
		for(int j = 24; j >= 0;j-=8,i++)
			buf[9 + namelen + i] = dent->nxt_dirent_lba >> j;
		buf[13 + namelen] = dent->nxt_dirent_offset >> 8;
		buf[14 + namelen] = dent->nxt_dirent_offset;
		i = 0;
		for(int j = 24; j >= 0;j-=8,i++)
			buf[15 + namelen + i] = dent->curr_ent_lba >> j;
		buf[19 + namelen] = dent->curr_ent_offset >> 8;
		buf[20 + namelen] = dent->curr_ent_offset;
		debug("MKDIR","Writing...");
		ata_write_master(buf,lba);
		debug("MKDIR","Writing ENT");
		struct ent *ent = malloc(512);
		ent->alloc = 1;
		ent->nxt_ent_lba = 0;
		ent->nxt_ent_offset = 0;
		ent->nxt_dirent_lba = 0;
		ent->nxt_dirent_offset = 0;
		debug("MKDIR","Freeing buf");
		free(buf);
		debug("MKDIR","Allocating buf");
		buf = malloc(1024);
		for(i = 0; i < 512;i++)
			buf[i] = 0;
		buf[0] = ent->alloc;
		ata_write_master(buf,dent->curr_ent_lba);
		debug("MKDIR","Done");
		return 1;
	}else{
		struct dirent *dent = parse_dirent("/");
		int lba = sblk->root_dirent_lba;
		int offset = sblk->root_dirent_offset;
		int write_lba = 0;
		int write_offset = 0;
//		kprintf("Reading: %d:%d\n",lba,offset);
		while(dent->nxt_dirent_lba != 0){
			/*uint8_t *buf = malloc(1024);
			int err = ata_read_master(buf,lba,0);
			if(err < 0){
				debug("MKDIR","failed to read");
				kprintf("%d %d\n",lba,offset);
				panic();
			}
			int _lba = buf[offset + 9 + buf[offset  + 1]] << 24 | buf[offset + 10 + buf[offset + 1]] << 16 | buf[offset + 11 + buf[offset + 1]] << 8 | buf[offset + 12 + buf[offset + 1]];
			int _offset = buf[offset + buf[offset + 1] + 13] << 8 | buf[offset + 14 + buf[offset + 1]];
			if(_lba == 0)
				break;
			lba = _lba;
			offset = _offset;
			free(buf);*/
			lba = dent->nxt_dirent_lba;
			offset = dent->nxt_dirent_offset;
			dent = parse_dent(dent->nxt_dirent_lba,dent->nxt_dirent_offset);
			if(strcmp(dent->name,path) == 0){
				kprintf("Error: Dir exists\n");
				return -1;
			}
		}
		char *name = malloc(80);
		uint8_t *nbuf = malloc(512);
		ata_read_master(nbuf,lba,0);
		int namelen = nbuf[offset + 1];
		for(int i = 0; i < namelen;i++)
			name[i] = nbuf[2 + i];
		struct dirent *prev = dent;
		if(prev == (struct dirent *)-1)
			panic();
		/*while(prev->nxt_dirent_lba != 0){
			if(strcmp(prev->name,path) == 0){
				kprintf("Error: Directory exists!\n");
				return -1;
			}
			if(prev == (struct dirent *)-1)
				panic();
			lba = prev->nxt_dirent_lba;
			offset = prev->nxt_dirent_offset;
			prev = parse_dent(prev->nxt_dirent_lba,prev->nxt_dirent_offset);
		}*/

		uint8_t *buf = malloc(1024);
		struct dirent *dent1 = malloc(sizeof(*dent1) * sizeof(dent1));
		dent1->alloc = 1;
		dent1->namelen = strlen(path);
		strcpy(dent1->name,path);
		dent1->first_fent_lba = 0;
		dent1->first_fent_offset = 0;
		dent1->nxt_dirent_lba = 0;
		dent1->nxt_dirent_offset = 0;
		dent1->curr_ent_lba = 0;
		dent1->curr_ent_offset = 0;
		struct free_blk *blk = find_freedent(sizeof*prev);
		if(blk == (struct free_blk*)-1)
			panic();
		prev->nxt_dirent_lba = blk->lba;
		prev->nxt_dirent_offset = blk->offset;
//		kprintf("%d %d\n",prev->nxt_dirent_lba,prev->nxt_dirent_offset);
		/*if((offset + sizeof(*prev)) > 512){
			prev->nxt_dirent_lba = lba + 1;
			prev->nxt_dirent_offset = 0;
		}else{
			prev->nxt_dirent_lba = lba;
			prev->nxt_dirent_offset +=sizeof(*prev);
		}*/
		debug("MKDIR","Writing");
		int err  = write_dirent(prev,lba,offset);
		if(err < 0){
			debug("MKDIR","Failed to write");
			panic();
		}
//		kprintf("%d:%d\n",blk->lba,blk->offset);
		err = write_dirent(dent1,blk->lba,blk->offset);
		if(err < 0){
			debug("MKDIR","Failed to write");
			panic();
		}
	}
	return ERR;
}
int isRootDir(const char *path){
	int i = 0;
	if(path[0] != '/')
		return -1;
	while(path[i] == '/')
		i++;
	while(path[i] != 0 && path[i] != '/')
		i++;
	if(path[i] == 0)
		return 1;
	while(path[i] != 0 && path[i] != '/')
		i++;
	if(path[i] == 0)
		return 1;
	return 0;
}
struct ent *_parse_ent(int lba,int offset){
	uint8_t * buf = malloc(1024);
	struct ent *ret = malloc(sizeof(*ret) * sizeof(ret));
	ata_read_master(buf,lba,0);
	ret->alloc = buf[offset];
	ret->size = buf[offset + 1];
	for(int i = 24;i >= 0;i-=8)
                ret->nxt_ent_lba |= buf[offset + 2 + (24 - i)/8] << i;
        for(int i = 8; i>=0;i-=8)
                ret->nxt_ent_offset|=buf[offset + 6 + (8 - i)/8] << i;
        for(int i = 24; i >= 0;i-=8)
                ret->nxt_dirent_lba|=buf[offset + 8 + (24 - i)/8] << i;
        for(int i = 8; i >= 0;i-=8)
                ret->nxt_dirent_offset|=buf[offset + 12 + (24 - i)/8] << i;
        return ret;
}
struct ent* parse_ent(struct dirent *dent){
	struct ent *ret = malloc(sizeof(*ret)*sizeof(ret));
	uint8_t *buf = malloc(1025);
	ata_read_master(buf,dent->curr_ent_lba,0);
	uint8_t *extr = malloc(512);
	ata_read_master(extr,dent->curr_ent_lba + 1,0);
	for(int i = 0; i < 512;i++)
		buf[i + 513] = extr[i];
	int offset = dent->curr_ent_offset;
	ret->alloc = buf[offset];
	ret->size = buf[offset + 1];
	for(int i = 24;i >= 0;i-=8)
		ret->nxt_ent_lba |= buf[offset + 2 + (24 - i)/8] << i;
	for(int i = 8; i>=0;i-=8)
		ret->nxt_ent_offset|=buf[offset + 6 + (8 - i)/8] << i;
	for(int i = 24; i >= 0;i-=8)
		ret->nxt_dirent_lba|=buf[offset + 8 + (24 - i)/8] << i;
	for(int i = 8; i >= 0;i-=8)
		ret->nxt_dirent_offset|=buf[offset + 12 + (24 - i)/8] << i;
	return ret;
}
struct free_blk *find_free_ent(int root_lba,int root_offset,int size){
	int i = 0;
	int lba = 0;
	int offset = 0;
	struct dirent *dent = parse_dent(root_lba,root_offset);
	while(dent->alloc == 1 && dent->nxt_dirent_lba > 0){
		lba = dent->nxt_dirent_lba;
		offset = dent->nxt_dirent_lba;
		dent = parse_dent(lba,offset);
	}
	struct free_blk *ret = malloc(sizeof(*ret)*sizeof(ret));
	ret->lba = lba;
	ret->offset = offset + size;
	if(ret->offset > 512){
		ret->lba++;
		ret->offset = 0;
	}
	return ret;
}
int write_ent(struct ent *ent,int lba, int offset){

}
#endif

char **sep(const char *str,int c){
	int i = 0,k = 0,l = 0;
	char **ret = (char**)malloc(102400);
	while(str[i] != 0){
		while(str[i] == c)
			i++;
		ret[l] = malloc(1024);
		while(str[i] != c && str[i] != 0){
			ret[l][k] = str[i];
			k++;
			i++;
		}
		l++;
		k = 0;
	}
	ret[l] = (char*)0;
	return ret;
}
#ifdef __BUILD_ZFS
struct free_blk *_find_free(int size){
	struct superblock *sblk = parse_superblk(*(int*)0x501);
	struct dirent *dent = parse_dent(sblk->root_dirent_lba,sblk->root_dirent_offset);
	struct ent *ent = parse_ent(dent);
	uint8_t *buf = malloc(512);
	ata_read_master(buf,ent->nxt_dirent_lba,0);
	int offset = ent->nxt_dirent_offset;
	int lba = ent->nxt_dirent_lba;
	while(1){
		if(buf[offset] == 0){
			if(buf[offset + 1] >= size || buf[offset + 1] == 0){
				struct free_blk *ret = malloc(sizeof(*ret)*sizeof(ret));
				ret->lba = lba;
				ret->offset = offset;
			}
		}
		offset+=buf[offset + 1];
		if(offset > 512){
			offset = 0;
			lba++;
			ata_read_master(buf,lba,0);
		}
	}
}
int zfs_mkdir(const char *path){
	debug("MKDIR","start");
	if(strcmp(path,"/") == 0){
		struct superblock *sblk = parse_superblk(*(int*)0x501);
		struct dirent *dent = malloc(sizeof(*dent) * sizeof(dent));
		struct ent *ent = malloc(sizeof(*ent) * sizeof(ent));
		dent->alloc = 1;
		dent->size = sizeof(*dent);
		dent->namelen = 1;
		strcpy(dent->name,"/");
		dent->first_fent_lba = 0;
		dent->first_fent_offset = 0;
		dent->nxt_dirent_lba = 0;
		dent->nxt_dirent_offset = 0;
		dent->curr_ent_lba = 80;
		dent->curr_ent_offset = 0;
		write_dirent(dent,sblk->root_dirent_lba,sblk->root_dirent_offset);
		ent->alloc = 1;
		ent->size = sizeof(*ent);
		ent->nxt_ent_lba = 0;
		ent->nxt_ent_offset = 0;
		struct free_blk *fblk = _find_free(sizeof(struct dirent));
		ent->nxt_dirent_lba = fblk->lba;
		ent->nxt_dirent_offset = fblk->offset;
		write_ent(ent,80,0);
	}
	char **ssep = sep(path,'/');
	struct superblock *sblk = parse_superblk(*(int*)0x501);
	struct dirent *dent = parse_dent(sblk->root_dirent_lba,sblk->root_dirent_offset);
	int i = 0;
	struct ent *ent = parse_ent(dent);
	dent = parse_dent(ent->nxt_dirent_lba,ent->nxt_dirent_offset);
	int prev_lba,prev_offset;
	while(ssep[i] != (char*)0){
		if(strcmp(dent->name,ssep[i]) == 0 && ssep[i + 1] != (char*)0){
			ent = _parse_ent(dent->curr_ent_lba,dent->curr_ent_offset);
			prev_lba = ent->nxt_dirent_lba;
			prev_offset = ent->nxt_dirent_offset;
	                dent = parse_dent(ent->nxt_dirent_lba,ent->nxt_dirent_offset);
			i++;
			continue;
		}else if(strcmp(dent->name,ssep[i]) == 0){
			kprintf("Error:Directory exists\n");
			return -1;
		}
		if(dent->nxt_dirent_lba == 0 && ssep[i + 1] == (char*)0){
			break;
		}else if(dent->nxt_dirent_lba == 0){
			kprintf("[FATAL]Early end in directory\n");
			panic();
		}
		dent = parse_dent(dent->nxt_dirent_lba,dent->nxt_dirent_offset);
	}
	struct free_blk *fblk = _find_free(sizeof(*dent));// = find_free_ent(sblk->root_dirent_lba,sblk->root_dirent_offset,sizeof(*dent));
	kprintf("%d %d\n",fblk->lba,fblk->offset);
	
	struct dirent *ndent = malloc(sizeof(*dent)*sizeof(dent));
	ndent->alloc = 1;
	ndent->size = sizeof(*ndent) + strlen(ssep[i]);
	ndent->namelen = strlen(ssep[i]);
	strcpy(ndent->name,ssep[i]);
	ndent->first_fent_lba = 0;
	ndent->first_fent_offset = 0;
	ndent->nxt_dirent_lba = 0;
	ndent->nxt_dirent_offset = 0;
	struct free_blk *nfblk = _find_free(sizeof(struct ent));
	ndent->curr_ent_lba = nfblk->lba;
	ndent->curr_ent_offset = nfblk->offset;
	kprintf("%d %d\n",nfblk->lba,nfblk->offset);
	write_dirent(ndent,nfblk->lba,nfblk->offset);
	struct ent *nent = malloc(sizeof(*ent)*sizeof(ent));
	nent->alloc = 1;
	nent->size =sizeof(*nent);
	nent->nxt_ent_lba = 0;
	nent->nxt_ent_offset = 0;
	nent->nxt_dirent_lba = 0;
	nent->nxt_dirent_offset = 0;
	debug("MKDIR","Done");
	return 1;
}
int __mkdir(const char *path){
//	debug("MKDIR",path);
	if(strcmp(path,"/") == 0)
		return __broken_mkdir(path);
//	else if(isRootDir(path))
//		return __broken_mkdir(path);
	else{
		int i = 0;
		struct superblock *sblk = parse_superblk(*(int*)0x501);
		struct dirent *ent = parse_dent(sblk->root_dirent_lba,sblk->root_dirent_offset);
		while(1){
			char *part1 = malloc(80);
			while(path[i] == '/')
				i++;
			int j = 0;
			while(path[i] != '/' && path[i] != 0){
				part1[j] = path[i];
				i++;
				j++;
			}
			int brk = 0;
			if(path[i] == 0)
				break;
			int lba = ent->nxt_dirent_lba;
			int offset = ent->nxt_dirent_offset;
			while(1){
				if(strcmp(ent->name,part1) == 0)
					break;
				lba = ent->nxt_dirent_lba;
				offset = ent->nxt_dirent_offset;
				if(lba == 0 && (path[i] != 0 && path[i + 1] != 0)){
					kprintf("No such file or directory:%s\n",part1);
					return -1;
				}else if(lba == 0)
					break;
				ent = parse_dent(lba,offset);
			}
			struct ent *rent = parse_ent(ent);
			ent = parse_dent(rent->nxt_dirent_lba,rent->nxt_dirent_offset);
		}
		struct ent *rent = parse_ent(ent);
		struct free_blk *fblk = find_free_ent(sblk->root_dirent_lba,sblk->root_dirent_offset,sizeof(*ent));
		struct ent *prev;
		while(rent->nxt_dirent_lba != 0){
			prev = rent;
			rent = _parse_ent(rent->nxt_ent_lba,rent->nxt_ent_offset);
		}
		ent->nxt_dirent_lba = fblk->lba;
		ent->nxt_dirent_offset = fblk->offset;
		/*write_ent(rent,prev->nxt_ent_lba,prev->nxt_ent_offset);
		struct dirent *fin = malloc(sizeof(*fin)*sizeof(fin));
		fin->alloc = 1;
		char *name = malloc(80);
		int nc = countc(path,'/');
		int j = 0,k = 0;
		i = 0;
		while(path[i] != 0){
			if((k == nc && path[strlen(path) - 1] != '/') || (k == nc - 1 && path[strlen(path) - 1] == '/')){
				name[j] = path[i];
				j++;
			}
			if(path[i] == '/')
				k++;
			i++;
		}
		kprintf("%s\n",name);
		fin->namelen = strlen(name);
		strcpy(fin->name,name);
		fin->first_fent_lba = 0;
		fin->first_fent_offset = 0;
		fin->nxt_dirent_lba = 0;
		fin->nxt_dirent_offset = 0;
		struct free_blk *fr = find_free_ent(sblk->root_dirent_lba,sblk->root_dirent_offset,sizeof(*fr));
		fin->curr_ent_lba = fr->lba;
		fin->curr_ent_offset = fr->offset;
		write_dirent(fin,fblk->lba,fblk->offset);*/
		
	}
}
int countc(const char *str,char c){
	int i = 0;
	int ret = 0;
	while(str[i] != 0){
		if(str[i] == c)
			ret++;
		i++;
	}
	return ret;
}
int mkfs(){
	return mkfs_zfs(*(int*)0x501);
}
int mkfs_zfs(int lba){
	debug("MKFS_ZFS","Writing superblock");
	struct superblock *sblk = malloc(512);
	memcpy(sblk->sig,sig,16);
	sblk->root_dirent_lba = lba + 1;
	sblk->root_dirent_offset = 0;
	uint8_t *buf = malloc(512);
	for(int i = 0; i < 512;i++)
		buf[i] = 0;
	for(int i = 0; i <= 4;i++)
		buf[i] = sblk->sig[i];
	int j = 4;
	for(int i = 24; i >= 0;i-=8,j++)
		buf[j] = sblk->root_dirent_lba >> i;
	buf[j + 5] = sblk->root_dirent_offset >> 8;
	buf[j + 6] = sblk->root_dirent_offset;
	ata_write_master(buf,lba);
	debug("MKFS_ZFS","Writing root directory");
	int err = mkdir("/");
	if(err < 0)
		panic();
}
struct superblock *parse_superblk(int lba){
	struct superblock * ret = malloc(512);
	uint8_t *buf = malloc(512);
	ata_read_master(buf,lba,0);
	ret->sig = malloc(1024);
	memcpy(ret->sig,buf,4);
	ret->root_dirent_lba = buf[4] << 24 | buf[5] << 16 | buf[6] << 8 | buf[7];
	ret->root_dirent_offset = buf[8] << 8 | buf[9];
	return ret;
}
__DIR *__opendir(const char *dirname){
	struct superblock *sblk = parse_superblk(*(int*)0x501);
	if(strcmp(dirname,"/") != 0){
		char *name = part(dirname,1,'/');
		kprintf("%s\n",name);
		return (__DIR*)ERR;
	}
	else{
		__DIR *ret = malloc(sizeof(*ret) * sizeof(ret));
		ret->dirent = parse_dirent("/");
		if(strcmp(ret->dirent->name,"/") != 0){
			debug("OPENDIR","Invalid FS");
			panic();
		}
	}
}
#endif
