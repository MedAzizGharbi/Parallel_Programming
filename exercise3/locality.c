#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static double now_seconds(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec * 1e-9;
}

static double sum_row_major(double* A, int N) {
    double s = 0.0;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            s += A[i * N + j];
        }
    }
    return s;
}

static double sum_col_major(double* A, int N) {
    double s = 0.0;
		for(int j = 0 ; j < N ; j++){
				for(int i = 0 ; i < N ; i++){
						s+= A[i * N + j];
				}
		}
    return s;
}

int main(int argc, char** argv) {
    int N = (argc >= 2) ? atoi(argv[1]) : 4000;

    double* A = (double*) malloc((size_t) N * N * sizeof(double));
    if (!A) { fprintf(stderr, "allocation failed\n"); return 1; }
    for (long k = 0; k < (long) N * N; ++k) A[k] = 1.0;

    double t0 = now_seconds();
    double s_row = sum_row_major(A, N);
    double t_row = now_seconds() - t0;

    t0 = now_seconds();
    double s_col = sum_col_major(A, N);
    double t_col = now_seconds() - t0;

    printf("N = %d\n", N);
    printf("  row-major sum    = %.1f   time = %.3f s\n", s_row, t_row);
    printf("  column-major sum = %.1f   time = %.3f s\n", s_col, t_col);
    if (t_row > 0.0) {
        printf("  ratio col/row    = %.2fx\n", t_col / t_row);
    }

    free(A);
    return 0;
}
