#ifndef __LIBZFS_H
#define __LIBZFS_H
#include <stdint.h>
#include "lib.h"
//const uint8_t sig[5] = {0x7F,'z','f','s',0};
struct free_blk{
	uint32_t lba;
	uint16_t offset;
};
struct superblock{
	uint8_t *sig;
	uint32_t root_dirent_lba;
	uint16_t root_dirent_offset;
};
struct dirent{
	uint8_t alloc;
	uint8_t size;
	uint8_t namelen;
	uint8_t name[80];
	uint32_t first_fent_lba;
	uint16_t first_fent_offset;
	uint32_t nxt_dirent_lba;
	uint16_t nxt_dirent_offset;
	uint32_t curr_ent_lba;
	uint16_t curr_ent_offset;
};
struct ent{
	//DIR
	uint8_t alloc;
	uint8_t size;
	uint32_t nxt_ent_lba;
	uint16_t nxt_ent_offset;
	uint32_t nxt_dirent_lba;
	uint16_t nxt_dirent_offset;
	//FILE
};
typedef struct{
	struct dirent *dirent;
	struct ent *ent;
}__DIR;
#ifndef KERNEL
int main(int argc,char *argv[]);
#else
int countc(const char *str,char c);
int mkfs();
int is_zfs();
int mkdir(const char *path);
int mkfs_zfs(int lba);
struct superblock *parse_superblk(int lba);
struct dirent *parse_dirent(const char *path);
#endif
#endif
