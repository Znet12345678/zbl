#ifndef __TIME_H
#define __TIME_H
#define TIME
#include <sys/types.h>
typedef struct timeval {
  long tv_sec;
  long tv_usec;
} timeval;
#endif
