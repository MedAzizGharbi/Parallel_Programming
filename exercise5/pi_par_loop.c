#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

double step;

int main(int argc, char *argv[]) {

  if (argc < 3) {
    printf("Usage : ./pi_par <num_steps> <num_threads>\n");
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
    int tid = omp_get_thread_num();
    double local_sum = 0.0;

#pragma omp for schedule(static)
    for (long i = 0; i < num_steps; i++) {
      double x = (i + 0.5) * step;
      local_sum += 4.0 / (1.0 + x * x);
    }

#pragma omp critical
    sum += local_sum;
  }


  double end = omp_get_wtime();

  double pi = sum * step;

  printf("pi_par_loop,%ld,%d,%f,%f\n", num_steps, NUM_THREADS, end - start, pi);

  return 0;
}
