#ifndef __SIMPFS_H
#define __SIMPFS_H
#include <stdint.h>
struct tree_ent{
	uint8_t alloc;
	uint8_t type;
	uint32_t nxtLba;
	uint32_t currLba;
};
#define __TYPE_DIR 0
#define __TYPE_FILE 1
#define __TYPE_DEV 2
struct tree_filehdr{
	uint8_t alloc;
	uint8_t namelen;
	uint8_t name[80];
};
struct tree_dirhdr{
	uint32_t nxtTreeLba;
};
struct fd{
	uint8_t alloc;
	uint8_t type;
	uint8_t name[80];
	uint32_t pos;
	uint32_t tLba;
};
#define O_RDONLY 0b01
#define O_WRONLY 0b10
#define O_RDWR 0b11
typedef struct __DIR{
	struct tree_ent *ent;
	struct tree_filehdr *fhdr;
	struct tree_dirhdr *dhdr;
}DIR;
int open(const char *fname,int options);
struct fd *getInfo(const char *str);
int mkfs();
int mkdir(const char *name);
DIR *opendir(const char *name);

#endif
