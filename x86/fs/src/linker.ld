ENTRY(_start)
SECTIONS{
	. = 1M;
	.TEXT BLOCK(4K):ALIGN(4K){
		*(.text)
	}		
}
