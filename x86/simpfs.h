/*
*simpFS.h
*Header for simple file system
*(c) 2017 Zachary James Schlotman
*/
#ifndef __SIMPFS_H
#define __SIMPFS_H
#include <stdint.h>
/*
*struct tree_ent:
*Placed at the start of every block
*/
struct tree_ent{
	uint8_t alloc;/*Has this block been initialized*/
	uint8_t type;/*What type is this block*/
	uint32_t nxtLba;/*What is the next LBA of the next part of this system*/
};
#define __TYPE_DIR 0 /*Directory type(as dirhdr as well as filehdr and ent*/
#define __TYPE_FILE 1 /*File type only has ent and filehdr*/
#define __TYPE_DEV 2 /*Device type CURRENTLY UNUSED*/
/*
*struct tee_filehdr
*Placed at offset sizeof(struct tree_ent) contains info for files and directories
*/
struct tree_filehdr{
	uint8_t alloc;/*Is this allocated*/
	uint8_t namelen;/*Length of the name of this file*/
	uint8_t name[80];/*Name of this file*/
};
/*
*struct tree_dirhdr
*Placed at offset sizeof(struct tree_ent) + sizeof(struct tree_filehdr) Only present in dirs
*/
struct tree_dirhdr{
	uint32_t nxtTreeLba;/*The lba of the start of the dir(nxtLba contains the lba of the next entry of the parent dir)*/
};
/*
*File descriptor structure
*Located at 0x0A000000 in memory
*Used when doing I/O on primitive files
*/
struct fd{
	uint8_t alloc;/*Allocated(useful when checking to see what number to assign)*/
	uint8_t type;/*Flags for the file(2nd bit is write 1st bit is read)*/
	uint8_t name[80];/*Name of file*/
	uint32_t pos;/*Position in file*/
	uint32_t tLba;/*Starting lba of file*/
};
#ifndef BUILD_HOST
#define O_RDONLY 0b01/*Read Only*/
#define O_WRONLY 0b10/*Write Only*/
#define O_RDWR 0b11/*Read And Write*/
/*
*struct __DIR typedef DIR
*Useful when using directories
*/
typedef struct __DIR{
	struct tree_ent *ent;
	struct tree_filehdr *fhdr;
	struct tree_dirhdr *dhdr;
}DIR;
/*
*int open(const char *fname,int options)
*fname is the path to the file
*options is the flags for the file
*returns a file descriptor or -1 on error
*/
int open(const char *fname,int options);
/*
*struct fd *getInfo(const char *str)
*getInfo is an internal function do not use it outside of the driver
*Used to get info on a file and returns a file descriptor structure
*/
struct fd *getInfo(const char *str);
/*
*int mkfs();
*Makes a simpfs filesystem on the hard disk at whatever address is stored in 0x501 in memory
*/
int mkfs();
/*
*int mkdir(const char *name)
*name is the path to the dir
*returns 1 on success or 0 on error
*/
int mkdir(const char *name);
/*
*DIR *opendir(const char *name);
*Returns directory structure or 0 on error
*/
DIR *opendir(const char *name);
#else
struct __DIR{
	struct tree_ent *ent;
	struct tree_filehdr *fhdr;
	struct tree_dirhdr *dhdr;
};
#endif
#endif
