#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <stdio.h>
#include <time.h>
#include <inttypes.h>
#include "test_common.h"

static uint64_t get_now_ns(void)
{
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    return ((uint64_t)now.tv_sec) * 1000000000 + (uint64_t)now.tv_nsec;
}

static void speed_test(int n)
{
    test_value_t* vs = malloc(sizeof(test_value_t) * n);
    for (int i = 0; i < n; i++)
        vs[i].v = rand();

    uint64_t start = get_now_ns();

    heap_handle_t heap = heap_new(compare_value, set_pos, get_pos);

    for (int i = 0; i < n; i++)
        heap_add(heap, &vs[i]);

    for (int i = 0; i < n / 2; i++)
        heap_delete(heap, &vs[i]);

    while (heap_pop(heap) != NULL) {};

    heap_free(heap, NULL, NULL);

    uint64_t end = get_now_ns();

    free(vs);

    printf("%d: %" PRIu64 " us\n", n, (end - start) / 1000);
}

int main(void)
{
    printf("Speed test start\n\n");

    speed_test(100);
    speed_test(1000);
    speed_test(10000);
    speed_test(100000);
    speed_test(1000000);

    printf("\nSpeed test End\n");
    return 0;
}
