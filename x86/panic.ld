ENTRY(_entry)
SECTIONS{
	. = 0x20000000;
	.text BLOCK(4K) : ALIGN(4K){
		*(.text);
	}
}
