all:
	i386-zbl-gcc -c module_entry.c -o module_entry.o
	i386-zbl-gcc -c helloworld.c -o helloworld.o
	i386-zbl-gcc -Tlinker.mod.ld module_entry.o helloworld.o -o helloworld.ko -lc
	i386-zbl-gcc loadmod.c -o ../../exec/loadmod.elf -I../include
	i386-zbl-gcc -c module_call.c -o module_call.o -I../include -I../../..
	i386-zbl-ar rcs libmod.a module_call.o
	i386-zbl-gcc -c modcall.c -o modcall.o -I../include -I../../..
	i386-zbl-gcc modcall.o -o ../../exec/modcall.elf -L. -lmod
