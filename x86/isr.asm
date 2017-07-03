[bits 32]
global hwint
extern helloworld
extern t_readvals
extern t_writevals
align 4
hwint:pushad
cld
call t_readvals
popad
cmp ah,0
jz hw
iret
hw:call helloworld
call t_writevals
iret
