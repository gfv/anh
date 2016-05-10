#include <stdio.h>

extern long run_test();

int main( int argc, char** argv ) {
  printf( "amh started\n" );
  printf( "run_test pointer: %p\n", &run_test );
  printf( "test result: %ld\n", run_test() );
  return 0;
}
