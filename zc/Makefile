all:
	gcc -c libasm.c -o libasm.o -std=gnu99
	gcc -c zasm.c -o zasm.o -std=gnu99
	gcc zasm.o libasm.o -o zasm -lm
	gcc bindump.c -o bindump -std=gnu99
	gcc readzexec.c -o readz -std=gnu99
	gcc zdump.c -o zdump -std=gnu99
