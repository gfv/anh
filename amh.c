#include <unistd.h>
#include <math.h>
#include <stdio.h>
#include <time.h>
#include <assert.h>

#include "_test_list.h"

volatile long long run_count;

#ifndef UNROLL
#error Do not know how many unrolled runs are there
#endif

#define WARMUP_TIME 3
#define TEST_TIME 5

#define MAX_ITER 1048576

struct test_result {
  char* name;
  int iterations;
  double avg;
  double stddev;
};

struct test_result results[TEST_NUM];

int iterations = 0;
long int time_table[MAX_ITER];
volatile int stop_iterations = 0;

int main( int argc, char** argv ) {
  run_count = 1; assert( run_count > 0 );


  printf( "AMH started: %lld runs per %d unrolled operations\n", run_count, UNROLL );

  int test_num = 0;
  while ( test_names[test_num] != 0 ) {
    double avg = 0;

    printf( "  Running test %s\n", test_names[test_num] );
    test_setup[test_num]();

    printf( "   Running warmup for %ds...\n", WARMUP_TIME );
    struct timespec series_start_time;
    struct timespec current_time;
    clock_gettime(CLOCK_MONOTONIC, &series_start_time);

    while (1) {
      test_entry[test_num]();

      clock_gettime(CLOCK_MONOTONIC, &current_time);
      if (series_start_time.tv_sec + WARMUP_TIME < current_time.tv_sec) break;
    }

    printf( "   Running tests for %ds...\n", TEST_TIME );
    clock_gettime(CLOCK_MONOTONIC, &series_start_time);
    while ( iterations < MAX_ITER ) {
      struct timespec begin_run;
      struct timespec end_run;

      clock_gettime(CLOCK_MONOTONIC, &begin_run);
      long long R = test_entry[test_num]();
      clock_gettime(CLOCK_MONOTONIC, &end_run);

      long long begin_ns = begin_run.tv_sec * 1000000000 + begin_run.tv_nsec;
      long long end_ns = end_run.tv_sec * 1000000000 + end_run.tv_nsec;

      long time_diff = end_ns - begin_ns;
      assert( time_diff > 0 );

      time_table[iterations] = time_diff / (run_count * UNROLL);
      avg_ti += time_table[iterations];

      // printf("Run %i, fin@%ld.%ld delta %ldns\n", iterations, end_run.tv_sec, end_run.tv_nsec, time_diff);
      if (series_start_time.tv_sec + TEST_TIME < end_run.tv_sec) break;
      iterations++;
    }

    avg = avg / iterations;

    double stddev = 0;

    for ( int i = 0; i < iterations; i++ ) {
      stddev += (time_table[i] - avg_ti) * (time_table[i] - avg_ti);
    }
    stddev = sqrt(stddev / (iterations * run_count * UNROLL));

    results[test_num].name = test_names[test_num];
    results[test_num].iterations = iterations;
    results[test_num].avg = avg;
    results[test_num].stddev = stddev;

    test_num++;
  }

  printf( "Results:\n" );

  for ( int i = 0; i < TEST_NUM; i++ ) {
    char* time_unit = "ns";
    double avg;
    double stddev;
    double confidence_d = 1.96 * (results[i].stddev / sqrt (results[i].iterations * run_count * UNROLL));

    if ( results[i].avg > 1e6 ) {
      avg = results[i].avg / 1e6;
      stddev = results[i].stddev / 1e6;
      confidence_d /= 1e6;
      time_unit = "ms";
    } else if ( avg_ti > 1e3 ) {
      avg = results[i].avg / 1e3;
      stddev = results[i].stddev / 1e3;
      confidence_d /= 1e3;
      time_unit = "ms";
    } else {
      avg = results[i].avg;
      stddev = results[i].stddev;
    }

    printf( "%s\t%i runs\tAverage operation: %.2lf ± %.2lf%s (95%% conf-int %.2lf%s; %.2lf%s)\n", results[i].name, results[i].iterations, avg, stddev, time_unit,
            avg - confidence_d, time_unit, avg + confidence_d, time_unit);
  }

  return 0;
}
