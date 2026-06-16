#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
static long num_steps = 100000;
double step;

int main(int argc, char *argv[]) {
  double i;
  double x, pi, sum = 0.0;
  if (argc < 2) {
    printf("Usage : ./pi_seq <num_steps>");
    return 1;
  }

  long num_steps = atol(argv[1]);

  step = 1.0 / (double)num_steps;
  double start = omp_get_wtime();
  for (i = 0; i < num_steps; i++) {
    x = (i + 0.5) * step;
    sum = sum + 4.0 / (1 + x * x);
  }
  double end = omp_get_wtime();
  double elapsed = end - start;
  pi = sum * step;
  printf("pi_seq, %ld, 1, %f, %f\n", num_steps, elapsed, pi);
  return 0;
}
