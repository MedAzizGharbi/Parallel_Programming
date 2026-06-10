#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#define T 4
#define R 3

// Sequential version
int count3s(int *a, int n) {
  int count = 0;
  for (int i = 0; i < n; i++) {
    if (a[i] == 3) {
      ++count;
    }
  }
  return count;
}

// Variant 1
int count3sv1(int *a, int n) {
  int count = 0;
  omp_set_num_threads(T);
#pragma omp parallel for schedule(static)
  for (int i = 0; i < n; i++) {
    if (a[i] == 3) {
      ++count;
    }
  }
  return count;
}

// Variant 2
int count3sv2(int *a, int n) {
  int count = 0;
  omp_set_num_threads(T);
#pragma omp parallel for schedule(static)
  for (int i = 0; i < n; ++i) {
    if (a[i] == 3) {
#pragma omp atomic
      ++count;
    }
  }
  return count;
}

// Variant3
int count3sv3(int *a, int n) {
  int count = 0;
  int private_count[T];
  for (int i = 0; i < T; ++i) {
    private_count[i] = 0;
  }
  omp_set_num_threads(T);
#pragma omp parallel for schedule(static)
  for (int i = 0; i < n; ++i) {
    if (a[i] == 3) {
      ++private_count[omp_get_thread_num()];
    }
  }
#pragma omp parallel
  {
#pragma omp atomic
    count += private_count[omp_get_thread_num()];
  }
  return count;
}

// Variant 4
int count3sv4(int *a, int n) {
  int count = 0;
  struct padded_int {
    int value;
    char padding[60];
  } private_count[T];
  for (int i = 0; i < T; ++i) {
    private_count[i].value = 0;
  }

  omp_set_num_threads(T);
#pragma omp parallel for schedule(static)
  for (int i = 0; i < n; ++i) {
    if (a[i] == 3) {
      ++private_count[omp_get_thread_num()].value;
    }
  }

#pragma omp parallel
  {
#pragma omp atomic
    count += private_count[omp_get_thread_num()].value;
  }

  return count;
}

int main(int argc, char *argv[]) {
  int N;
  if (argc < 2) {
    N = 100000000;
  }
  N = atol(argv[1]);
  int *a = malloc(N * sizeof(int));
  for (long i = 0; i < N; i++) {
    a[i] = rand() % 10;
  }

  int count;
  for (int r = 0; r < R; r++) {
    double start = omp_get_wtime();
    count = count3s(a, N);
    double runtime = omp_get_wtime() - start;
    printf("Sequential // Run number %d // Count of 3s : %d // Time elapsed : "
           "%f\n",
           r + 1, count, runtime);
  }
  for (int r = 0; r < R; r++) {
    double start = omp_get_wtime();
    count = count3sv1(a, N);
    double runtime = omp_get_wtime() - start;
    printf(
        "Variant 1 // Run number %d // Count of 3s : %d // Time elapsed : %f\n",
        r + 1, count, runtime);
  }
  for (int r = 0; r < R; r++) {
    double start = omp_get_wtime();
    count = count3sv2(a, N);
    double runtime = omp_get_wtime() - start;
    printf(
        "Variant 2 // Run number %d // Count of 3s : %d // Time elapsed : %f\n",
        r + 1, count, runtime);
  }
  for (int r = 0; r < R; r++) {
    double start = omp_get_wtime();
    count = count3sv3(a, N);
    double runtime = omp_get_wtime() - start;
    printf(
        "Variant 3 // Run number %d // Count of 3s : %d // Time elapsed : %f\n",
        r + 1, count, runtime);
  }
  for (int r = 0; r < R; r++) {
    double start = omp_get_wtime();
    count = count3sv4(a, N);
    double runtime = omp_get_wtime() - start;
    printf(
        "Variant 4 // Run number %d // Count of 3s : %d // Time elapsed : %f\n",
        r + 1, count, runtime);
  }
  return 0;
}
