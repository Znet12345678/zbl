#ifndef __SYS_STAT_H
#define __SYS_STAT_H
#include <sys/types.h>
#include <time.h>
struct stat {
	mode_t			st_mode;
	ino_t			st_ino;
	dev_t			st_dev;
	dev_t			st_rdev;
	nlink_t			st_nlink;
	uid_t			st_uid;
	gid_t			st_gid;
	off_t			st_size;
	blksize_t		st_blksize;
	blkcnt_t		st_blocks;
	time_t			st_mtime;		
};
#endif
