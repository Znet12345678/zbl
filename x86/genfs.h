#ifndef __GENFS_H
#define __GENFS_H
#include <stdint.h>
#define TYPE_DIR 0
#define TYPE_FILE 1
#define TYPE_DEV 2
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
};
typedef struct KFILE{
	struct __ent *fent;
	uint8_t pos;
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
#endif
#ifdef LINUX
int write_dirs(const char *path,FILE *f);
#endif
#endif
