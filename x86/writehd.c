#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <dirent.h>
#include <fcntl.h>
#include "simpfs.h"
int recurse_write(char *in,char *name){
	FILE *f = fopen(name,"r+b");
	if(!f){
		fprintf(stderr,"%s:",name);
		perror("[0]Couldn't open file");
		return -1;
	}
	DIR *d = opendir(in);
	if(!d){
		fprintf(stderr,"%s:",in);
		perror("Couldn't open dir");
		return -1;
	}
	struct dirent *ent;
	while((ent = readdir(d))){
		if(ent->d_type == DT_DIR){
			if(strcmp(ent->d_name,".") == 0 || strcmp(ent->d_name,"..") == 0)
				continue;
			char *path = malloc(1024);
			sprintf(path,"/%s/%s",in,ent->d_name);
			int val = __mkdir(path,f);
			if(!val){
				printf("Failed to write dir %s\n",path);
			}
			fclose(f);
			f = fopen(name,"r+b");
			if(!f){
				fprintf(stderr,"%s:",name);
				perror("[1]Couldn't open file");
				return -1;
			}
		}
	}
	fclose(f);
	closedir(d);
	d = opendir(in);
	if(!d){
		fprintf(stderr,"%s:",in);
		perror("Couldn't open dir");
		return -1;
	}
	while((ent = readdir(d))){
		if(ent->d_type == DT_DIR){
			if(strcmp(ent->d_name,".") == 0 || strcmp(ent->d_name,"..") == 0)
                                continue;
                        char *path = malloc(1024);
                        sprintf(path,"%s/%s",in,ent->d_name);
			recurse_write(path,name);
		}
	}
	closedir(d);
	return 1;
}
int main(int argc,char **argv){
	if(argc < 3)
		return -1;
	FILE *f = fopen(argv[2],"wb");
	if(!f)
		return -1;
	mkfs(f);
	fclose(f);
	f = fopen(argv[2],"r+b");
	__mkdir("/",f);
	fclose(f);
	f = fopen(argv[2],"r+b");
	char *path = malloc(1024);
	sprintf(path,"/%s",argv[1]);
	__mkdir(path,f);
	fclose(f); 
	int ret = recurse_write(argv[1],argv[2]) ? 0 : -1;
	return ret;
}
