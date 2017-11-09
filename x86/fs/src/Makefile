CC=i386-elf-gcc
CFLAGS=-nostdlib -ffreestanding -std=gnu99 -I../..
ALTCFLAGS=-nostdlib -ffreestanding -std=gnu99
LDFLAGS=-lk -L../.. -nostdlib -T../../linker.ld -lgcc
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
	${CC} -c nlldr.c -o nlldr.o ${CFLAGS}
	i386-zbl-gcc -c dev.c -o dev.o ${ALTCFLAGS}
	i386-zbl-gcc -c libctest.c -o libctest.o ${ALTCFLAGS}
	i386-zbl-gcc -c crash.c -o crash.o ${ALTCFLAGS}
	${LD} -Tlinker.2.ld boot.o help.o -o ../exec/help.elf -L../.. -lk -nostdlib
	${LD} -Tlinker.2.ld boot.o ls.o -o ../exec/ls.elf -L../.. -lk -nostdlib
	${LD} -Tlinker.ld boot.o  init.o -o ../init.elf -L../.. -lk -nostdlib
	${LD} -Tlinker.2.ld boot.o echo.o -o ../exec/echo.elf -L../.. -lk -nostdlib
	${LD} -Tlinker.2.ld boot.o cat.o -o ../exec/cat.elf -L../.. -lk -nostdlib
#	${LD} -Tlinker.2.ld boot.o nlldr.o -o ../exec/nlldr.elf -L../.. -lk -nostdlib
#	i386-zbl-gcc newlibtest.c -o ../exec/newlibtest.elf
	i386-zbl-gcc  libctest.o -o ../exec/libctest.elf -L../libc/src -lc -lgcc
	i386-zbl-gcc  dev.o -o ../exec/dev.elf -lc -lgcc
	i386-zbl-gcc crash.o -o ../exec/crash.elf -lc -lgcc
	@make -C ../kernel/src
	@make -C ../kernel/modules
	${CC} -c pmdos.c -o pmdos.o ${CFLAGS} 
	${CC} -Tlinker.2.ld boot.o pmdos.o -o ../exec/pmdos.elf -L../.. -lk -nostdlib -lgcc
