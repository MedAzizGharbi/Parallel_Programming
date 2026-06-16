#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// static int w = 2000;
// static int h = 2000;
// static int max_iter = 10000;

static int mandel_pixel(double cr, double ci, int max_iter) {
  double zr = 0.0, zi = 0.0;
  int n = 0;
  while (n < max_iter && zr * zr + zi * zi <= 4.0) {
    double zr2 = zr * zr - zi * zi + cr;
    zi = 2.0 * zr * zi + ci;
    zr = zr2;
    ++n;
  }
  return n;
}

int main(int argc, char *argv[]) {
  if (argc < 7) {
    printf("usage : ./mandelbrot <w> <h> <max_iter> <threads> <schedule> "
           "<chunk>\n");
    return 0;
  }
  int w = atoi(argv[1]);
  int h = atoi(argv[2]);
  int max_iter = atoi(argv[3]);
  int n_threads = atoi(argv[4]);
  char *schedule = argv[5];
  int chunk = atoi(argv[6]);
  omp_set_num_threads(n_threads);
  omp_sched_t sched;

  if (strcmp(schedule, "static") == 0)
    sched = omp_sched_static;
  else if (strcmp(schedule, "dynamic") == 0)
    sched = omp_sched_dynamic;
  else if (strcmp(schedule, "guided") == 0)
    sched = omp_sched_guided;
  else {
    fprintf(stderr, "Unknown schedule %s\n", schedule);
    return 1;
  }

  omp_set_schedule(sched, chunk);
  const double x_min = -2.0, y_min = -1.5;
  const double dx = (1.0 - x_min) / w;
  const double dy = (1.0 - y_min) / h;
  double *pixels = malloc(w * h * sizeof(double));
  double start = omp_get_wtime();
#pragma omp parallel for schedule(runtime)
  for (int i = 0; i < h; i++) {
    double ci = y_min + i * dy;
    for (int j = 0; j < w; j++) {
      double cr = x_min + j * dx;
      pixels[i * w + j] = mandel_pixel(cr, ci, max_iter);
    }
  }
  double end = omp_get_wtime();
  printf("%d,%d,%d,%d,%s,%d,%f\n", w, h, max_iter, n_threads, schedule, chunk,
         end - start);
  return 0;
}
