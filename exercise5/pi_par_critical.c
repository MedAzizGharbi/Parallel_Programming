#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

double step;

#define VARIANT_A 0 // change to 0 for Variant B

int main(int argc, char *argv[]) {

  if (argc < 3) {
    printf("Usage: ./pi_par_critical <num_steps> <num_threads> \n");
    return 1;
  }

  long num_steps = atol(argv[1]);
  int NUM_THREADS = atoi(argv[2]);

  omp_set_num_threads(NUM_THREADS);

  step = 1.0 / (double)num_steps;

  double sum = 0.0;

  double start = omp_get_wtime();

#pragma omp parallel
  {
    int nthreads = omp_get_num_threads();
    int tid = omp_get_thread_num();

    long chunk = num_steps / nthreads;
    long start_i = tid * chunk;
    long end_i = (tid == nthreads - 1) ? num_steps : start_i + chunk;

#if VARIANT_A
    // 🔴 Variant A: critical inside loop
    for (long i = start_i; i < end_i; i++) {
      double x = (i + 0.5) * step;
      double val = 4.0 / (1.0 + x * x);

#pragma omp critical
      sum += val;
    }

#else
    // 🟢 Variant B: local accumulation
    double local_sum = 0.0;

    for (long i = start_i; i < end_i; i++) {
      double x = (i + 0.5) * step;
      local_sum += 4.0 / (1.0 + x * x);
    }

#pragma omp critical
    sum += local_sum;
#endif
  }

  double end = omp_get_wtime();

  double pi = sum * step;

  printf("%s,%ld,%d,%f,%f\n", VARIANT_A ? "A" : "B", num_steps, NUM_THREADS,
         end - start, pi);

  return 0;
}
