bits 32
org 0x00007E00
mov byte [0xb8000],'x'
hng:jmp hng
times 510 - ($ - $$) db 0
