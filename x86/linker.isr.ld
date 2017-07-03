ENTRY(hwint)
SECTIONS{
	. = 0x00F00000;
	.text BLOCK(4K) : ALIGN(4K){
		*(.text)
	}
}
