
stack_top:
.section .text
.global _start
.global return
BOOTSEG = 0x07c0
_start:
	call main
	ret
hng:JMP hng
