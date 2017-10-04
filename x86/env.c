#include "lib.h"
#include "env.h"
void setenv(const char *name,const char *val){
	struct env *e = (struct env *)0x502;
	while(e->alloc){
		if(strcmp(e->name,name) == 0){
			e->val = malloc(strlen(val));
			strcpy(e->val,val);
		}
		e+=sizeof(*e);
	}
	e->alloc = 1;
	e->name = malloc(strlen(name));
	strcpy(e->name,name);
	e->val = malloc(strlen(val));
	strcpy(e->val,val);
}
char *getenv(const char *name){
	int i = 0;
	struct env *e = (struct env *)0x502;
	while(e->alloc){
		if(strcmp(e->name,name) == 0)
			return e->val;
		e+=sizeof(*e);
	}
	return 0;
}
