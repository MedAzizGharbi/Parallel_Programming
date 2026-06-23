#include <omp.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  int64_t n, len;
} best_t;

static best_t better(best_t a, best_t b) {
  if (b.len > a.len || (b.len == a.len && b.n < a.n))
    return b;
  return a;
}

#pragma omp declare reduction(best:best_t : omp_out = better(omp_out, omp_in)) \
    initializer(omp_priv = (best_t){0, -1})

int64_t collatz_length(int64_t n) {
  int x = 0;
  while (n != 1) {
    if (n % 2 == 0) {
      n = n / 2;
    } else {
      n = 3 * n + 1;
    }
    x++;
  }
  return x;
}

int main(int argc, char *argv[]) {
  if (argc < 5) {
    printf("Usage : ./collatz <N> <THREADS> <SCHEDULE> <CHUNK>");
    return 1;
  }
  int N = atoi(argv[1]);
  int THREADS = atoi(argv[2]);
  char *schedule = argv[3];
  int chunk = atoi(argv[4]);
  omp_set_num_threads(THREADS);
  if (strcmp(schedule, "static") == 0) {
    if (chunk == 16)
      omp_set_schedule(omp_sched_static, 16);
    else
      omp_set_schedule(omp_sched_static, 1);
  } else if (strcmp(schedule, "dynamic") == 0) {
    if (chunk == 16)
      omp_set_schedule(omp_sched_dynamic, 16);
    else
      omp_set_schedule(omp_sched_dynamic, 1);
  } else if (strcmp(schedule, "guided") == 0) {
    omp_set_schedule(omp_sched_guided, 1);
  } else {
    printf("Unknown schedule : %s \n", schedule);
    return 1;
  }
  best_t result = {0, -1};
  double start = omp_get_wtime();
#pragma omp parallel for schedule(runtime) reduction(best : result)
  for (int64_t n = 1; n <= N; ++n) {
    int64_t L = collatz_length(n);
    result = better(result, (best_t){n, L});
  }
  double elapsed = omp_get_wtime() - start;
  int best_n = result.n;
  int best_len = result.len;

  printf("%d,%d,%s,%d,%f,%d,%d\n", N, THREADS, schedule, chunk, elapsed, best_n,
         best_len);
  return 0;
}
