#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
double step;

int main(int argc, char *argv[]) {
  double i;
  double x, pi, sum = 0.0;
  if (argc < 3) {
    printf("Usage : ./pi_par <num_steps> <num_threads>");
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
    double local_sum = 0.0;
    double x;
#pragma parallel for schedule(static)
    for (i = 0; i < num_steps; i++) {
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
  printf("pi_par_loop, %ld, %d, %f, %f\n", num_steps, NUM_THREADS, elapsed, pi);
  return 0;
}
