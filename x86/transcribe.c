#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <dirent.h>
#include <fcntl.h>
#include "simpfs.h"
uint32_t find_free(FILE *f){
        uint32_t ret = 1;
        uint8_t *buf = malloc(512);
        int r = _ata_read_master(buf,ret,f);
    	if(r < 512)
		return ret;
	while(buf[0]){
                ret++;
		free(buf);
		buf = malloc(512);
                r = _ata_read_master(buf,ret,f);
		if(r < 512)
			return ret;
        }
        return ret;
}
int mkfs(FILE *f){
        uint8_t *buf = malloc(512);
	bzero(buf,512);
        buf[0] = 0x7f;
        buf[1] = 'S';
        buf[2] = 'I';
        buf[3] = 'M';
        buf[4] = 'P';
        struct tree_ent *head = malloc(512);
	bzero(head,512);
        head->alloc = 1;
        head->type = __TYPE_DIR;
        head->nxtLba = 1;
        memcpy(buf + 5,head,sizeof(*head));
        _ata_write_master(buf,0,f);

}

int _ata_read_master(void *buf,int lba,FILE * f){
	fseek(f,lba * 512,SEEK_SET);
	int r = fread(buf,sizeof(uint8_t),512,f);
	if(r == 0)
		bzero(buf,512);
	return r;
}
int _ata_write_master(void *buf,int lba,FILE * f){
	fseek(f,lba*512,SEEK_SET);
	int r = fwrite(buf,sizeof(uint8_t),512,f);

	return r;
}
char **sep(const char *str,int c){
        int i = 0,k = 0,l = 0;
        char **ret = (char**)malloc(102400);
	bzero(ret,102400);
	
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
        return ret;
}
struct __DIR *__opendir(const char *name,FILE *f){
	uint8_t **s = sep(name,'/');
        struct tree_ent *ent = malloc(512);
        uint8_t *buf = malloc(1024);
        _ata_read_master(buf,0,f);
        memcpy(ent,buf + 5,sizeof(*ent));
        struct tree_dirhdr *dhdr = malloc(512);
        int i = 0;
        int prevlba = 0;
        struct tree_filehdr *fhdr = malloc(sizeof(*fhdr));
        while(s[i] != 0){
                _ata_read_master(buf,ent->nxtLba,f);
                memcpy(ent,buf,sizeof(*ent));
                while(ent->alloc){
                        if(ent->type == __TYPE_DIR){
                                memcpy(fhdr,buf + sizeof(*ent),sizeof(*fhdr));
                                if(strcmp(fhdr->name,s[i]) == 0){
                                        prevlba = ent->nxtLba;
                                        _ata_read_master(ent,dhdr->nxtTreeLba,f);
                                        break;
                                }
                        }
                        _ata_read_master(buf,ent->nxtLba,f);
                        memcpy(ent,buf,sizeof(*ent));
               }
                if(!ent->alloc)
                        return 0;
                i++;
        }
        struct __DIR *ret = malloc(sizeof(*ret));
        *ret->ent = *ent;
        *ret->fhdr = *fhdr;
        *ret->dhdr = *dhdr;
        return ret;
}
int __mkdir(const char *name,FILE *f){

        uint8_t **s = sep(name,'/');
        int i = 0;
        uint8_t *buf = malloc(512);
        struct tree_ent *ent = malloc(512);
	bzero(ent,512);
        _ata_read_master(buf,0,f);
        memcpy(ent,buf + 5,512-5);
        int prevLba = ent->nxtLba;
        struct tree_dirhdr *dhdr = malloc(512);
    	bzero(dhdr,512);
	while(s[i+1] != 0){
                _ata_read_master(buf,ent->nxtLba,f);
                memcpy(ent,buf,sizeof(*ent));
                while(ent->alloc){
                        if(ent->type == __TYPE_DIR){
                                struct tree_filehdr *fhdr = malloc(sizeof(*fhdr));
                                memcpy(fhdr,buf + sizeof(*ent),sizeof(*fhdr));
                                if(strcmp(fhdr->name,s[i]) == 0){
                                        prevLba = ent->nxtLba;
                                        _ata_read_master(ent,dhdr->nxtTreeLba,f);
                                        break;
                                }
                        }
                        _ata_read_master(buf,ent->nxtLba,f);
                        memcpy(ent,buf,sizeof(*ent));
                }
                if(!ent->alloc)
                        return 0;
                i++;
        }
        struct tree_filehdr *tfhdr = ent + sizeof(*ent);
        if(strcmp(tfhdr->name,s[i]) == 0 && strlen(s[i]) > 0){
                printf("Error: Directory exists!\n");
                return 0;
        }
	printf("%d\n",ent->nxtLba);
        while(ent->nxtLba != 0){
                prevLba = ent->nxtLba;
                struct tree_filehdr *fhdr = buf + sizeof(*ent);
                if(strcmp(fhdr->name,s[i]) == 0 && strlen(s[i]) > 0){
                        printf("Error: Directory exists!\n");
                        return 0;
                }
                _ata_read_master(buf,ent->nxtLba,f);
                memcpy(ent,buf,sizeof(*ent));
        }
        ent->nxtLba = find_free(f);
        _ata_write_master(ent,prevLba,f);
        free(ent);
        ent = malloc(sizeof(*ent));
	bzero(ent,sizeof(*ent));
        ent->alloc = 1;
        ent->type = __TYPE_DIR;
        ent->nxtLba = 0;
        free(dhdr);
        dhdr = malloc(sizeof(*dhdr));
        struct tree_filehdr *fhdr = malloc(1024);
        fhdr->alloc = 1;
	bzero(fhdr->name,80); 
       if(strcmp(name,"/") != 0){
                fhdr->namelen = strlen(s[i]);
                memcpy(fhdr->name,s[i],fhdr->namelen);
        }else{
                fhdr->namelen = strlen(name);

                memcpy(fhdr->name,name,strlen(name));
        }
        dhdr->nxtTreeLba = 0;
        buf = malloc(512);
	bzero(buf,512);
        uint32_t lba = find_free(f);
        memcpy(buf,ent,sizeof(*ent));
        _ata_write_master(buf,find_free(f),f);
        ent->nxtLba = find_free(f);
        memcpy(buf,ent,sizeof(*ent));
        memcpy(buf + sizeof(*ent),fhdr,sizeof(*fhdr));
        memcpy(buf + sizeof(*ent) + sizeof(*fhdr),dhdr,sizeof(*dhdr));
        _ata_write_master(buf,lba,f);
        dhdr->nxtTreeLba = find_free(f);
        memcpy(buf + sizeof(*ent) + sizeof(*fhdr),dhdr,sizeof(*dhdr));
        _ata_write_master(buf,lba,f);
	printf("WRITE:%s\n",name);
        return 1;
}
