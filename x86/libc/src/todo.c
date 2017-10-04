#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <kernel.h>
int fgets(void *buf,size_t n,FILE *f){
	printf("fgets(...): TODO\n");
	return -1;
}
int putc(int c,FILE *f){
	printf("putc(...): TODO\n");
	return -1;
}
int fputc(int c,FILE *f){
	printf("fputc(...): TODO\n");
	return -1;
}
int fclose(FILE *f){
	printf("fclose(...): TODO\n");
	return -1;
}
int feof(FILE *f){
	printf("feof(...): TODO\n");
	return -1;
}
FILE *fopen(const char *path,const char *mode){
	printf("fopen(...): TODO\n");
	return 0;
}
size_t fwrite(const void *pntr,size_t size,size_t nb,FILE *f){
	printf("fwrite(...): TODO\n"); 
	return 0;
}
size_t fread(void *pntr,size_t size,size_t nb,FILE *f){
	printf("fread(...): TODO\n");
	return 0;
}
int sprintf(char *buf,const char *format,...){
	printf("spritnf(...): TODO\n");
	return -1;
}
FILE *stdin;
FILE *stdout;
FILE *stderr;
void *bsearch(const void *key, const void *base,
                     size_t nmemb, size_t size,
                     int (*compar)(const void *, const void *)){

}
void *realloc(void *pntr,size_t s){
	return malloc(s);
}
//char *getenv(const char *name){
//	int i = 0;
//	char **env = (char**)0x502;
//	while(strcmp(env[i],name) != 0 && env[i] != 0)
//		i++;
//	return env[i];
//}

