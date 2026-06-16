#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int min(int a, int b) { return a < b ? a : b; };

int main(int argc, char *argv[]) {
  if (argc < 4) {
    printf("Usage : ./matmul <N> <Num_threads> <Bsize>");
    return 1;
  }
  int N = atoi(argv[1]);
  int THREADS = atoi(argv[2]);
  int B = atoi(argv[3]);

  int i, j, k;
  double sum = 0.0;
  double *a = malloc(N * N * sizeof(double));
  double *b = malloc(N * N * sizeof(double));
  double *c = calloc(N * N, sizeof(double));
  /* Init */
  for (i = 0; i < N; i++) {
    for (j = 0; j < N; j++) {
      a[i * N + j] = 3.0 * i + j;
      b[i * N + j] = 5.2 * i + 2.3 * j;
    }
  }
  omp_set_num_threads(THREADS);
  /* Matrix Multiplication with parallel for loop */
  double start = omp_get_wtime();
#pragma omp parallel for collapse(2) schedule(static)
  for (int ii = 0; ii < N; ii += B) {
    for (int kk = 0; kk < N; kk += B) {
      for (int jj = 0; jj < N; jj += B) {
        for (i = ii; i < min(ii + B, N); i++) {
          for (k = kk; k < min(kk + B, N); k++) {
            double scalar = a[i * N + k];
            for (j = jj; j < min(jj + B, N); j++) {
              c[i * N + j] += scalar * b[k * N + j];
            }
          }
        }
      }
    }
  }
  double end = omp_get_wtime();
  double runtime = end - start;
  for (i = 0; i < N; ++i) {
    for (j = 0; j < N; ++j) {
      sum += c[i * N + j];
    }
  }
  printf("matmul_blocked,%d,%d,%d,%f,%.4f\n", N, B, THREADS, runtime, sum);
  free(a);
  free(b);
  free(c);
  return 0;
}
