#!/bin/bash

echo '#pragma once'

for TEST in $*; do
  TEST=${TEST%.test.o}
  echo "extern long long test_run_${TEST}();"
  echo "extern long long test_setup_${TEST}();"
  #echo "extern long long unroll_${TEST};"
done

echo 'long long (*test_entry [])() = {'
for TEST in $*; do
  TEST=${TEST%.test.o}
  echo " test_run_${TEST},"
done
echo ' 0 };';

echo 'long long (*test_setup [])() = {'
for TEST in $*; do
  TEST=${TEST%.test.o}
  echo " test_setup_${TEST},"
done
echo ' 0 };';

echo 'char *test_names[] = {' 
for TEST in $*; do
  TEST=${TEST%.test.o}
  echo " \"${TEST}\","
done
echo ' 0 };';
echo "#define TEST_NUM $#"


#echo 'long long* test_unrolls [] = {'
#for TEST in $*; do
#  TEST=${TEST%.test.o}
#  echo " &unroll_${TEST},"
#done
#echo ' 0 };';
