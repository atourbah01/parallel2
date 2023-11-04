#include <stdio.h>
#include <omp.h>
#include <time.h>

#define M 1000
#define N 1000
#define P 1000

#define NUM_THREADS 4

int A[M][N];
int B[N][P];
int C[M][P];

double get_time() {
    struct timespec time;
    clock_gettime(CLOCK_MONOTONIC, &time);
    return time.tv_sec + 1e-9 * time.tv_nsec;
}

int main() {
    double start_time, end_time;
    double sequential_time, parallel_time;

    // Initialize matrix A
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            A[i][j] = i * N + j + 1;
        }
    }
    // Initialize matrix B
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < P; j++) {
            B[i][j] = (i + 1) * P + j;
        }
    }

    // Sequential matrix multiplication
    start_time = get_time();
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < P; j++) {
            int sum = 0;
            for (int k = 0; k < N; k++) {
                sum += A[i][k] * B[k][j];
            }
            C[i][j] = sum;
        }
    }
    end_time = get_time();
    sequential_time = end_time - start_time;

    // Parallel matrix multiplication
    start_time = get_time();
    #pragma omp parallel for
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < P; j++) {
            int sum = 0;
            for (int k = 0; k < N; k++) {
                sum += A[i][k] * B[k][j];
            }
            C[i][j] = sum;
        }
    }
    end_time = get_time();
    parallel_time = end_time - start_time;

    // Calculate performance metrics
    double speedup = sequential_time / parallel_time;
    double efficiency = speedup / NUM_THREADS;
    double scalability = speedup;

    // Print the result matrix C
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < P; j++) {
            printf("%d ", C[i][j]);
        }
        printf("\n");
    }

    printf("Speedup: %lf\n", speedup);
    printf("Efficiency: %lf\n", efficiency);
    printf("Scalability: %lf\n", scalability);

    return 0;
}
