#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int fgets(void *buf,size_t n,FILE *f){
	printf("fgets(...): TODO\n");
}
int putc(int c,FILE *f){
	printf("putc(...): TODO\n");
}
int fputc(int c,FILE *f){
	printf("fputc(...): TODO\n");
}
int fclose(FILE *f){
	printf("fclose(...): TODO\n");
}
int feof(FILE *f){
	printf("feof(...): TODO\n");
}
FILE *fopen(const char *path,const char *mode){
	printf("fopen(...): TODO\n");
}
size_t fwrite(const void *pntr,size_t size,size_t nb,FILE *f){
	printf("fwrite(...): TODO\n"); 
}
size_t fread(void *pntr,size_t size,size_t nb,FILE *f){
	printf("fread(...): TODO\n");
}
int sprintf(char *buf,const char *format,...){
	printf("spritnf(...): TODO\n");
}
FILE *stdin = (FILE*)0;
FILE *stdout = (FILE*)0;
FILE *stderr = (FILE*)0;
void *bsearch(const void *key, const void *base,
                     size_t nmemb, size_t size,
                     int (*compar)(const void *, const void *)){

}
void *realloc(void *pntr,size_t s){

}
