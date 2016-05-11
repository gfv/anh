#!/bin/bash

UNROLL=1024

CFLAGS="-O0 -g -std=gnu99 -DUNROLL=${UNROLL}"
NASMFLAGS="-f elf64 -dUNROLL=${UNROLL}"
LDFLAGS=""

function assert() {
  echo "$*" >&2
  $* || { echo "FAIL: exit code $?" >&2; exit 1; }
}

rm *.o _linker.ld _test_list.h amh 2>/dev/null
[ "$1" = "clean" ] && exit 0

for F in *.test.asm; do
  O=${F%.test.asm}.test.o
  assert nasm $NASMFLAGS -o $O $F
done

assert ./gen-linker.sh *.test.o > _linker.ld
assert ./gen-header.sh *.test.o > _test_list.h

assert gcc $CFLAGS -o amh.o -c amh.c
assert gcc $LDFLAGS amh.o *.test.o -o amh -lm -Wl,-T,_linker.ld
