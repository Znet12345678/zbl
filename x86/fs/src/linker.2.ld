ENTRY(_start)
SECTIONS{
        . = 0x00100000;
        .TEXT BLOCK(4K):ALIGN(4K){
                *(.text)
        }
}
