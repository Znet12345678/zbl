#ifndef __SYS_TIME_H
#define __SYS_TIME_H
#define TIME
#include <sys/types.h>
typedef struct timeval {
  long tv_sec;
  long tv_usec;
} timeval;
#endif
