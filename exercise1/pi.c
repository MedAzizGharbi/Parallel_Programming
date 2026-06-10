#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("usage : ./pi <num_iterations>");
    return 0;
  }
  long num_iterations = atoi(argv[1]);
  int sign = 1;
  double sum = 0.0;
  for (long i = 0; i < num_iterations; i++) {
    if (i % 2 == 0) {
      sum += 1.0 / ((i + 1) + i);
    } else {
      sum -= 1.0 / ((i + 1) + i);
    }
  }
  double pi = sum * 4.0;
  printf("Approximation of pi : %.15f", pi);
  return EXIT_SUCCESS;
}
