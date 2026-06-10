#include <stdio.h>
#include <stdlib.h>
int main(int argc, char *argv[]) {
  if (argc < 3) {
    printf("usage ./matrix_vector <W> <H>");
    return 0;
  }
  srand(42);
  int W = atoi(argv[1]);
  int H = atoi(argv[2]);

  // We flattened the matrix so the access is mat[i * W + j]
  int *mat = malloc(W * H * sizeof(int));
  int *v = malloc(W * sizeof(int));
  int *result = calloc(H, sizeof(int));
  if (!mat || !v) {
    printf("Memory allocation failed !\n");
    return 1;
  }
  // Initialising the vector and matrix with random values
  for (int i = 0; i < W; i++) {
    v[i] = rand() % 10;
  }
  for (int i = 0; i < H; i++) {
    for (int j = 0; j < W; j++) {
      mat[i * W + j] = rand() % 10;
    }
  }

  // Multiplication
  for (int i = 0; i < H; i++) {
    for (int j = 0; j < W; j++) {
      result[i] += mat[i * W + j] * v[j];
    }
  }
  for (int i = 0; i < H; i++) {
    printf("%d\n", result[i]);
  }

  free(mat);
  free(v);
  free(result);
  return 0;
}
