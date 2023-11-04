#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define M 1000
#define N 1000
#define P 1000
#define NUM_THREADS 50

int** A;
int** B;
int** C;

typedef struct {
    int start;
    int end;
} thread_range;

void *multiply(void *param) {
    
    thread_range *range = (thread_range *)param;

    int start = range->start;
    int end = range->end;

     for (int i = start; i < end; i++) {
        for (int j = 0; j < P; j++) {
            int sum = 0;
            for (int k = 0; k < N; k++) {
                sum += A[i][k] * B[k][j];
            }
            C[i][j] = sum;
        }
    }
    pthread_exit(0);
}

int main() {
    clock_t start, end;
    double sequential_time, parallel_time;

    pthread_t threads[NUM_THREADS];
    thread_range ranges[NUM_THREADS];

    // Allocate memory for matrices A, B, and C
    A = (int**)malloc(M * sizeof(int*));
    B = (int**)malloc(N * sizeof(int*));
    C = (int**)malloc(M * sizeof(int*));
    for (int i = 0; i < M; i++) {
        A[i] = (int*)malloc(N * sizeof(int));
        C[i] = (int*)malloc(P * sizeof(int));
    }
    for (int i = 0; i < N; i++) {
        B[i] = (int*)malloc(P * sizeof(int));
    }

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

    // Start timing for sequential execution
    start = clock();

    // Sequential matrix multiplication
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < P; j++) {
            int sum = 0;
            for (int k = 0; k < N; k++) {
                sum += A[i][k] * B[k][j];
            }
            C[i][j] = sum;
        }
    }

    // End timing for sequential execution
    end = clock();
    sequential_time = (double)(end - start) / CLOCKS_PER_SEC;

    // Print the result matrix C (sequential version)
    printf("Sequential Result:\n");
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < P; j++) {
            printf("%d ", C[i][j]);
        }
        printf("\n");
    }
    // Reset matrix C for parallel execution
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < P; j++) {
            C[i][j] = 0;
        }
    }
    // Start timing for parallel execution
    start = clock();

    // Create threads for matrix multiplication (parallel version)
    for (int i = 0; i < NUM_THREADS; i++) {
        ranges[i].start = i * (M / NUM_THREADS);
        ranges[i].end = (i == NUM_THREADS - 1) ? M : (i + 1) * (M / NUM_THREADS);
        pthread_create(&threads[i], NULL, multiply,&ranges[i]);
    }

    // Wait for threads to finish
    for (int i = 0; i < NUM_THREADS; i++) {
            pthread_join(threads[i], NULL);
    }

    // End timing for parallel execution
    end = clock();
    parallel_time = (double)(end - start) / CLOCKS_PER_SEC;

    // Print the result matrix C (parallel version)
    printf("Parallel Result:\n");
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < P; j++) {
            printf("%d ", C[i][j]);
        }
        printf("\n");
    }

    // Calculate performance metrics
    double speedup = sequential_time / parallel_time;
    double efficiency = speedup / NUM_THREADS;
    double scalability = speedup;

    printf("Parallel execution time: %lf seconds\n", parallel_time);
    printf("Speedup: %lf\n", speedup);
    printf("Efficiency: %lf\n", efficiency);
    printf("Scalability: %lf\n", scalability);

    // Free allocated memory
    for (int i = 0; i < M; i++) {
        free(A[i]);
        free(C[i]);
    }
    for (int i = 0; i < N; i++) {
        free(B[i]);
    }
    free(A);
    free(B);
    free(C);

    return 0;
}
