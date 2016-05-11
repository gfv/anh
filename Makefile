LDOPTS = 

default: amh

clean:
	rm *.o amh; true

amh: amh.o test.o
	gcc -static -g -o amh amh.o -lm -Wl,-T,linker.ld

amh.o: amh.c
	gcc -std=gnu99 -g -o amh.o -c amh.c

test.o: test.asm
	nasm -f elf64 test.asm

dump: amh
	objdump -fh amh

.PHONY: default dump
