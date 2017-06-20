[BITS 32]
[ORG 0x00100000]
_start:mov byte [0xb8000],'I'
mov byte [0xb8002],'t'
mov byte [0xb8004],' '
mov byte [0xb8006],'W'
mov byte [0xb8008],'o'
mov byte [0xb800A],'r'
mov byte [0xb800C],'k'
mov byte [0xb800E],'s'
hng:jmp hng
