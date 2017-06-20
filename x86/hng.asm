bits 32
org 0x00007E00
hng:jmp hng
times 510 - ($ - $$) db 0
