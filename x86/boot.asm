[bits 32]
section .text
extern main
global _start
section .bss
stack_top:resb 409600
stack_bottom:
section .text
_start:mov esp,0x01000000
jmp main
