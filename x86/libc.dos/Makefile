all:
	make -C src
	make -C test
install:
	make -C src
	cp src/*.a /usr/local/i386-zbl/lib
	cp src/objs/crt0.o /usr/local/i386-zbl/lib
	cp -r include/* /usr/local/i386-zbl/include
