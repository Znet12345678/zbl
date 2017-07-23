/*
*kprintf function
*libio
*(c) 2015 zachary james schlotman
*/
#include <stdarg.h>
#include <kernel.h>
#include <string.h>
#include <stdbool.h>
int intlen(int n){
	int ret = 0;
	while(n > 0){
	//	ret++;
		n/=10;
		ret++;
	}
	return ret;
}
static void _print(const char *str,unsigned long len){
        int i = 0;
        while(i < len){
                t_putc((int)((const unsigned char *)str)[i]);
                i++;
        }
}

void printf(const char *format,...){
	va_list args;
	va_start(args,format);
	unsigned long amount;
	bool bad = false;
	while(* format != '\0'){
		if(*format != '%'){
			print_c:
			amount = 1;
			while(format[amount] && format[amount] != '%')
				amount++;
			_print(format,amount);
			format+=amount;
			continue;
		}
		const char *begin = format;
		if(*(++(format)) == '%')
			goto print_c;
		if(bad){
			badconv:
				bad = true;
				format = begin;
				goto print_c;
		}
		if(*format == 'c'){
			format++;
			char c = (char)va_arg(args,int);
			print(&c);
		}
		else if(*format == 's'){
			format++;
			const char *str = (const char *)va_arg(args,const char *);
			print(str);
		}
		else if(*format == 'd'){
			format++;
			int j = (int)va_arg(args,int);
			if(j == 0)
				t_putc('0');
			if(j < 0){
				t_putc('-');
				j*=-1;
			}
			char buf[1024] = {[0 ... 1023]0};
			//char *buf = malloc(intlen(buf));
			//char buf[1024] = {[0 ... 1023]0};
			//char buf[1024] = {[0 ... 1023]0};
			int i = intlen(j) - 1;
			while(j > 0){
				int _buf = (j%10);
				if(_buf == 0)
					buf[i] = '0';
				else if(_buf == 1)
					buf[i] =  '1';
	                        else if(_buf == 2)
                                        buf[i] = '2';
                                else if(_buf == 3)
                                        buf[i] = '3';
                                else if(_buf == 4)
                                        buf[i] = '4';
                                else if(_buf == 5)
                                        buf[i] = '5';
                                else if(_buf == 6)
                                        buf[i] = '6';
                                else if(_buf == 7)
                                        buf[i] = '7';
                                else if(_buf == 8)
                                        buf[i] = '8';
                                else if(_buf == 9)
                                        buf[i] = '9';

				i--;
				j/=10;
			}
			i = 0;
			while(i < strlen(buf)){
				t_putc(buf[i]);
				i++;
			}

		}else if(*format == 'f'){
			format++;
			float j = (float)va_arg(args,float);
			int n = (int)j;
			float j2 = j - n;
			j2*=5;
			printf("%d.%d",j,j2);
		}
		else
			goto badconv;
		
	}
	va_end(args);
	//place_cursor();
}
void debug(const char *proccess,const char *output){
	printf("[%s](debug):%s\n",proccess,output);
}
