global _start
extern main
_start:mov ah,0x0e
mov al,'.'
int 0x10
jmp main
