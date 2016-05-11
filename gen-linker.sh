#!/bin/bash
BASE_ADDR=0x10000000
STEP=0x10000000

echo "SECTIONS {"
echo '  .test_meta : {'
echo '    *.test.o (.meta)';
echo '  }'

ADDR=$BASE_ADDR
for TEST in $*; do
  TEST=${TEST%.test.o}
  echo "  .test_${TEST}_code $ADDR : {"
  echo "    ${TEST}.test.o (.text)";
  echo "  }"
  ADDR=$(printf "0x%x" $(( $ADDR + $STEP )) )
  echo "  .test_${TEST}_data $ADDR : {"
  echo "    ${TEST}.test.o (.data)";
  echo "  }"
  ADDR=$(printf "0x%x" $(( $ADDR + $STEP )) )
  echo "  .test_${TEST}_bss $ADDR : {"
  echo "    ${TEST}.test.o (.bss)";
  echo "  }"
  ADDR=$(printf "0x%x" $(( $ADDR + $STEP )) )
done
echo "}"
echo "INSERT AFTER .text;"
