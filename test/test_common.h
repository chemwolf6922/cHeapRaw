#ifndef TEST_COMMON_H
#define TEST_COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "heap.h"

typedef struct
{
    heap_pos_t pos;
    int v;
} test_value_t;

static inline bool compare_value(void* A, void* B)
{
    return ((test_value_t*)A)->v > ((test_value_t*)B)->v;
}

static inline void set_pos(void* value, heap_pos_t pos)
{
    ((test_value_t*)value)->pos = pos;
}

static inline heap_pos_t get_pos(void* value)
{
    return ((test_value_t*)value)->pos;
}

#endif
