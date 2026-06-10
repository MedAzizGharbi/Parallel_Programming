#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
#pragma omp parallel
  {
    printf("I am parallel\n");
    printf("I am not parallel\n");
		printf("Thread id : %d , number of threads : %d\n", omp_get_thread_num(), omp_get_num_threads());
  }
	printf("Number of threads outside the paralle thing : %d\n", omp_get_num_threads());
  return 0;
}
