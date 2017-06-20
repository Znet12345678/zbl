#include "lib.h"
#include "libmem.h"
void mem_init(){
}
void *alloc(unsigned long size,unsigned long pos){
	struct mem_ent *mement = (struct mem_ent *)0x01000000;
	while(mement->alloc == 1)
		mement+=sizeof(*mement) + mement->size;
	mement->alloc = 1;
	void *ret;
	if(mement->size >= size || mement->size == 0){
		mement->size = size;
		mement->nxt = 0;
		memcpy(ret + pos,mement + sizeof(*mement),size);
		pos+=size;
	}else{
		uint8_t *pntr = alloc(size - mement->size,pos);
		mement->nxt = (struct mem_ent *)pntr - sizeof(*mement->nxt);
		memcpy(ret + pos,mement + sizeof(*mement),size);
		pos+=size-mement->size;
	}
	return ret;
}
void *__malloc(unsigned long size,uint32_t offset){
	struct mem_ent *mement = (struct mem_ent *)0x01000000;
	void *ret;
	char *cpntr = (char*)ret;
	while(mement->alloc == 1)
		mement=mement + sizeof(*mement) + mement->size;
	mement->alloc = 1;
	if(mement->size >= size || mement->size == 0){
		mement->size = size;
		mement->nxt = 0;
		//memcpy(ret,mement + sizeof(*mement),size);
		ret = mement + sizeof(*mement);
	}else{
		ret = mement + sizeof(*mement);
		uint8_t *pntr = __malloc(size - mement->size,offset);
		mement->nxt = (struct mem_ent *)(pntr - sizeof(*mement));
		memmove(ret + offset,mement->nxt + sizeof(*mement->nxt),mement->nxt->size);
		offset+=mement->nxt->size;
	}
	cpntr = (char*)ret;
//	for(int i = 0; i < size;i++)
//		cpntr[i] = 0;
	return ret;
}
void *malloc(unsigned long size){
	return __malloc(size,0);
}
void free(void *pntr){
	/*struct mem_ent *mement = (struct mem_ent *)0x01000000;
	while(mement->size != 0 && memcmp(mement + mement->size,pntr,mement->size) != 0)
		mement+=sizeof(*mement) + mement->size;
	mement->alloc = 0;*/
	struct mem_ent *mement = (struct mem_ent *)pntr - sizeof(*mement);
	while(mement->nxt != 0){
		mement->alloc = 0;
		mement = mement->nxt;
	}
}
void *blmalloc(unsigned long size){
	return malloc(size);
}
