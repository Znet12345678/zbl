
#include <stdint.h>
unsigned long strlen(const char *str){
	int i = 0;
	while(str[i] != 0)
		i++;
	return i;
}
int main(){
	t_init();
	uint8_t *vga = (uint8_t*)0xb8000;
	const char *str = "Hello World!";
	for(int i = 0 ; i < strlen(str);i++)
		t_putc(str[i]);
	while(1);
}
