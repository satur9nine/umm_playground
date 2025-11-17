// Build: gcc -O2 -pthread threads_work.c -o threads_work

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>

#include "umm_malloc_cfg.h"
#include "umm_malloc.h"

#define NTHREADS   4
#define ITERATIONS 100

#define HEAP_SIZE (512*1024)

static uint8_t G_HEAP[HEAP_SIZE];

void *UMM_MALLOC_CFG_HEAP_ADDR = G_HEAP;
uint32_t UMM_MALLOC_CFG_HEAP_SIZE = HEAP_SIZE;

// Empty function you can fill later.
// noinline + barrier to discourage the compiler from optimizing the call away.
__attribute__((noinline)) static void work(void) {
    int s = rand() % 500;
    void *p = umm_malloc(s);
    int r = rand() % 20;
    usleep(100 * r);
    umm_free(p);
}

static void *thread_main(void *arg) {
    (void)arg;
    for (unsigned long i = 0; i < ITERATIONS; ++i) {
        work();
    }
    return NULL;
}

int main(void) {
    srand(77);
    umm_init();

    pthread_t threads[NTHREADS];

    for (int i = 0; i < NTHREADS; ++i) {
        int rc = pthread_create(&threads[i], NULL, thread_main, NULL);
        if (rc != 0) {
            fprintf(stderr, "pthread_create failed (thread %d), rc=%d\n", i, rc);
            // Join any threads already started before exiting
            for (int j = 0; j < i; ++j) pthread_join(threads[j], NULL);
            return EXIT_FAILURE;
        }
    }

    for (int i = 0; i < NTHREADS; ++i) {
        pthread_join(threads[i], NULL);
    }

    umm_info(NULL, true);

    int usage_metric = umm_usage_metric();
    int fragmentation_metric = umm_fragmentation_metric();
    size_t free_heap_bytes = umm_free_heap_size();
    size_t max_free_block = umm_max_free_block_size();

    printf("UMM usage metric %d, fragmentation metric %d, free heap %zu bytes, largest free block %zu bytes\n",
        usage_metric,
        fragmentation_metric,
        free_heap_bytes,
        max_free_block);

    // All threads finished; program exits.
    return EXIT_SUCCESS;
}
