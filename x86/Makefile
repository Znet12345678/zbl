CFLAGS = -nostdlib -std=gnu99 -DKERNEL -ffreestanding -DKERNEL -DMIN_DEBUG
CC=i386-elf-gcc
CRTBEGIN=$(shell $(CC) -print-file-name=crtbegin.o)
CRTEND=$(shell $(CC) -print-file-name=crtend.o)
all:
	rm -f libk.a
	rm -f *.img
	@make -C libc install
#	make -C fs/src
#	${CC} -c syscalls.c -o syscalls.o ${CFLAGS}
#	i386-zbl-gcc newlibtest.c -o fs/exec/newlibtest.elf
	nasm -f elf isr.asm -o isr.o
	nasm -f elf idt.asm -o idt.asm.o
#	${CC} isr.o idt.asm.o -o fs/isr.ko -lk
	cc pad.c -o pad
	cc -c mkgenfs.c -o mkgenfs.o -I. -DLINUX -std=gnu99 -Werror
	cc mkgenfs.o -o mkgenfs
	${CC} -c libelf.c -o libelf.o ${CFLAGS}
	${CC} -c idt.c -o idt.o ${CFLAGS} -Werror
	i386-elf-gcc -c genfs.c -o genfs.o ${CFLAGS}
	i386-elf-gcc -c libmem.c -o libmem.o ${CFLAGS} -Werror
	i386-elf-gcc -c libzfs.c -o libzfs.o ${CFLAGS} -Werror -fmax-errors=1
	i386-elf-gcc -c prim.c -o prim.o ${CFLAGS} -Werror
	i386-elf-gcc -c fat.c -o fat.o ${CFLAGS}
	nasm -f bin null.asm -o null
	i386-elf-gcc -c crtn.S -o crtn.o ${CFLAGS}
	i386-elf-gcc -c crti.S -o crti.o ${CFLAGS}
	make -C libk
	i386-elf-gcc ${CFLAGS} -c str.c -o str.o
	i386-elf-gcc -c mem.c -o mem.o ${CFLAGS} -Werror
	nasm -f elf dummy.start.asm -o dummy.start.o
	nasm -f elf helloworld.asm -o helloworld.o
	i386-elf-gcc helloworld.o -o helloworld.elf -nostdlib
	i386-elf-gcc -c cat.c -o cat.o ${CFLAGS}
	i386-elf-gcc ${CFLAGS} -c tty.c -o tty.o
	gcc genfsfromfile.c -o genfsfromfile -std=gnu99
	i386-elf-gcc ${CFLAGS} -c kprintf.c -o kprintf.o
	i386-elf-gcc -c dummy.c -o dummy.o ${CFLAGS}
	i386-ld -Ttext 0x00100000 dummy.o tty.o -o dummy.bin --oformat binary
	i386-elf-gcc dummy.start.o dummy.o tty.o -o dummy.3.elf ${CFLAGS}
	i386-elf-gcc -c ata.c -o ata.o ${CFLAGS}
	i386-elf-gcc -c elf.c -o elf.o ${CFLAGS}
	nasm -f elf dummy.elf.asm -o dummy.elf.o
	i386-elf-gcc -nostdlib dummy.elf.o -o dummy.elf
	nasm -f bin padding.asm -o padding.bin
	nasm -f bin stage2.asm -o stage2.bin
	nasm -f bin stage1.2.asm -o stage1.2.bin
	nasm -f elf goprotected.asm -o goprotected.elf
	nasm -f elf inportb.asm -o inportb.elf
	nasm -f elf start.asm -o start.elf
	nasm -f bin stage1.asm -o stage1.bin
	nasm -f bin sig.asm -o sig.bin
	nasm -f elf init.asm -o init.elf
	nasm -f elf inb.asm -o inb.elf
	i386-elf-gcc -c lib.c -o lib.o ${CFLAGS}
	i386-elf-gcc -c stage2.c -o stage2.o ${CFLAGS} -DSTAGE2
	i386-elf-gcc -c io.c -o io.o ${CFLAGS}
	i386-elf-gcc -c stage2.S -o stage2.S.o ${CFLAGS}
	i386-elf-gcc -c blmalloc.c -o blmalloc.o ${CFLAGS}
	i386-elf-gcc -c ldr.c -o ldr.o ${CFLAGS}
	i386-ld -Ttext 0x7e00 stage2.o lib.o inb.elf io.o blmalloc.o ata.o libelf.o tty.o kprintf.o ldr.o cat.o mem.o -o stage2.2.elf
	i386-ld -Ttext 0x7e00 stage2.o lib.o inb.elf io.o blmalloc.o  ata.o libelf.o tty.o kprintf.o ldr.o cat.o mem.o -o stage2.2.bin --oformat binary
#	i386-ld -Ttext 0x8000 stage2.S.o stage2.o goprotected.elf tty.o inportb.elf -o stage2.2.elf
	objcopy -S -O binary stage2.2.elf stage2.2.bin.1
#	i386-elf-ld --oformat binary stage2.S.o stage2.o goprotected.elf tty.o inportb.elf -o stage2.2.bin -Ttext 0x8000
#	cat stage1.bin padding.bin stage2.2.bin padding.bin > bootloader.bin
	nasm -f bin hng.asm -o hng.bin
	i386-elf-gcc -c boot.S -o boot.o ${CFLAGS}
	nasm -f bin dummy.asm -o dummy.2.bin
	i386-ld -Ttext 0x8000 --oformat binary boot.o stage2.o lib.o inb.elf io.o blmalloc.o ata.o libelf.o tty.o kprintf.o ldr.o cat.o mem.o  -o boot.bin
	i386-elf-gcc -c _tty.c -o _tty.o ${CFLAGS}  -Werror
	i386-elf-gcc -c kernel.c -o kernel.o ${CFLAGS}
	i386-elf-gcc -c boot.S -o boot.o ${CFLAGS}
	i386-elf-gcc -c fs.c -o fs.o ${CFLAGS}  -Werror
	i386-elf-gcc -c dev.c -o dev.o ${CFLAGS}
#	@echo "Compiling and archiving libk.a"
#	@i386-elf-gcc -c libk.c -o libk.o ${CFLAGS}
#	@ar rcs libk.a ${CRTBEGIN} ${CRTEND} idt.asm.o crti.o isr.o dev.o crtn.o libk.o idt.o libmem.o _tty.o lib.o fs.o ata.o kprintf.o io.o str.o libk/fs.o libk/str.o libk/err.o libk/io.o prim.o libzfs.o fat.o genfs.o fs.o libmem.o libelf.o
	nasm -f elf boot.asm -o boot.elf
	nasm -f elf panic.asm -o panic.o
#	i386-elf-gcc -T panic.ld panic.o -o fs/panic.ko -nostdlib
	i386-elf-gcc ${CFLAGS} -Tlinker.ld crti.o crtn.o boot.o _tty.o idt.o lib.o fs.o ata.o kernel.o libelf.o kprintf.o io.o str.o libk/fs.o libk/str.o libk/err.o libk/io.o prim.o libzfs.o fat.o genfs.o ${CRTBEGIN} ${CRTEND} libmem.o elf.o idt.asm.o isr.o dev.o -o kernel.elf
#	./genfsfromfile fs.img  dummy.2.bin stage2.2.bin msg.txt kernel.lfs.bin kernel.lfs.elf dummy.bin helloworld.elf dummy.3.elf kernel.elf
	./genfsfromfile fs.img kernel.elf stage2.2.bin msg.txt
	gcc genkernimg.c -o genkernimg -std=gnu99
	cat stage1.bin stage1.2.bin stage2.2.bin fs.img > bootloader.bin
	cat stage1.bin stage1.2.bin hng.bin > hng.raw
	cat stage1.bin stage1.2.bin boot.bin > bootloader.2.bin
	cat stage1.bin stage1.2.bin stage2.2.bin > bootloader.raw
	qemu-img create hd.img.1 100M
#	dd if=/dev/zero of=hd.img.1 bs=1M count=100
	cat bootloader.bin hd.img.1 > hd.img
	rm -f hd.img.1
	./genkernimg bootloader.raw kernel.elf install.pre
	i386-elf-gcc -c _tty.c -o _tty.qemu.o -DQEMU_BUILD ${CFLAGS}
	i386-elf-gcc crti.o crtn.o elf.o isr.o boot.o genfs.o dev.o _tty.qemu.o lib.o fs.o ata.o kernel.o kprintf.o io.o str.o libk/fs.o libk/str.o libk/err.o libk/io.o prim.o  ${CRTBEGIN} ${CRTEND} libzfs.o libmem.o libelf.o idt.o idt.asm.o -o kernel.elf.qemu ${CFLAGS} -T linker.ld
	@qemu-img create install.img.1 100M
	@echo "Compiling and archiving libk.a"
	@i386-elf-gcc -c libk.c -o libk.o ${CFLAGS}
	@ar rcs libk.a ${CRTBEGIN} ${CRTEND} idt.asm.o crti.o isr.o dev.o crtn.o libk.o idt.o libmem.o _tty.o lib.o fs.o ata.o kprintf.o io.o str.o libk/fs.o libk/str.o libk/err.o libk/io.o prim.o libzfs.o fat.o genfs.o fs.o libmem.o libelf.o
#	make -C fs/src
	i386-elf-gcc -T panic.ld panic.o -o fs/panic.ko -nostdlib -lk -L.
	${CC} -Tlinker.isr.ld isr.o -o fs/kernel.ko -lk -L. -nostdlib
	@echo "Compiling and linking yourkernel.elf"
	@i386-elf-gcc -c yourkernel.c -o yourkernel.elf.o ${CFLAGS}
	@i386-ld -Tlinker.ld yourkernel.elf.o -o yourkernel.elf -L. -lk
	make -C fs/src
	./mkgenfs fs install.img.1
#	dd if=/dev/zero of=install.img.1 bs=1M count=100
	./pad install.pre pad.bin
	cat install.pre pad.bin install.img.1 > install.img
	rm -f install.img.1
	@echo "Install Image is install.img"
#	@echo "Compiling and archiving libk.a"
#	@i386-elf-gcc -c libk.c -o libk.o ${CFLAGS}
#	@ar rcs libk.a libk.o ${CRTBEGIN} ${CRTEND} libmem.o crti.o crtn.o boot.o tty.o lib.o fs.o ata.o kprintf.o io.o str.o libk/fs.o libk/str.o libk/err.o libk/io.o prim.o libzfs.o fat.o genfs.o fs.o libmem.o libelf.o
#	@echo "Compiling and linking yourkernel.elf"
#	@i386-elf-gcc -c yourkernel.c -o yourkernel.elf.o ${CFLAGS}
#	@i386-ld -Tlinker.ld yourkernel.elf.o -o yourkernel.elf -L. -lk
	@echo "Generating youros.img"
	@./genkernimg bootloader.raw fs/init.elf youros.img.1
#	@dd if=/dev/zero of=youros.img.2 bs=1M count=100
	@qemu-img create youros.img.2 100M
	@cat youros.img.1 youros.img.2 > youros.img
	@rm -f youros.img.2 youros.img.1
	#make -C fs/src
