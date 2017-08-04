ENTRY(_entry)
SECTIONS{
	. = 0x05000000;
	.text BLOCK(4K) : ALIGN(4K){
		*(.text);
	}
}
