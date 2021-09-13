#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <sys/time.h>
#include "heap.h"

bool compare_value(void* A,void* B)
{
    return *(int*)A > *(int*)B;
}

int main(int argc, char const *argv[])
{
    heap_handle_t heap = heap_create(compare_value);

    int* value1 = malloc(sizeof(int));
    int* value2 = malloc(sizeof(int));
    int* value3 = malloc(sizeof(int));
    *value1 = 1;
    *value2 = 1;
    *value3 = 1;
    heap_add(heap,value1);
    heap_add(heap,value2);
    heap_add(heap,value3);
    heap_delete(heap,value1);
    heap_delete(heap,value2);
    heap_delete(heap,value3);

    heap_free(heap,NULL);
    return 0;
}
