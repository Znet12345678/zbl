BITS 16
ORG 0x7c00
jmp a;Skip the signature
db 0x1f;Signature for installation media
db 0xaf
db 0x0f
db 0x9f
a:mov ah,0x0e
mov al,'.'
int 0x10
global err
global loop1
extern extern_cmp
mov ah,2;Parimater to read from disk
mov al,1;Number of sectors
mov ch,0;Cylinder number
mov cl,2;Sector Number
mov dh,0;Head Number
mov dl,0;drive number
mov bx,0x1000;Buffer pointer
loop1:
int 0x13;BIOS Interrupt to read disk
cmp ah,0;check return code
jz check;jump to check for the boot sig
mov ah,2;restore parameters
mov al,1
mov ch,0
mov cl,2
mov dh,0
jmp drive
re:
;inc dl;incriment drive counter
jmp loop1;loop
drive:cmp dl,0
jz floppy2
cmp dl,1
jz hda
cmp dl,0x80
jz hdb
cmp dl,0x81
mov ah,0x0e
mov al,'E'
int 0x10
__hng:jmp __hng
floppy2:mov dl,1
jmp re
hda:mov dl,0x80
jmp re
hdb:mov dl,0x81
jmp re
check:
jmp check1
check1:
cmp byte [0x1000],0x0f;compare first byte
jz check2;continue to compare second byte
jmp loop1;go back if sig not found
check2:cmp byte [0x1001],0x1f;compare second byte
jz check3;continue to compare 3rd byte
jmp loop1;go back if sig not found
check3:cmp byte[0x1002],0xaa;compare 3rd byte
jz  ldr;jump to loader
jmp loop1;go back if sig not found
ldr:mov ah,0x0e;BIOS interrupt for 10h to print char
mov al,'.';char to print
int 0x10;BIOS intterupt to print char
mov [0x9000],dl;store drive in 2 locations in memory
mov [0x500],dl
jmp 0:0x1003;long jump to the code to transition to protected mode
err:;print error message
mov ah,0x0e
mov al,'E'
int 0x10
mov al,'R'
int 0x10
mov al,'R'
int 0x10
mov al,'O'
int 0x10
mov al,'R'
int 0x10
hng:jmp hng;loop forever
nxt:mov ah,2
mov al,1
mov ch,0
mov cl,2
mov dh,0
cmp dl,0
jz _fdb
cmp dl,1
jz _hda
cmp dl,0x80
jz _hdb
mov dl,0x80
jmp loop1
_fdb:mov dl,1
jmp loop1
_hda:mov dl,0x80
jmp loop1
_hdb:mov dl,0x81
jmp loop1
times 510 - ($ - $$) db 0
dw 0xaa55
