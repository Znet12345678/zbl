#ifdef OBSOLETE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <dirent.h>
#include <genfs.h>
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
void usage(const char *arg){
	printf("Usage: %s in out\n",arg);
}

int f_write_master(FILE *out,uint8_t *data,int lba){
	fseek(out,lba * 512,SEEK_SET);
	for(int i = 0; i < 512;i++)
		putc(data[i],out);
	return 1;
}
int f_read_master(FILE *in,uint8_t *buf,int lba){
	fseek(in,lba*512,SEEK_SET);
	for(int i = 0; i < 512;i++)
		buf[i] = getc(in);
}
struct __superblock *__parse_superblock(FILE *in){
	uint8_t *buf = malloc(1024);
	f_read_master(in,buf,0);
	struct __superblock *sblk = malloc(sizeof(*sblk));
	memcpy(sblk,buf,sizeof(*sblk));
	return sblk;
}
struct __ent *__parse_ent(int lba,int offset,FILE *in){
	uint8_t *buf = malloc(1024);
	f_read_master(in,buf,lba);
	struct __ent *ret = malloc(sizeof(*ret));
	memcpy(ret,buf + offset,sizeof(*ret));
	free(buf);
	return ret;
}
struct __data_ent *__parse_dent(int lba,int offset,FILE *in){
	uint8_t *buf = malloc(1024);
	f_read_master(in,buf,lba);
	struct __data_ent *ret = malloc(sizeof(*ret));
	memcpy(ret,buf + offset,sizeof(*ret));
	free(buf);
	return ret;
}
__DIR *__opendir(const char *path,FILE *in){
	struct __superblock *sblk = __parse_superblock(in);
	__DIR *ret = malloc(sizeof(*ret));
	if(strcmp(path,"/") == 0){
		struct __ent *ent = __parse_ent(sblk->root_dir_lba,sblk->root_dir_offset,in);
		ret->ent = ent;
		ret->dent = __parse_dent(ent->data_ent_lba,ent->data_ent_offset,in);
		return ret;
	}
	char **arr = sep(path,'/');
	int last = 0;
	while(arr[last] != 0)
		last++;
	int i = 0;
	__DIR *root = __opendir("/",in);
	ret->dent = __parse_dent(root->ent->data_ent_lba,root->ent->data_ent_offset,in);
	struct __ent *ent= __parse_ent(root->dent->data_lba,root->dent->data_offset,in);
	//printf("%s\n",arr[last - 2]);
	while(i < last - 1){
		while(strcmp(ent->name,arr[i]) != 0){
			if(ent->alloc != 1){
				printf("No such file or directory! Looking for %s\n",arr[i]);
				return 0;
			}
			ent = __parse_ent(ent->nxt_ent_lba,ent->nxt_ent_offset,in);
		}
		ret->dent = __parse_dent(ent->data_ent_lba,ent->data_ent_offset,in);
		ent = __parse_ent(ret->dent->data_lba,ret->dent->data_offset,in);
		i++;
	}
	ret->ent = ent;
	return ret;
}
struct __fdat *__parse_fdat(int lba,int offset,FILE *in){
	uint8_t *buf = malloc(1024);
	f_read_master(in,buf,lba);
	struct __fdat *fdat = malloc(sizeof(*fdat));
	memcpy(fdat,buf + offset,sizeof(*fdat));
	return fdat;
}
struct __data *__find_free_blk(FILE *in){
	struct __superblock *sblk = __parse_superblock(in);
	uint8_t *buf = malloc(1024);
	int offset = sblk->root_dir_offset,lba = sblk->root_dir_lba;
	f_read_master(in,buf,lba);
	while(buf[offset] == 1){
		if(buf[offset + 1] == 0){
			if(offset + sizeof(struct __ent) * 2 < 512)
				offset+=sizeof(struct __ent);
			else{
				lba++;
				offset = 0;
			}
		}else if(buf[offset + 1] == 1){
			if(offset + sizeof(struct __data_ent) * 2 < 512)
				offset+=sizeof(struct __data_ent);
			else{
				lba++;
				offset = 0;
			}
		}else if(buf[offset + 1] == 4){
			struct __fdat *fdat = __parse_fdat(lba,offset,in);
			lba+=fdat->tlba + 1;
		}
		f_read_master(in,buf,lba);
	}
	struct __data *ret = malloc(sizeof(*ret));
	ret->lba = lba;
	ret->offset = offset;
	//printf("%d %d\n",ret->lba,ret->offset);
	return ret;
}
int __mkdir(const char *dir,FILE *in,FILE *out){
	//printf("[MKDIR]%s\n",dir);
	struct __superblock *sblk = __parse_superblock(in);
	char **path = sep(dir,'/');
	int last = 0;
	while(path[last] != 0)
		last++;
//	if(dir[strlen(dir) - 1] != '/')
//		last++;
	if(strcmp(dir,"/") == 0){
		struct __ent *ent = malloc(sizeof(*ent));
		ent->alloc = 1;
		ent->ent_type = 0;
		ent->type =TYPE_DIR;
		ent->namelen = 1;
		strcpy(ent->name,"/");
		ent->nxt_ent_lba = 0;
		ent->nxt_ent_offset = 0;
		ent->data_ent_lba = sblk->root_dir_lba;
		ent->data_ent_offset = sblk->root_dir_offset + sizeof(*ent);
		uint8_t *pntr = malloc(1024);
		f_read_master(in,pntr,sblk->root_dir_lba);
		memcpy(pntr + sblk->root_dir_offset,ent,sizeof(*ent));
		f_write_master(out,pntr,sblk->root_dir_lba);
		struct __data_ent *dent = malloc(sizeof(*dent));
		dent->alloc = 1;
		dent->ent_type = 1;
		dent->type = TYPE_DIR;
		dent->nxt_lba = 0;
		dent->nxt_offset = 0;
		dent->data_lba = 0;
		dent->data_offset = 0;
		free(pntr);
		pntr = malloc(1024);
		f_read_master(in,pntr,ent->data_ent_lba);
		memcpy(pntr + ent->data_ent_offset,dent,sizeof(*dent));
		f_write_master(out,pntr,ent->data_ent_lba);
		struct __data *d = __find_free_blk(in);
		dent->data_lba = d->lba;
		dent->data_offset = d->offset;
		memcpy(pntr + ent->data_ent_offset,dent,sizeof(*dent));
		f_write_master(out,pntr,ent->data_ent_lba);
		return 1;
	}
	char *prev = malloc(1024);
	strcpy(prev,"/");
	int i = 0;
	while(i < last - 1){
		strcat(prev,path[i]);
		strcat(prev,"/");
		i++;
	}
	__DIR *d = __opendir(prev,in);
	if(!d)
		return -1;
	int prev_lba = d->dent->data_lba, prev_offset = d->dent->data_offset;
	struct __ent *ent = __parse_ent(prev_lba,prev_offset,in);
	while(ent->nxt_ent_lba > 0){
		prev_lba = ent->nxt_ent_lba;
		prev_offset = ent->nxt_ent_offset;
		ent = __parse_ent(ent->nxt_ent_lba,ent->nxt_ent_offset,in);
	}
//	printf("[DEBUG]%s\n",path[last - 2]);
	struct __data *dat = __find_free_blk(in);
	ent->nxt_ent_lba = dat->lba;
	ent->nxt_ent_offset = dat->offset;
	uint8_t *wrt = malloc(1024);
	f_read_master(in,wrt,prev_lba);
	memcpy(wrt + prev_offset,ent,sizeof(*ent));
	f_write_master(out,wrt,prev_lba);
	//free(wrt);
	//wrt = malloc(1024);
	struct __ent *_ent = malloc(sizeof(*ent));
	_ent->alloc = 1;
	_ent->ent_type = 0;
	_ent->type = TYPE_DIR;
	_ent->namelen = strlen(path[last - 1]);
	for(int i = 0; i < sizeof(_ent->name);i++)
		_ent->name[i] = 0;
	strcpy(_ent->name,path[last - 1]);
	_ent->nxt_ent_lba = 0;
	_ent->nxt_ent_offset = 0;
	_ent->data_ent_lba = 0;
	_ent->data_ent_offset = 0;
	f_read_master(in,wrt,dat->lba);
	memcpy(wrt + dat->offset,_ent,sizeof(*_ent));
	f_write_master(out,wrt,dat->lba);
	struct __data *da = __find_free_blk(in);
	//free(wrt);
	//wrt = malloc(1024);
	_ent->data_ent_lba = da->lba;
	_ent->data_ent_offset = da->offset;
	memcpy(wrt + dat->offset,_ent,sizeof(*ent));
	f_write_master(out,wrt,dat->lba);
	//free(wrt);
	//wrt = malloc(1024);
	struct __data_ent *dent = malloc(sizeof(*dent));
	dent->alloc = 1;
	dent->ent_type = 1;
	dent->type = TYPE_DIR;
	dent->nxt_lba = 0;
	dent->nxt_offset = 0;
	dent->data_lba = 0;
	dent->data_offset = 0;
	f_read_master(in,wrt,da->lba);
	memcpy(wrt + da->offset,dent,sizeof(*dent));
	f_write_master(out,wrt,da->lba);
	//free(wrt);
	//wrt = malloc(1024);
	struct __data *d2 = __find_free_blk(in);
	dent->data_lba = d2->lba;
	dent->data_offset = d2->offset;
	memcpy(wrt + da->offset,dent,sizeof(*dent));
	f_write_master(out,wrt,da->lba);
	struct __ent *__ent = malloc(sizeof(*ent));
	__ent->alloc = 1;
	__ent->ent_type = 0;
	__ent->type = TYPE_DIR;
	__ent->namelen = 1;
	strcpy(__ent->name,".");
	f_read_master(in,wrt,d2->lba);
	memcpy(wrt + d2->offset,__ent,sizeof(*__ent));
	f_write_master(out,wrt,d2->lba);
	free(wrt);
	free(d2);
	free(dent);
	free(ent);
	free(_ent);
	//printf("[MKDIR FINISH]%s\n",dir);
	return 1;
}
int main(int argc,char *argv[]){
	if(argc != 3){
		usage(argv[0]);
		return -1;
	}
	struct __superblock *sblk = malloc(sizeof(*sblk));
	sblk->sig[0] = 0x7f;
	sblk->sig[1] = 'G';
	sblk->sig[2] = 'E';
	sblk->sig[3] = 'N';
	sblk->root_dir_offset = 0;
	sblk->root_dir_lba = 1;
	FILE *out = fopen(argv[2],"r+b");
	if(!out){
		perror("Couldn't open file");
		return -1;
	}
	uint8_t *buf = malloc(1024);
	for(int i = 0; i < 512;i++)
		buf[i] = 0;
	memcpy(buf,sblk,sizeof(*sblk));
	f_write_master(out,buf,0);
	__mkdir("/",out,out);
	char *orig = malloc(1024);
	sprintf(orig,"/%s",argv[1]);
	__mkdir(orig,out,out);
	write_dirs(argv[1],out);
	fclose(out);
	return 0;
}
int write_file(const char *path,FILE *f,FILE *io){
	if(!f || !io)
		return -1;
	char **arr = sep(path,'/');
	int last = 0;
	while(arr[last] != 0)
		last++;
	char *prev = malloc(1024);
	strcpy(prev,"/");
	for(int i = 0; i < last - 1;i++){
		strcat(prev,arr[i]);
		strcat(prev,"/");
	}
	//printf("[WRITE_FILE]Opening %s\n",prev);
	__DIR *d = __opendir(prev,io);
	struct __ent *ent = __parse_ent(d->dent->data_lba,d->dent->data_offset,io);
	int prev_lba = d->dent->data_lba,prev_offset = d->dent->data_offset;
	while(ent->nxt_ent_lba > 0){
		prev_lba = ent->nxt_ent_lba;
		prev_offset = ent->nxt_ent_offset;
		ent = __parse_ent(ent->nxt_ent_lba,ent->nxt_ent_offset,io);
	}
	struct __data *dat = __find_free_blk(io);
	ent->nxt_ent_lba = dat->lba;
	ent->nxt_ent_offset = dat->offset;
	uint8_t *pntr = malloc(1024);
	f_read_master(io,pntr,prev_lba);
	memcpy(pntr + prev_offset,ent,sizeof(*ent));
	f_write_master(io,pntr,prev_lba);
	free(ent);
	//printf("%d %d %d %d\n",ent->nxt_ent_lba,ent->nxt_ent_offset,prev_lba,prev_offset);
	ent = malloc(sizeof(*ent));
	ent->alloc = 1;
	ent->ent_type = 0;
	ent->type = TYPE_FILE;
	//printf("Writing %s\n",arr[last - 1]);
	ent->namelen = strlen(arr[last - 1]);
	for(int i = 0; i < sizeof(ent->name);i++)
		ent->name[i] = 0;
	strcpy(ent->name,arr[last - 1]);
	//printf("Writing %s\n",ent->name);
	ent->nxt_ent_lba = 0;
	ent->nxt_ent_offset = 0;
	ent->data_ent_lba = 0;
	ent->data_ent_offset = 0;
	f_read_master(io,pntr,dat->lba);
	memcpy(pntr + dat->offset,ent,sizeof(*ent));
	f_write_master(io,pntr,dat->lba);
	struct __data *_d = __find_free_blk(io);
	//printf("%d\n",_d->lba);
	ent->data_ent_lba = _d->lba;
	ent->data_ent_offset = _d->offset;
	memcpy(pntr + dat->offset,ent,sizeof(*ent));
	//printf("%d %d\n",ent->data_ent_lba,ent->data_ent_offset);
	//printf("%d\n",pntr[dat->offset]);
	f_write_master(io,pntr,dat->lba);
	struct __fdat *fdat = malloc(sizeof(*fdat));
	fseek(f,0,SEEK_END);
	uint32_t bytes = ftell(f);
	fseek(f,0,SEEK_SET);
	int lba = _d->lba,offset = _d->offset;
	//struct __fdat *fdat = malloc(sizeof(*fdat));
	fdat->alloc = 1;
	fdat->ent_type = 4;
	fdat->tlba = (bytes/512 + 1);
	fdat->slba = _d->lba + 1;
	uint8_t *b = malloc(1024);
	f_read_master(io,b,_d->lba);
	memcpy(b + _d->offset,fdat,sizeof(*fdat));
	f_write_master(io,b,_d->lba);
	int c;
	int pos = 0;
	uint32_t nlba = _d->lba + 1;
	while((c = getc(f)) != EOF){
		if(pos % 512 == 0){
			fputc(1,io);
			pos++;
		}
		fputc(c,io);
		pos++;
	}
	return 1;
	/*while(bytes > 0){
		fdat->alloc = 1;
		fdat->ent_type = 4;
		fdat->nxt_fdat_lba = 0;
		fdat->nxt_fdat_offset = 0;
		f_read_master(io,pntr,lba);
		memcpy(pntr + offset,fdat,sizeof(*fdat));
		f_write_master(io,pntr,lba);
		struct __data *nd = __find_free_blk(io);
		fdat->nxt_fdat_lba = nd->lba;
		fdat->nxt_fdat_offset = nd->offset;
		//printf("{%d %d}\n",nd->lba,nd->offset);
		//printf("[%d %d]\n",fdat->nxt_fdat_lba,fdat->nxt_fdat_offset);
		memcpy(pntr + offset,fdat,sizeof(*fdat));
		f_write_master(io,pntr,lba);
		uint8_t *buf = malloc(1024);
		for(int i = 0; i < 512;i++)
			buf[i] = 0;
		if(bytes > (512 - sizeof(*fdat) - offset))
			for(int i = 0; i < 512 - sizeof(*fdat) - offset;i++)
				buf[i] = getc(f);
		else
			for(int i = 0; i < bytes;i++)
				buf[i] = getc(f);
		f_read_master(io,pntr,lba);
		memcpy(pntr + offset + sizeof(*fdat),buf,512 - sizeof(*fdat) - offset);
		f_write_master(io,pntr,lba);
		bytes-=(512 - sizeof(*fdat) - offset);
		lba = fdat->nxt_fdat_lba;
		offset = fdat->nxt_fdat_offset;
		free(fdat);
		fdat = malloc(sizeof(*fdat));
		free(buf);
//		printf("%d\n",bytes);
//		bytes-=(512 - sizeof(*fdat) - offset);
	}
//	printf("%d\n",lba);
	return 1;*/
}
int write_dirs(const char *name,FILE *f){
	DIR *d = opendir(name);
	if(!d){
		perror("Couldn't open dir");
		return -1;
	}
	struct dirent *ent;
	while((ent = readdir(d))){
		if(ent->d_type == DT_DIR){
			if(strcmp(ent->d_name,".") == 0 || strcmp(ent->d_name,"..") == 0)
				continue;
			char *rpath = malloc(1024);
			sprintf(rpath,"/%s/%s",name,ent->d_name);
			__mkdir(rpath,f,f);
		}
	}
	closedir(d);
	free(ent);
	d = opendir(name);
	while(ent = readdir(d)){
		if(ent->d_type == DT_REG){
			char *rpath = malloc(1024);
			sprintf(rpath,"/%s/%s",name,ent->d_name);
			char *path = malloc(1024);
			sprintf(path,"%s/%s",name,ent->d_name);
			FILE *nf = fopen(path,"rb");
			if(!nf){
				printf("%s:",path);
				perror("Couldn't open file");
				return -1;
			}
			write_file(rpath,nf,f);
			fclose(nf);
		}
	}
	closedir(d);
	free(ent);
	d = opendir(name);
	while((ent = readdir(d))){
		if(ent->d_type == DT_DIR){
			if(strcmp(ent->d_name,".") == 0 || strcmp(ent->d_name,"..") == 0)
				continue;
			char *path = malloc(1024);
			sprintf(path,"%s/%s",name,ent->d_name);
			write_dirs(path,f);
		}
	}
	//fclose(f);
	return 1;
}
#endif
