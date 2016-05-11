#include <math.h>
#include <stdio.h>
#include <time.h>
#include <assert.h>

extern long run_test();
extern long asm_setup();
volatile long long instruction_count;
volatile long long run_count;

#define WARMUP_N 256
#define N 1024

double iprs[N];
long int times[N];

int main( int argc, char** argv ) {
  run_count = 1; assert( run_count > 0 );
  
  double avg_is = 0;
  double avg_ti = 0;

  asm_setup();

  printf( "AMH started\n" );
  printf( "Running %d warmup rounds...\n", WARMUP_N );
  for ( int i = 0; i < WARMUP_N; i++ ) {
    run_test();
  }

  printf( "Running %d testing rounds...\n", N );
  for ( int i = 0; i < N; i++ ) {
    struct timespec begin_run;
    struct timespec end_run;

    clock_gettime(CLOCK_MONOTONIC, &begin_run);
    long long R = run_test();
    clock_gettime(CLOCK_MONOTONIC, &end_run);

    long long begin_ns = begin_run.tv_sec * 1000000000 + begin_run.tv_nsec;
    long long end_ns = end_run.tv_sec * 1000000000 + end_run.tv_nsec;

    long time_diff = end_ns - begin_ns;
    assert( time_diff > 0 );

    iprs[i] = instruction_count / run_count;
    times[i] = time_diff / run_count;
    avg_is += iprs[i];
    avg_ti += times[i];
  }

  avg_is = avg_is / N;
  avg_ti = avg_ti / N;

  double stddev_is = 0;
  double stddev_ti = 0;

  for ( int i = 0; i < N; i++ ) {
    stddev_is += (iprs[i] - avg_is) * (iprs[i] - avg_is);
    stddev_ti += (times[i] - avg_ti) * (times[i] - avg_ti);
  }
  stddev_is = sqrt(stddev_is / N);
  stddev_ti = sqrt(stddev_ti / N);

  printf( "Average instruction count: %.2lf ± %.2lf\n", avg_is, stddev_is );

  char* time_unit = "ns";
  if ( avg_ti > 1e6 ) {
    avg_ti /= 1e6;
    stddev_ti /= 1e6;
    time_unit = "ms";
  }
  if ( avg_ti > 1e3 ) {
    avg_ti /= 1e3;
    stddev_ti /= 1e3;
    time_unit = "μs";
  }

  printf( "Average operation: %.2lf ± %.2lf %s (%.1f%%)\n", avg_ti, stddev_ti, time_unit, stddev_ti*100 / avg_ti );
  return 0;
}
