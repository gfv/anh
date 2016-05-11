#pragma once
extern long long test_run_xchg();
extern long long test_setup_xchg();
extern long long test_run_xor3();
extern long long test_setup_xor3();
long long (*test_entry [])() = {
 test_run_xchg,
 test_run_xor3,
 0 };
long long (*test_setup [])() = {
 test_setup_xchg,
 test_setup_xor3,
 0 };
char *test_names[] = {
 "xchg",
 "xor3",
 0 };
#define TEST_NUM 2
