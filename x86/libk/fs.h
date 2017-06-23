#ifdef __BUILD_FS
#ifndef __LIBK_FS_H
#define __LIBK_FS_H
#ifndef __LIBK_FS_SIG
#define __LIBK_FS_SIG
#include <stdint.h>
#endif
static const uint8_t sig[4] = {0x6f,'z','f','s'};
typedef struct superblk{
	uint8_t sig[4];//0x6f,'z','f','s'
	uint8_t size;
	uint32_t root_lba;
	uint16_t root_offset;
}sblk;
typedef struct dirent{
	uint8_t alloc;
	uint8_t size;
	uint8_t namelen;
	uint8_t *name;
	uint16_t perms;
	uint32_t nxt_ent_lba;
	uint16_t nxt_ent_offset;
	uint32_t nxt_dirent_lba;
	uint16_t nxt_dirent_offset;
}__libk_dir_ent;
typedef struct ent{
	uint8_t alloc;
	uint8_t size;
	uint8_t namelen;
	uint8_t *name;
	uint32_t perms;
	uint8_t type;
	uint32_t lba;
	uint16_t offset;
	uint32_t nxt_ent_lba;
	uint16_t nxt_ent_offset;
}__libk_ent;
#define PERM_NOEXEALL 0
#define PERM_EXEALL 1
#define PERM_NOEXEADMIN 2
#define PERM_EXEADMIN 3
#define PERM_NOWRITEALL 0
#define PERM_WRITEALL 1
#define PERM_NOWRITEADMIN 2
#define PERM_WRITEADMIN 3
#define PERM_NOREADALL 0
#define PERM_READALL 1
#define PERM_NOREADADMIN 2
#define PERM_READADMIN 3
#define ENT_TYPE_DIR 0
#define ENT_TYPE_FILE 1
typedef struct file_ent{
	uint8_t alloc;
	uint32_t size;
	//data
	uint32_t nxt_lba;
	uint16_t nxt_offset;
}__libk_file_ent;
typedef struct FILE{
	__libk_file_ent *fent_pntr;
	__libk_ent *ent_pntr;
	int pos_byte;
	int pos_lba;
	int pos_offset;
	char *atrib;
}FILE;
#ifndef NULL
#define NULL 0
#endif
#ifdef KERNEL
int isfs();
struct fs_info{
	uint8_t alloc;
	uint32_t start_lba;
	uint8_t mount_path_len;
	uint8_t *mount_path;
};
FILE *fopen(const char *path,char *atrib);
int fclose(FILE *f);
__libk_file_ent *get_file_ent(const char *name,__libk_ent *ent);
__libk_dir_ent *get_dir_ent(const char *name,__libk_dir_ent *prevdir);
__libk_ent *get_ent(const char *name,__libk_dir_ent *dent,int type);
int write_file_ent(const char *name,__libk_ent *ent);
int write_ent(const char *name,__libk_dir_ent *prev,__libk_ent *went);
int write_dir_ent(const char *name,__libk_dir_ent *prev,__libk_dir_ent *went);
int mount(struct fs_info *fs);
#endif
#endif
#endif
