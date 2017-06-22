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
	${CC} -c help.c -o help.o ${CFLAGS}
	${CC} -c echo.c -o echo.o ${CFLAGS}
	${CC} -c cat.c -o cat.o ${CFLAGS}
	${LD} -Tlinker.2.ld boot.o help.o -o ../exec/help.elf -L../.. -lk -nostdlib
	${LD} -Tlinker.2.ld boot.o ls.o -o ../exec/ls.elf -L../.. -lk -nostdlib
	${LD} -Tlinker.ld boot.o  init.o -o ../init.elf -L../.. -lk -nostdlib
	${LD} -Tlinker.2.ld boot.o echo.o -o ../exec/echo.elf -L../.. -lk -nostdlib
	${LD} -Tlinker.2.ld boot.o cat.o -o ../exec/cat.elf -L../.. -lk -nostdlib
