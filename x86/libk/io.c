#ifdef __BUILD_FS_IO
#include "fs.h"
#include <lib.h>
int getLastInstance(const char *name,int c){
	int i = 0;
	int ret = 0;
	while(name[i] != 0){
		if(name[i] == c)
			ret = i;
		i++;
	}
	return ret;
}
int _mkdir(const char *dirname){
	char *path = malloc(1024);
	int end = getLastInstance(dirname,'/');
	for(int i = 0; i < end;i++){
		path[i] = dirname[i];
	}
	if(strcmp(path,"") == 0){
		__libk_dir_ent *dent = get_dir_ent("/",0);
		if(has_dir(dent,dirname)){
			kprintf("Failed to create directory:Directory exists!\n");
			return -1;
		}
		dent = get_dir_ent("/",0);
		dent = get_last_dirent(dent);
		__libk_dir_ent *ret = malloc(1024);
		ret->alloc = 1;
		ret->size = sizeof(*ret);
		ret->namelen = strlen(dirname);
		ret->name = dirname;
		ret->perms = PERM_NOEXEALL << 16 | PERM_WRITEALL << 8 | PERM_READALL;
		ret->nxt_ent_lba = 0;
		ret->nxt_ent_offset = 0;
		ret->nxt_dirent_lba = 0;
		ret->nxt_dirent_offset = 0;
		write_dir_ent(dirname,dent,ret);
		return 1;
	}else{
		return -1;
	}
}
#endif
