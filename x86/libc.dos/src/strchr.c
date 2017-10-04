#include <string.h>
char *strchr(const char *buf,int c){
	int i = 0;
	while(buf[i] != c && buf[i] != 0)
		i++;
	return (char*)(buf + i);
}
char *strrchr(const char *buf,int c){
	int i = strlen(buf) - 1;
	while(buf[i] != c && i >= 0)
		i--;
	return (char*)(buf + i);
}
