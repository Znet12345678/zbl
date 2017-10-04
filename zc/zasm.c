#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include "zexec.h"
#include "libasm.h"
int gsize(int *pntr,int n){
	int ret = 0;
	for(int i = 0; i < n;i++)
		ret+=pntr[i];
	return ret;
}
int symsize(struct __zexec_symtab *symtab,int n){
	int ret = 0;
	for(int i = 0; i < n;i++)
		ret+=symtab[i].size;
	return ret;
}
int main(int argc,char *argv[]){
	if(argc != 4)
		return -1;
	if(strcmp(argv[1],"bin") == 0){
		FILE *f = fopen(argv[2],"r");
		char *str = malloc(1024);
		FILE *out = fopen(argv[3],"wb");
		if(!f || !out)
			return -1;
		while(fgets(str,1024,f)){
			if(strcmp(str,"\n") == 0)
				continue;
			str[strlen(str) - 1] = 0;
			if(strncmp(str,"symb ",5) == 0)
				continue;
			uint8_t* val = assemble_line(str);
			if(val == 0){
				printf("Error\n");
				return -1;
			}
			if(val[0] >= 0x50 && val[0] < 0x60){
				fwrite(val,sizeof(uint8_t),1,out);
				continue;
			}if(val[0] == 0xff ){
				if(val[1] != 0x66)
					fwrite(val + 1,sizeof(uint8_t),3,out);
				else
					fwrite(val + 1,sizeof(uint8_t),6,out);
				continue;
			}if(val[0] == 0xe8){
				fwrite(val,sizeof(uint8_t),5,out);
				continue;
			}
//			printf("[%.2x %.2x]\n",val << 8,val);
			if(val[0] == 0x66)
				fwrite(val,sizeof(uint8_t),3,out);
			else
				fwrite(val,sizeof(uint8_t),2,out);
		}
		fclose(f);
		fclose(out);
		free(str);
	}else if(strcmp(argv[1],"zexec") == 0){
		FILE *f = fopen(argv[2],"r");
		FILE *out = fopen(argv[3],"wb");
		if(!f || !out)
			return -1;
		uint8_t *data = malloc(102400);
		char *line = malloc(512);
		struct __zexec_symtab *symtab = malloc(sizeof(*symtab)*1024);
		int i = 0;
		struct __zexec_hdr *hdr = malloc(sizeof(*hdr));
		int bytes = 0;
		int p = 1;
		int nphdr = 1;
		while(fgets(line,512,f)){
			if(strcmp(line,"\n") == 0)
				continue;
			line[strlen(line) - 1] = 0;
			if(strncmp(line,"symb ",5) == 0)
				continue;
			uint8_t *val = assemble_line(line);
			char *name = line + 5;
			if(val[0] == 0xe8 && (name[0] != '0' && name[1] != 'x' && name[strlen(name) - 1] != 'h'))
				nphdr++;
		}
		struct __zexec_phdr *phdr = malloc(nphdr*sizeof(*phdr));
		fseek(f,0,SEEK_SET);
		free(line);
		line = malloc(512);
		int *end = malloc(1024);
		int eindx = 0;
		int *sizes = malloc(1024);
		int lst_indx = 0;
		while(fgets(line,512,f)){
			if(strcmp(line,"\n") == 0)
				continue;
			line[strlen(line) - 1] = 0;
			if(strncmp(line,"symb ",5) == 0){	
				symtab[i].alloc = 1;
				symtab[i].namelen = strlen(line + 5);
				symtab[i].symLookup = 0;
				memcpy(symtab[i].symname,line + 5,strlen(line + 5));
				symtab[i].vmem_addr = 0;
				symtab[i].rawd_offset = sizeof(*hdr) + sizeof(*phdr) * nphdr + bytes;
				symtab[i].phdr = 0;
				if(i > 0){
					symtab[lst_indx].size = bytes - symsize(symtab,i - 1);
				}
				lst_indx = i;
				i++;
				continue;
			}
			uint8_t *val = assemble_line(line);
			char *name = line + 5;
			if(val[0] == 0xe8 && name[0] != '0' && name[1] != 'x' && name[strlen(name) - 1] != 'h'){
				symtab[i].alloc = 1;
				symtab[i].namelen = strlen(line + 5);
				symtab[i].symLookup = 1;
				memcpy(symtab[i].symname,line + 5,strlen(line + 5));
				symtab[i].vmem_addr = 0;
				symtab[i].rawd_offset = sizeof(*hdr) + sizeof(*phdr) * nphdr + bytes;
				symtab[i].phdr = p;
				//if(i > 0)
				//	symtab[i - 1].size = bytes - symsize(symtab,i - 1);
				symtab[i].size = 0;
				i++;
				phdr[p].alloc = 1;
				phdr[p].type = TYPE_DYNAM;
				phdr[p].vmem_addr = 0;
				phdr[p].rawd_offset = sizeof(*hdr) + sizeof(*phdr) * nphdr + bytes;
				end[eindx] = eindx == 0 ? bytes : bytes - gsize(end,eindx);
				eindx++;
				p++;
				memcpy(data + bytes,val,5);
				bytes+=5;
				continue;
			}else if(val[0] == 0xe8){
				memcpy(data + bytes,val,5);
				bytes+=5;
				continue;
			}
			if(val == 0){
				printf("Error\n");
				return -1;
			}
			if(val[0] >= 0x50 && val[0] < 0x60){
				memcpy(data + bytes,val,1);
				bytes++;
				continue;
			}
			if(val[0] == 0xff){
				if(val[1] == 0x66){
					memcpy(data + bytes,val + 1,6);
					bytes+=6;
				}else{
					memcpy(data + bytes,val + 1,3);
					bytes+=3;
				}
				continue;
			}
			if(val[0] == 0x66){
				memcpy(data + bytes,val,3);
				bytes+=3;
			}else{
				memcpy(data + bytes,val,2);
				bytes+=2;
			}
		}
		hdr->sig = 0x7f| 'e' << 8 | 'x' << 16 | 'e' << 24;
		hdr->arch = 32;
		hdr->sys = 0;
		hdr->entry = 0;
		hdr->nphdr = nphdr;
		hdr->phdr_offset = sizeof(*hdr);
		hdr->nsymtab = i;
		hdr->symtab_offset = hdr->phdr_offset + bytes + sizeof(struct __zexec_phdr) * hdr->nphdr;
		phdr[0].alloc = 1;
		phdr[0].type = TYPE_LOAD;
		phdr[0].vmem_addr = 0;
		phdr[0].rawd_offset = hdr->phdr_offset + sizeof(*phdr) * hdr->nphdr;
		for(int i = 0; i < nphdr - 1;i++){
			phdr[i].size = end[i];
		}
		phdr[nphdr - 1].size = bytes - gsize(end,eindx);
		fwrite(hdr,sizeof(uint8_t),sizeof(*hdr),out);
		fwrite(phdr,sizeof(uint8_t),sizeof(*phdr)*nphdr,out);
		fwrite(data,sizeof(uint8_t),bytes,out);
		symtab[lst_indx].size = bytes - symsize(symtab,hdr->nsymtab - 1); 
		fwrite(symtab,sizeof(uint8_t),sizeof(*symtab)*hdr->nsymtab,out);
		free(data);
		fclose(f);
		fclose(out);
	}
}
