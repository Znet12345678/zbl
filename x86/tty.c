#include "lib.h"
enum vga_colour{
        COLOUR_BLACK = 0,
        COLOUR_BLUE = 1,
        COLOUR_GREEN = 2,
        COLOUR_CYAN = 3,
        COLOUR_RED = 4,
        COLOUR_MAGENTA = 5,
        COLOUR_BROWN = 6,
        COLOUR_LIGHT_GREY = 7,
        COLOUR_DARK_GREY = 8,
        COLOUR_LIGHT_BLUE = 9,
        COLOUR_LIGHT_GREEN = 10,
        COLOUR_LIGHT_CYAN = 11,
        COLOUR_LIGHT_RED = 12,
        COLOUR_LIGHT_MAGENTA = 13,
        COLOUR_LIGHT_BROWN = 14,
        COLOUR_WHITE = 15,
};
static inline uint8_t mkcolour(enum vga_colour fg,enum vga_colour bg){
        return fg | bg << 4;
}
static inline uint16_t mkent(char c,uint8_t colour){
        uint16_t c16 = c;
        uint16_t colour16 = colour;
        return c16 | colour16 << 8;
}

uint16_t *vga = (uint16_t*)0xB8000;
uint16_t *tbuff;
int tcolour;
int x = 0,y = 0;
void t_init(){
	x = 0;
	y = 0;
	//tcolour = mkcolour(COLOUR_WHITE,COLOUR_BLUE);
	tcolour = mkcolour(COLOUR_BLACK,COLOUR_LIGHT_GREY);
	tbuff = vga;
	 for(int y = 0; y < 25;y++){
        	for(int x = 0;x < 80;x++){
        	    const unsigned long index = y * 80 + x;
        	    tbuff[index] = mkent(' ',tcolour);
	        }
    }

}

void t_putc(char c){
//	vga_putc(c,y,x);
//	if(c == '\n' || x == 80){
//		y++;
//		x=  0;
//	}else
//		x++;
		
//	return;
	if(x == 80 || c == '\n'){
		x = 0;
		if(y == 24){
			scroll();
		}else
			y++;
	}
	if(c != '\n')
		vga[y * 80 + x] = mkent(c,tcolour);
	if(x != 80 && c != '\n' && c != 0)
		x++;
	outb(0x3d4,0x0f);
	outb(0x3d5,((y * 80 + x) & 0xff));
	outb(0x3d4,0x0e);
	outb(0x3d5,((y * 80 + x >> 8) & 0xff));

}
void scroll(){
	for(int i = 0; i < 24;i++)
		for(int j = 0; j < 80;j++)
			vga[i * 80 + j] = vga[(i + 1) * 80 + j];
        for(int i = 0; i < 80;i++)
                vga[24 * 80 + i] = mkent(' ',tcolour);
	x = 0;
}
void t_writevals(){
	uint8_t *pntr = (uint8_t*)0x00000510;
	*pntr = (uint8_t)x;
	*pntr++;
	*pntr = (uint8_t)y;
	*pntr++;
}
