#ifdef OBSOLETE_KERNEL
#ifndef __GENFS_H
#define __GENFS_H
#include <stdint.h>
#define TYPE_DIR 0
#define TYPE_FILE 1
#define TYPE_DEV 2
#define O_RDONLY 0x01
#define O_WRONLY 0x02
#define O_RDWRITE 0x03
# define SEEK_SET       0       /* Seek from beginning of file.  */
# define SEEK_CUR       1       /* Seek from current position.  */
# define SEEK_END       2       /* Seek from end of file.  */
#define SEEK_CURR SEEK_CUR
struct fd{
	uint8_t alloc;
	uint8_t name[80];
	uint32_t pos_lba;
	uint16_t pos_offset;
	uint8_t flags;
};
struct __superblock{
	uint8_t sig[4];
	uint32_t root_dir_offset;
	uint16_t root_dir_lba;
};
struct __ent{
	uint8_t alloc;
	uint8_t ent_type;
	uint8_t type;
	uint8_t namelen;
	uint8_t name[80];
	uint32_t nxt_ent_lba;
	uint16_t nxt_ent_offset;
	uint32_t data_ent_lba;
	uint16_t data_ent_offset;
};
struct __data_ent{
	uint8_t alloc;
	uint8_t ent_type;
	uint8_t type;
	uint32_t nxt_lba;
	uint16_t nxt_offset;
	uint32_t data_lba;
	uint16_t data_offset;
};
struct __data{
	uint32_t lba;
	uint16_t offset;
};
#ifndef LINUX
typedef struct DIR{
	struct __ent *ent;
	struct __data_ent *dent;
}DIR;
#else
typedef struct __DIR{
	struct __ent *ent;
	struct __data_ent *dent;
}__DIR;
#endif
struct __dirent{
	int d_type;
	char d_name[256];
};
#ifndef LINUX
typedef struct FILE{
/*
*TODO
*/
}FILE;
#endif
struct __file_ent{
	/*uint8_t alloc;
	uint8_t ent_type;
	uint8_t name[256];
	uint8_t first_dat;*/
};
struct __fdat{
	uint8_t alloc;
	uint8_t ent_type;
	uint32_t tlba;
	uint32_t slba;
};
struct __finfo{
	unsigned int pos_lba;
	unsigned int pos_offset;
	unsigned char flags;
};
typedef struct KFILE{
	struct __ent *fent;
	struct __fdat *fdat;
	struct __finfo *finfo;
}KFILE;
#ifndef LINUX
DIR *opendir(const char *str);
struct superblock *parse_superblock(int lba);
struct ent *parse_ent(char *path);
struct ent *parse_data_ent(struct ent *e);
void __mkfs_genfs();
int __is_genfs();
int __gen_mkdir(const char *path);
int fputc(int c,FILE *f);
int putc(int c,FILE *f);
int getc(FILE *f);
int fgetc(FILE *f);
struct __dirent *readdir(DIR *d);
struct __ent * ___parse_ent(int lba,int offset);
int mkdir(const char *path);
struct __data_ent *__parse_dent(int lba,int offset);
int list(const char *path);
int write_file(char *fpath,uint8_t *dat, int n);
//int open(const char *str,int flags,int mode);
int read(int fd,void *buf,int count);
int write(int fd,void *buf,int count);
int close(int fd);
#endif
#ifdef LINUX
int write_dirs(const char *path,FILE *f);
#endif
#endif
#endif
