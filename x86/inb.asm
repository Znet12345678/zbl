[bits 32]
global inb
global outb
inportb:
mov dx,[esp+4]
in byte al,dx
ret
outportb:mov dx,[esp+4]
mov al,[esp+8]
out dx,al
ret
