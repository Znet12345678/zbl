#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include "libasm.h"
#include <math.h>
uint32_t jmpaddr(uint32_t addr){
	return (addr - 4) | (addr - 4) >>  8|(addr - 4) >> 16 | (addr - 4) >> 24;

}
char **sep(const char *str,char c){
	int i = 0,j = 0;
	char **ret = malloc(4096);
	ret[0] = malloc(80);
	int indx = 0;
	while(i < strlen(str)){
		if(str[i] == c){
			while(str[i] == c)
				i++;
			indx++;
			j = 0;
			ret[indx] = malloc(80);
			continue;
		}
		ret[indx][j] = str[i];
		i++;
		j++;
	}
	ret[indx+1] = 0;
	return ret;
}
uint16_t num(const char *str){
	if(str[0] == '\'' && str[strlen(str)-1] == '\'')
		return str[1];
	if(str[0] == '0' && str[1] == 'x'){
		int exp = strlen(str)-3;
		int ret = 0;
		for(int i = 2;i < strlen(str);i++,exp--){
			if(str[i] >= '0' && str[i] <= '9') 	
				ret+=(str[i]-'0') * (int)pow(16,exp);
			else if(str[i] >= 'A' && str[i] <= 'B')
				ret+=(str[i]-'A' + 10) * (int)pow(16,exp);
			else if(str[i] >= 'a' && str[i] <= 'b')
				ret+=(str[i]-'a' + 10) *(int)pow(16,exp);
		}
		return ret;
	}if(str[strlen(str) - 1] == 'h'){
		int exp = strlen(str)-2;
		int ret = 0;
		for(int i = 0; i < strlen(str)-1;i++,exp--){
			if(str[i] >= '0' && str[i] <= '9')
				ret+=(str[i]-'0') * (int)pow(16,exp);
			else if(str[i] >= 'A' && str[i] <= 'B')
                                ret+=(str[i]-'A' + 10) * (int)pow(16,exp);
                        else if(str[i] >= 'a' && str[i] <= 'b')
                                ret+=(str[i]-'a' + 10) *(int)pow(16,exp);

		}
		return ret;
	}
	return atoi(str);
}
uint8_t *gen2Bytes(uint8_t direction,uint8_t size,uint8_t rsize,uint8_t opcode,uint8_t mode,uint8_t reg,uint8_t regmem){
	uint8_t *ret = malloc(3);
	ret[1] = ((mode & (1 << 1)) >> 1) << 7 | (mode & 1) << 6 | ((reg & (1 << 2)) >> 2) << 5 | ((reg & ( 1 << 1)) >> 1) << 4 | (reg &  1) << 3 | regmem;
	ret[0] = opcode | (direction & 1) << 1 | size & 1;
	if(rsize == 1){
		ret[2] = ret[1];
		ret[1] = ret[0];
		ret[0] = 0x66;
	}
	return ret;
}
uint8_t genOP(uint8_t opcode,uint8_t direction,uint8_t size){
	return opcode | (direction & 1) << 1 | size &1;
}
uint8_t reg(const char *str){
	if(strcmp(str,"al") == 0 || strcmp(str,"ax") == 0 || strcmp(str,"eax") == 0)
		return 0;
	if(strcmp(str,"cl") == 0 || strcmp(str,"cx") == 0 || strcmp(str,"ecx") == 0)
		return 1;
	if(strcmp(str,"dl") == 0 || strcmp(str,"dx") == 0 || strcmp(str,"edx") == 0)
		return 2;
	if(strcmp(str,"bl") == 0 || strcmp(str,"bx") == 0 || strcmp(str,"ebx") == 0)
		return 3;
	if(strcmp(str,"ah") == 0 || strcmp(str,"sp") == 0 || strcmp(str,"esp") == 0)
		return 4;
	if(strcmp(str,"ch") == 0 || strcmp(str,"ebp") == 0 || strcmp(str,"bp") == 0)
		return 5;
	if(strcmp(str,"dh") == 0 || strcmp(str,"si") == 0 || strcmp(str,"esi") == 0)
		return 6;
	if(strcmp(str,"bh") == 0 || strcmp(str,"di") == 0 || strcmp(str,"edi") == 0)
		return 7;
	return 8;
}	

uint8_t size(const char *str){
	if(str[1] == 'h' || str[1] == 'l')
		return 0;
	if(str[1] == 'x')
		return 1;
	if(str[2] == 'x')
		return 1;
	return 0;
}
uint8_t rsize(const char *str){
	if(str[2] == 'x')
		return 2;
	return size(str);
}
uint8_t *assemble_line(const char *str){
	char **arr = sep(str,' ');
	int n3 = 0;
	while(arr[n3] != 0)
		n3++;
	if(n3 < 2)
		return 0;
	char **arr2 = sep(arr[1],',');
	int n = 0;
	while(arr2[n] != 0)
		n++;
	int n2 = 0;
	while(arr[n2] != 0)
		n2++;
	if(strcmp(arr[0],"call") == 0){
		uint8_t *ret = malloc(5);
		if(arr[1][0] == '0' && arr[1][1] == 'x' || arr[1][strlen(arr[1]) - 1] == 'h'){
			uint32_t n = num(arr[1]);
			if(n < 4){
				ret[0] = 0xe8;
				ret[1] = 0xfc + n;
				ret[2] = 0xff;
				ret[3] = 0xff;
				ret[4] = 0xff;
			}else{
				ret[0] = 0xe8;
//				ret[1] = n - 4 != 0 ? n - 4 : 0;
				ret[1] = (n - 4);
				ret[2] = (n - 4) >> 8;
				ret[3] = (n - 4) >> 16;
				ret[4] = (n - 4) >> 24;
//				ret[2] = n >> 8 != 0 ? (n >> 8) - 1 : 0;
//				ret[3] = n >> 16 != 0 ? (n >> 16) - 1 : 0;
//				ret[4] = n >> 24 != 0 ? (n >> 24) - 1 : 0;
				return ret;
			}
		}
		ret[0] = 0xe8;
		ret[1] = 0xfc;
		ret[2] = 0xff;
		ret[3] = 0xff;
		ret[4] = 0xff;
		return ret;
	}
	if(strcmp(arr[0],"push") == 0){
                uint8_t _ret = 0x50 + reg(arr2[0]);
                uint8_t *ret = malloc(1);
                memcpy(ret,&_ret,1);
                return ret;
        }if(strcmp(arr[0],"pop") == 0){
                uint8_t _ret = 0x58 + reg(arr2[0]);
                uint8_t *ret = malloc(1);
                memcpy(ret,&_ret,1);
                return ret;
        }if(strcmp(arr[0],"int") == 0){
		uint8_t *ret = malloc(5);
		ret[0] = genOP(0xcd,0,0);
		uint8_t val = num(arr2[0]);
		memcpy(ret + 1,&val,1);
		return ret;
	}
	if(n2 < 2 || n < 2)
		return 0;
	if(reg(arr2[1]) == 8){
		uint8_t *ret = malloc(5);
		ret[0] = 0xff;
		if(strcmp(arr[0],"mov") == 0){
			ret[1] = genOP(0xb8,0,0);
			uint16_t val = num(arr2[1]);
			memcpy(ret + 2,&val,sizeof(uint16_t));
			if(rsize(arr2[0]) == 2){
				ret[5] = ret[4];
				ret[4] = ret[3];
				ret[3] = ret[2];
				ret[2] = ret[1];
				ret[1] = 0x66;
			}
			return ret;
		}else if(strcmp(arr[0],"movb") == 0){
			ret[1] = genOP(0xc6,0,0);
			uint16_t val = num(arr2[1]);
			memcpy(ret + 2,&val,sizeof(uint16_t));
		}
	}
	if(strcmp(arr[0],"add") == 0)
		return gen2Bytes(0,size(arr2[0]),rsize(arr2[0]),0,0b11,reg(arr2[1]),reg(arr2[0]));
	if(strcmp(arr[0],"mov") == 0)
		return gen2Bytes(0,size(arr2[0]),rsize(arr2[0]),0x88,0b11,reg(arr2[1]),reg(arr2[0]));
	return 0;

}

