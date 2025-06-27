#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <numa.h>
#include <sched.h>
#include <string.h>
#include <time.h>
#include <stdint.h>

#define NUM_THREADS 4
#define SIZE_PER_THREAD (1024L * 1024L * 512L)  // 512 MB per thread
#define REPEAT 10
#define STRIDE 64  // cache-line stride

typedef struct {
    char *buf;
    size_t size;
    int thread_id;
    double bandwidth;
} ThreadArgs;

double now_sec() {
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return t.tv_sec + t.tv_nsec * 1e-9;
}

void *thread_read_bw(void *arg) {
    ThreadArgs *args = (ThreadArgs *)arg;
    char *src = args->buf;
    size_t size = args->size;
    int tid = args->thread_id;
    volatile uint8_t sink = 0;

    // Pin thread to core
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(tid, &cpuset);
    sched_setaffinity(0, sizeof(cpu_set_t), &cpuset);

    double t0 = now_sec();
    for (int r = 0; r < REPEAT; ++r) {
        for (size_t i = 0; i < size; i += STRIDE) {
            sink += src[i];
        }
    }
    double t1 = now_sec();
    args->bandwidth = ((double)size * REPEAT) / (t1 - t0) / 1e9;

    pthread_exit(NULL);
}

int main() {
    if (numa_available() == -1) {
        printf("NUMA is not available on this system.\n");
    }

    pthread_t threads[NUM_THREADS];
    ThreadArgs args[NUM_THREADS];

    printf("Allocating %ld MB total...\n", (SIZE_PER_THREAD * NUM_THREADS) / (1024 * 1024));
    for (int i = 0; i < NUM_THREADS; ++i) {
        char *buf = numa_alloc_onnode(SIZE_PER_THREAD, i % numa_max_node() + 1);
        if (!buf) {
            perror("numa_alloc_onnode failed");
            exit(1);
        }
        memset(buf, 1, SIZE_PER_THREAD);  // initialize

        args[i].buf = buf;
        args[i].size = SIZE_PER_THREAD;
        args[i].thread_id = i;
    }

    for (int i = 0; i < NUM_THREADS; ++i)
        pthread_create(&threads[i], NULL, thread_read_bw, &args[i]);

    for (int i = 0; i < NUM_THREADS; ++i)
        pthread_join(threads[i], NULL);

    double total_bw = 0.0;
    for (int i = 0; i < NUM_THREADS; ++i) {
        printf("Thread %d: %.2f GB/s\n", i, args[i].bandwidth);
        total_bw += args[i].bandwidth;
        numa_free(args[i].buf, SIZE_PER_THREAD);
    }

    printf("Total Bandwidth: %.2f GB/s\n", total_bw);
    return 0;
}