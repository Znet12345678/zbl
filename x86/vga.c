#include "lib.h"
#include "vga.h"
static void putpixel(unsigned char* screen, int x,int y, int color) {
    unsigned where = x + y*320;
    screen[where] = color & 255;              // BLUE
    screen[where + 1] = (color >> 8) & 255;   // GREEN
    screen[where + 2] = (color >> 16) & 255;  // RED
}
unsigned char *vgabuf = (unsigned char *)0xA0000;
unsigned char alph[]={0b0110,0b0101,0b01111,0b1001,0b1001,0b1001,0b1111,0b0010,0b0010,0b0100,0b1111};
int vga_putc(int c,int x,int y){
	int index = 0;
	if(c >= 'a' && c <= 'z')
		index = 6 * (c - 'a');
	for(int i = 0; i < 6;i++,index++){
		for(int j = 0; j < 4;j++){
			if((alph[index] >> j) & 1){
				putpixel(vgabuf,x * 4 + j,y*6 + i,COLOUR_WHITE);
			}
		}
	}
}
