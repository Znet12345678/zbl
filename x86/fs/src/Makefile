CC=i386-elf-gcc
CFLAGS=-nostdlib -ffreestanding -std=gnu99 -I../..
LDFLAGS=-lk -L../.. -nostdlib -T../../linker.ld
CRTBEGIN=$(shell $(CC) -print-file-name=crtbegin.o)
CRTEND=$(shell $(CC) -print-file-name=crtend.o)
LD=i386-elf-gcc
all:
	${CC} -c ls.c -o ls.o ${CFLAGS}
	${CC} -c init.c -o init.o ${CFLAGS}
	${CC} -c boot.S -o boot.o ${CFLAGS}
	${CC} ls.o -o ../exec/ls.elf ${LDFLAGS}
	${LD} -Tlinker.ld boot.o  init.o -o ../init.elf -L../.. -lk -nostdlib
