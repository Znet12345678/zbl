[bits 32]
global hwint
extern helloworld
extern t_readvals
extern t_writevals
extern write_int
extern open
extern dummy
extern close
extern kprintf
extern lseek
extern exec
extern read
extern malloc
extern t_putc
align 4
hwint:pushad
cld
call t_readvals
popad
cmp ah,0
jz hw
cmp ah,1
jz pr
cmp ah,2
jz _open
cmp ah,3
jz _close
cmp ah,4
jz _exec
cmp ah,5
jz _lseek
cmp ah,6
jz _print
cmp ah,7
jz _read
cmp ah,8
jz _t_readvals
cmp ah,9
jz _t_writevals
cmp ah,10
jz imalloc
cmp ah,11
jz _t_putc
iret
hw:call helloworld
call t_writevals
iret
pr:
push ebp
mov ebp,esp
xor ah,ah
push eax
call write_int
nop
leave
call t_writevals
iret
_open:push ebp
mov ebp,esp
push edx
push ecx
push ebx
call open
mov [ebp-4],eax
nop
leave
call t_writevals
iret
_close:push ebp
mov ebp,esp
push ebx
call close
mov [ebp-4],eax
nop
leave
call t_writevals
iret
_exec:push ebp
mov ebp,esp
push edx
push ecx
push ebx
call exec
nop
leave
call t_writevals
iret
_lseek:push ebp
mov ebp,esp
push edx
push ecx
push ebx
call lseek
nop
leave
call t_writevals
iret
_print:push ebp
mov ebp,esp
push ebx
call kprintf
nop
leave
call t_writevals
iret
_read:push ebp
mov ebp,esp
push edx
push ecx
push ebx
call read
nop
leave
call t_writevals
iret
_t_readvals:
call t_readvals
iret
_t_writevals:
call t_writevals
iret
_t_putc:
push ebp
mov ebp,esp
push ebx
call t_putc
nop
leave
call t_writevals
iret
imalloc:push ebp
mov ebp,esp
push ebx
call malloc
mov [ebp-4],eax
nop
leave
call t_writevals
iret
