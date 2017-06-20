.code16
.section .text

.global extern_cmp
extern_cmp:cmp $0x81,%dl
jg err
jmp loop1
