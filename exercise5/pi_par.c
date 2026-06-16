#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
double step;

int main(int argc, char *argv[]) {
  double i;
  double x, pi, sum = 0.0;
  if (argc < 3) {
    printf("Usage : ./pi_seq <num_steps> <num_threads>");
    return 1;
  }

  long num_steps = atol(argv[1]);
  int NUM_THREADS = atoi(argv[2]);
  omp_set_num_threads(NUM_THREADS);

  step = 1.0 / (double)num_steps;
  double start = omp_get_wtime();
  double *local_sums = calloc(NUM_THREADS, sizeof(double));
#pragma omp parallel
  {
    int tid = omp_get_thread_num();
    long chunk = num_steps / NUM_THREADS;
    long start_i = tid * chunk;
    long end_i = (tid == NUM_THREADS - 1) ? num_steps : start_i + chunk;
    double local_sum = 0.0;
    double x;
    for (i = start_i; i < end_i; i++) {
      x = (i + 0.5) * step;
      local_sum += 4.0 / (1 + x * x);
    }
    local_sums[tid] = local_sum;
  }
  for (int tid = 0; tid < NUM_THREADS; tid++) {
    sum += local_sums[tid];
  }
  double end = omp_get_wtime();
  double elapsed = end - start;
  pi = sum * step;
  printf("pi_par, %ld, %d, %f, %f", num_steps, NUM_THREADS, elapsed, pi);
  return 0;
}
