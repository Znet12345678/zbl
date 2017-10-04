libstrdir = .
libkerndir = .
libcdir = .
libctypedir = .
crt0dir = .
libstr_LIBRARIES = libstr.a
libstr_a_SOURCES = strlen.c strcpy.c strcat.c strcmp.c strncmp.c
libkern_LIBRARIES = libkern.a
libkern_a_SOURCES = print.c t_readvals.c t_writevals.c kprintf.c t_putc.c
libctype_LIBRARIES = libctype.a
libctype_a_SOURCES = isalnum.c isalpha.c iscntrl.c isdigit.c isgraph.c islower.c isupper.c isprint.c ispunct.c isspace.c isxdigit.c tolower.c toupper.c
libc_LIBRARIES = libc.a
libc_a_SOURCES = ${libstr_a_SOURCES} ${libkern_a_SOURCES} ${libctype_a_SOURCES}
crt0_o_SOURCES = crt0.c
CC=i386-zbl-gcc
AM_CFLAGS = -I../include -nostdlib
AR=i386-zbl-ar

