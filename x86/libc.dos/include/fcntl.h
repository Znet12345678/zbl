#ifndef __FCNTL_H
#define __FCNTL_H
#define	O_RDONLY	0x0001		/* open for reading only */
#define	O_WRONLY	0x0002		/* open for writing only */
#define	O_RDWR		0x0003		/* open for reading and writing */
#define	O_ACCMODE	0x0004		/* mask for above modes */
#define	F_DUPFD		0		/* duplicate file descriptor */
#define	F_GETFD		1		/* get file descriptor flags */
#define	F_SETFD		2		/* set file descriptor flags */
#define	F_GETFL		3		/* get file status flags */
#define	F_SETFL		4		/* set file status flags */
#define	FD_CLOEXEC	1
#define	O_CREAT		0x0200		/* create if nonexistant */
#define	O_TRUNC		0x0400		/* truncate to zero length */
#define	O_EXCL		0x0800		/* error if already exists */
#define	O_NONBLOCK	0x0004		/* no delay */
#define	O_APPEND	0x0008	
//int open(const char *path, int flags);
int read(int fd,void *buf,int n);
int write(int fd,void *buf,int n);	
int close(int fd);
#endif
