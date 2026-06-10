#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  long start;
  long end;
  double partial_sum;
} ThreadData;

void *computePartialSum(void *arg) {
  ThreadData *data = (ThreadData *)arg;

  double psum = 0.0;
  for (long i = data->start; i < data->end; i++) {
    if (i % 2 == 0) {
      psum += 1.0 / ((i + 1) + i);
    } else {
      psum -= 1.0 / ((i + 1) + i);
    }
  }
  data->partial_sum = psum;
  return data;
}

int main(int argc, char *argv[]) {
  if (argc < 3) {
    printf("usage : ./pi <num_iterations> <num_threads>");
    return 0;
  }
  long num_iterations = atol(argv[1]);
  int num_threads = atoi(argv[2]);
  pthread_t threads[num_threads];
  ThreadData thread_data[num_threads];
  long chunk_size = num_iterations / num_threads;

  for (int i = 0; i < num_threads; i++) {
    thread_data[i].start = i * chunk_size;
    if (i == num_threads - 1) {
      thread_data[i].end = num_iterations;
    } else {
      thread_data[i].end = (i + 1) * chunk_size;
    }
    thread_data[i].partial_sum = 0;
    pthread_create(&threads[i], NULL, computePartialSum, &thread_data[i]);
  }
  double sum = 0.0;
  for (int i = 0; i < num_threads; i++) {
    pthread_join(threads[i], NULL);
    sum += thread_data[i].partial_sum;
  }
  double pi = sum * 4.0;
  printf("Approximation of pi : %.15f", pi);
  return EXIT_SUCCESS;
}
