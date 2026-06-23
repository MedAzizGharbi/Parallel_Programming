#include <omp.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int bin_of(uint64_t i, int bins) {
  uint64_t h = i * 2654435761ULL + 12345ULL;
  return (int)(h % (uint64_t)bins);
}
int main(int argc, char *argv[]) {
  if (argc < 5) {
    printf("Usage : ./histogram <N> <bins> <threads> <method>");
    return 1;
  }
  int N = atoi(argv[1]);
  int bins = atoi(argv[2]);
  int threads = atoi(argv[3]);
  char *method = argv[4];
  int *hist = calloc(bins, sizeof(int));
  omp_set_num_threads(threads);
  double start = omp_get_wtime();
  /*-------------- CRITICAL METHOD -------------- */
  if (strcmp(method, "critical") == 0) {
#pragma omp parallel for schedule(static)
    for (uint64_t i = 0; i < (uint64_t)N; i++) {
      int b = bin_of(i, bins);
#pragma omp critical
      hist[b]++;
    }
  }
  /*-------------- ATOMIC METHOD -------------- */
  else if (strcmp(method, "atomic") == 0) {
#pragma omp parallel for schedule(static)
    for (uint64_t i = 0; i < (uint64_t)N; i++) {
      int b = bin_of(i, bins);
#pragma omp atomic
      hist[b]++;
    }
  }
  /*-------------- LOCK METHOD -------------- */
  else if (strcmp(method, "lock") == 0) {
    omp_lock_t *locks = malloc(bins * sizeof(omp_lock_t));
    for (int i = 0; i < bins; i++)
      omp_init_lock(&locks[i]);
#pragma omp parallel for schedule(static)
    for (uint64_t i = 0; i < (uint64_t)N; i++) {
      int b = bin_of(i, bins);
      omp_set_lock(&locks[b]);
      hist[b]++;
      omp_unset_lock(&locks[b]);
    }
    for (int i = 0; i < bins; i++) {
      omp_destroy_lock(&locks[i]);
    }
    free(locks);
  }
  /*-------------- CORSE-GRAINED LOCK METHOD -------------- */
  else if (strcmp(method, "lock_coarse") == 0) {
    omp_lock_t lock;
    omp_init_lock(&lock);
#pragma omp parallel for schedule(static)
    for (uint64_t i = 0; i < (uint64_t)N; i++) {
      int b = bin_of(i, bins);

      omp_set_lock(&lock);
      hist[b]++;
      omp_unset_lock(&lock);
    }
    omp_destroy_lock(&lock);
  }
  /*-------------- PRIVATE METHOD -------------- */
  else if (strcmp(method, "private") == 0) {
#pragma omp parallel
    {
      int *local_hist = calloc(bins, sizeof(int));
#pragma omp for schedule(static)
      for (uint64_t i = 0; i < (uint64_t)N; i++) {
        int b = bin_of(i, bins);
        local_hist[b]++;
      }
#pragma omp critical
      {
        for (int j = 0; j < bins; j++) {
          hist[j] += local_hist[j];
        }
      }
      free(local_hist);
    }
  } else {
    fprintf(stderr, "Unknown method: %s\n", method);
    free(hist);
    return 1;
  }
  double end = omp_get_wtime();
  int sum = 0;
  for (int i = 0; i < bins; i++) {
    sum += hist[i];
  }
  int check = (sum == N);
  printf("histogram,%d,%d,%d,%s,%f,%d", N, bins, threads, method, end - start,
         check);

  free(hist);
  return 0;
}
