LDOPTS = -Wl,-sectalign,,.text,0x1000

default: amh

clean:
	rm *.o amh; true

amh: Makefile amh.o test.o
	gcc -o amh $(LDOPTS) amh.o test.o

amh.o: amh.c
	gcc -o amh.o -c amh.c

test.o: test.asm
	nasm -f macho64 test.asm

dump: amh
	gobjdump --all amh

.PHONY: default dump
