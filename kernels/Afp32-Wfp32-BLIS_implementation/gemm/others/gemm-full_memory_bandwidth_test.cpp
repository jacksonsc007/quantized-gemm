#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <stdint.h>

// Size of memory to test (in bytes)
#define MEM_SIZE (1UL << 30) // 1GB
#define NUM_ITERATIONS 10
#define CACHE_LINE_SIZE 64

// Function prototypes
double test_sequential_read(uint8_t *buffer, size_t size);
double test_sequential_write(uint8_t *buffer, size_t size);
double test_strided_read(uint8_t *buffer, size_t size, int stride);
double test_strided_write(uint8_t *buffer, size_t size, int stride);
double test_random_read(uint8_t *buffer, size_t size);
double test_random_write(uint8_t *buffer, size_t size);
void fill_random(uint8_t *buffer, size_t size);

int main() {
    // Allocate memory buffer
    uint8_t *buffer = (uint8_t *)malloc(MEM_SIZE);
    if (!buffer) {
        fprintf(stderr, "Failed to allocate memory\n");
        return 1;
    }
    
    // Fill buffer with random data
    printf("Filling buffer with random data...\n");
    fill_random(buffer, MEM_SIZE);
    
    printf("Memory Bandwidth Tests (Buffer Size: %lu MB)\n", MEM_SIZE / (1024 * 1024));
    printf("------------------------------------------------\n");
    
    // Test sequential access
    printf("Sequential Read Bandwidth: %.2f MB/s\n", test_sequential_read(buffer, MEM_SIZE));
    printf("Sequential Write Bandwidth: %.2f MB/s\n", test_sequential_write(buffer, MEM_SIZE));
    
    // Test strided access (with different strides)
    for (int stride = 2; stride <= 32; stride *= 2) {
        printf("Strided Read (stride=%d) Bandwidth: %.2f MB/s\n", 
               stride, test_strided_read(buffer, MEM_SIZE, stride));
        printf("Strided Write (stride=%d) Bandwidth: %.2f MB/s\n", 
               stride, test_strided_write(buffer, MEM_SIZE, stride));
    }
    
    // Test random access
    printf("Random Read Bandwidth: %.2f MB/s\n", test_random_read(buffer, MEM_SIZE));
    printf("Random Write Bandwidth: %.2f MB/s\n", test_random_write(buffer, MEM_SIZE));
    
    // Clean up
    free(buffer);
    
    return 0;
}

// Fill buffer with random data
void fill_random(uint8_t *buffer, size_t size) {
    srand(time(NULL));
    for (size_t i = 0; i < size; i++) {
        buffer[i] = rand() % 256;
    }
}

// Test sequential read bandwidth
double test_sequential_read(uint8_t *buffer, size_t size) {
    volatile uint8_t sink; // To prevent compiler optimization
    clock_t start, end;
    double total_time = 0;
    
    for (int iter = 0; iter < NUM_ITERATIONS; iter++) {
        start = clock();
        for (size_t i = 0; i < size; i += CACHE_LINE_SIZE) {
            sink = buffer[i]; // Read one cache line at a time
        }
        end = clock();
        total_time += (double)(end - start) / CLOCKS_PER_SEC;
    }
    
    double avg_time = total_time / NUM_ITERATIONS;
    return (size / (1024.0 * 1024.0)) / avg_time; // MB/s
}

// Test sequential write bandwidth
double test_sequential_write(uint8_t *buffer, size_t size) {
    clock_t start, end;
    double total_time = 0;
    
    for (int iter = 0; iter < NUM_ITERATIONS; iter++) {
        start = clock();
        for (size_t i = 0; i < size; i += CACHE_LINE_SIZE) {
            buffer[i] = (uint8_t)i; // Write one cache line at a time
        }
        end = clock();
        total_time += (double)(end - start) / CLOCKS_PER_SEC;
    }
    
    double avg_time = total_time / NUM_ITERATIONS;
    return (size / (1024.0 * 1024.0)) / avg_time; // MB/s
}

// Test strided read bandwidth
double test_strided_read(uint8_t *buffer, size_t size, int stride) {
    volatile uint8_t sink; // To prevent compiler optimization
    clock_t start, end;
    double total_time = 0;
    size_t stride_bytes = stride * CACHE_LINE_SIZE;
    
    for (int iter = 0; iter < NUM_ITERATIONS; iter++) {
        start = clock();
        for (size_t i = 0; i < size; i += stride_bytes) {
            sink = buffer[i]; // Read with stride
        }
        end = clock();
        total_time += (double)(end - start) / CLOCKS_PER_SEC;
    }
    
    double avg_time = total_time / NUM_ITERATIONS;
    return (size / (1024.0 * 1024.0)) / avg_time; // MB/s
}

// Test strided write bandwidth
double test_strided_write(uint8_t *buffer, size_t size, int stride) {
    clock_t start, end;
    double total_time = 0;
    size_t stride_bytes = stride * CACHE_LINE_SIZE;
    
    for (int iter = 0; iter < NUM_ITERATIONS; iter++) {
        start = clock();
        for (size_t i = 0; i < size; i += stride_bytes) {
            buffer[i] = (uint8_t)i; // Write with stride
        }
        end = clock();
        total_time += (double)(end - start) / CLOCKS_PER_SEC;
    }
    
    double avg_time = total_time / NUM_ITERATIONS;
    return (size / (1024.0 * 1024.0)) / avg_time; // MB/s
}

// Test random read bandwidth
double test_random_read(uint8_t *buffer, size_t size) {
    volatile uint8_t sink; // To prevent compiler optimization
    clock_t start, end;
    double total_time = 0;
    
    // Create random access pattern
    size_t num_accesses = size / CACHE_LINE_SIZE;
    size_t *indices = (size_t *)malloc(num_accesses * sizeof(size_t));
    
    for (size_t i = 0; i < num_accesses; i++) {
        indices[i] = (rand() % (size / CACHE_LINE_SIZE)) * CACHE_LINE_SIZE;
    }
    
    for (int iter = 0; iter < NUM_ITERATIONS; iter++) {
        start = clock();
        for (size_t i = 0; i < num_accesses; i++) {
            sink = buffer[indices[i]]; // Random read
        }
        end = clock();
        total_time += (double)(end - start) / CLOCKS_PER_SEC;
    }
    
    free(indices);
    double avg_time = total_time / NUM_ITERATIONS;
    return (size / (1024.0 * 1024.0)) / avg_time; // MB/s
}

// Test random write bandwidth
double test_random_write(uint8_t *buffer, size_t size) {
    clock_t start, end;
    double total_time = 0;
    
    // Create random access pattern
    size_t num_accesses = size / CACHE_LINE_SIZE;
    size_t *indices = (size_t *)malloc(num_accesses * sizeof(size_t));
    
    for (size_t i = 0; i < num_accesses; i++) {
        indices[i] = (rand() % (size / CACHE_LINE_SIZE)) * CACHE_LINE_SIZE;
    }
    
    for (int iter = 0; iter < NUM_ITERATIONS; iter++) {
        start = clock();
        for (size_t i = 0; i < num_accesses; i++) {
            buffer[indices[i]] = (uint8_t)i; // Random write
        }
        end = clock();
        total_time += (double)(end - start) / CLOCKS_PER_SEC;
    }
    
    free(indices);
    double avg_time = total_time / NUM_ITERATIONS;
    return (size / (1024.0 * 1024.0)) / avg_time; // MB/s
}