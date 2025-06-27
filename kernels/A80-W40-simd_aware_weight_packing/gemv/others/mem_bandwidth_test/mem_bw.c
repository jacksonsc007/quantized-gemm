#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define SIZE (1024L * 1024L * 1024L)  // 1 GB
#define REPEAT 10

double now_sec() {
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return t.tv_sec + t.tv_nsec * 1e-9;
}

void benchmark_copy(char *dst, char *src, size_t size) {
    double t0 = now_sec();
    for (int i = 0; i < REPEAT; ++i) {
        memcpy(dst, src, size);
    }
    double t1 = now_sec();
    double bw = (double)size * REPEAT / (t1 - t0) / 1e9;
    printf("Memcpy Bandwidth: %.2f GB/s\n", bw);
}

void benchmark_write(char *dst, size_t size) {
    double t0 = now_sec();
    for (int i = 0; i < REPEAT; ++i) {
        memset(dst, 1, size);
    }
    double t1 = now_sec();
    double bw = (double)size * REPEAT / (t1 - t0) / 1e9;
    printf("Write Bandwidth : %.2f GB/s\n", bw);
}

void benchmark_read(const char *src, size_t size) {
    volatile unsigned char sink = 0;
    double t0 = now_sec();
    for (int i = 0; i < REPEAT; ++i) {
        for (size_t j = 0; j < size; j += 64) {
            sink += src[j];
        }
    }
    double t1 = now_sec();
    double bw = (double)size * REPEAT / (t1 - t0) / 1e9;
    printf("Read Bandwidth  : %.2f GB/s\n", bw);
}

int main() {
    printf("Allocating %ld MB buffers...\n", SIZE / (1024 * 1024));
    char *a = (char *)malloc(SIZE);
    char *b = (char *)malloc(SIZE);
    if (!a || !b) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    memset(a, 1, SIZE);  // initialize memory

    benchmark_read(a, SIZE);
    benchmark_copy(b, a, SIZE);
    benchmark_write(b, SIZE);

    free(a);
    free(b);
    return 0;
}