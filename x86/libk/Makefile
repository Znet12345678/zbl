CFLAGS=-I.. -nostdlib -ffreestanding -DKERNEL -fmax-errors=1 -DDEBUG -std=gnu99
all:
	gcc -m32 -c fs.c -o fs.o ${CFLAGS}
	gcc -m32 -c str.c -o str.o ${CFLAGS}
	gcc -m32 -c err.c -o err.o ${CFLAGS}
	gcc -m32 -c io.c -o io.o ${CFLAGS}
