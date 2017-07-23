STR_OBJS = objs/strlen.o objs/strcpy.o objs/strcat.o objs/strcmp.o objs/strncmp.o objs/memcpy.o objs/memmove.o objs/memset.o objs/strncpy.o objs/strncat.o objs/memcmp.o
str_LIBRARIES = libstr.a
KERN_OBJS = objs/print.o objs/t_readvals.o objs/t_writevals.o objs/kprintf.o objs/t_putc.o
kern_LIBRARIES = libkern.a
CTYPE_OBJS = objs/isalnum.o objs/isalpha.o objs/iscntrl.o objs/isdigit.o objs/isgraph.o objs/islower.o objs/isupper.o objs/isprint.o objs/ispunct.o objs/isspace.o objs/isxdigit.o objs/tolower.o objs/toupper.o
STDLIB_OBJS = objs/malloc.o
ctype_LIBRARIES = libctype.a
MAIN_OBJS = ${str_OBJS} ${kern_OBJS} ${ctype_OBJS}
MATH_OBJS = objs/fact.o objs/sin.o objs/pow.o
main_LIBRARIES = libc.a
MATHT_OBJS = objs/factt.o objs/sint.o objs/powt.o
CC=i386-zbl-gcc
CFLAGS += -I../include -nostdlib -Werror
AR=i386-zbl-ar
all:
	${CC} -c strlen.c -o objs/strlen.o ${CFLAGS}
	${CC} -c strcpy.c -o objs/strcpy.o ${CFLAGS}
	${CC} -c strcat.c -o objs/strcat.o ${CFLAGS}
	${CC} -c strncpy.c -o objs/strncpy.o ${CFLAGS}
	${CC} -c strncat.c -o objs/strncat.o ${CFLAGS}
	${CC} -c strcmp.c -o objs/strcmp.o ${CFLAGS}
	${CC} -c strncmp.c -o objs/strncmp.o ${CFLAGS}
	${CC} -c memcpy.c -o objs/memcpy.o ${CFLAGS}
	${CC} -c memmove.c -o objs/memmove.o ${CFLAGS}
	${CC} -c memcmp.c -o objs/memcmp.o ${CFLAGS}
	${CC} -c crt0.c -o objs/crt0.o ${CFLAGS}
	${CC} -c memset.c -o objs/memset.o ${CFLAGS}
	${AR} rcs libstr.a ${STR_OBJS}
	${CC} -c print.c -o objs/print.o ${CFLAGS}
	${CC} -c t_readvals.c -o objs/t_readvals.o ${CFLAGS}
	${CC} -c t_writevals.c -o objs/t_writevals.o ${CFLAGS}
	${CC} -c kprintf.c -o objs/kprintf.o ${CFLAGS} -Wno-error
	${CC} -c crti.S -o objs/crti.o ${CFLAGS}
	${CC} -c crtn.S -o objs/crtn.o ${CFLAGS}
	${CC} -c t_putc.c -o objs/t_putc.o ${CFLAGS}
	${AR} rcs libkern.a ${KERN_OBJS}
	${CC} -c isalnum.c -o objs/isalnum.o ${CFLAGS}
	${CC} -c isalpha.c -o objs/isalpha.o ${CFLAGS}
	${CC} -c iscntrl.c -o objs/iscntrl.o ${CFLAGS}
	${CC} -c isdigit.c -o objs/isdigit.o ${CFLAGS}
	${CC} -c isgraph.c -o objs/isgraph.o ${CFLAGS}
	${CC} -c islower.c -o objs/islower.o ${CFLAGS}
	${CC} -c isupper.c -o objs/isupper.o ${CFLAGS}
	${CC} -c isprint.c -o objs/isprint.o ${CFLAGS}
	${CC} -c ispunct.c -o objs/ispunct.o ${CFLAGS}
	${CC} -c isspace.c -o objs/isspace.o ${CFLAGS}
	${CC} -c isxdigit.c -o objs/isxdigit.o ${CFLAGS}
	${CC} -c tolower.c -o objs/tolower.o ${CFLAGS}
	${CC} -c toupper.c -o objs/toupper.o ${CFLAGS}
	${AR} rcs libctype.a ${CTYPE_OBJS}
	${CC} -c malloc.c -o objs/malloc.o ${CFLAGS}
	${AR} rcs libstd.a ${STDLIB_OBJS}
	${AR} rcs libc.a objs/crti.o ${KERN_OBJS} ${STR_OBJS} ${CTYPE_OBJS} ${STDLIB_OBJS} objs/crtn.o
	${CC} -c pow.c -o objs/pow.o ${CFLAGS}
	${CC} -c fact.c -o objs/fact.o ${CFLAGS}
	${CC} -c sin.c -o objs/sin.o ${CFLAGS}
	${AR} rcs libm.a ${MATH_OBJS}
	gcc -c pow.c -o objs/powt.o  -D__TBUILD
	gcc -c fact.c -o objs/factt.o  -D__TBUILD
	gcc -c sin.c -o objs/sint.o  -D__TBUILD
	ar rcs libmatht.a ${MATHT_OBJS}
