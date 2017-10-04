CFLAGS=-fPIC -std=gnu99
all:
	gcc -c libasm.c -o libasm.o ${CFLAGS}
	gcc -c zasm.c -o zasm.o -std=gnu99
	gcc zasm.o libasm.o -o zasm -lm
	gcc bindump.c -o bindump -std=gnu99
	gcc readzexec.c -o readz -std=gnu99
	gcc zdump.c -o zdump -std=gnu99
	gcc -c libzexec.c -o libzexec.o -std=gnu99 ${CFLAGS}
	gcc -shared -fPIC libasm.o libzexec.o -o libzexec.so
	gcc -c dl.c -o dl.o -std=gnu99
	gcc dl.o -o dl
