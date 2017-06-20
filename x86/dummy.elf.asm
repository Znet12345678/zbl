global _start
_start:mov byte [0xb8000],'!'
times 510 - ($ - $$) db 0
