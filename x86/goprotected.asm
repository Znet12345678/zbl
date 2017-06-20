global goprotected
extern return
goprotected:
mov ah,0
mov al,0x14
int 0x10
mov ax,3
int 0x10
cli
lgdt [gdtr]
mov eax,cr0
or al,1
mov cr0,eax
mov ebx,0xB8000
mov al,'.'
mov [fs:ebx],al
ret
gdt:dd 0
dd 0
code:dw 0xffff
dw 3
db 128
db 10011010b
db 11001111b
data:dw 0xffff
dw 3
db 128
db 10010010b
db 11001111b
db 0
gdt_end:
gdtr:dw gdt_end - gdt - 1
dd gdt

