#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <sys/time.h>
#include "heap.h"

bool compare_value(void* A,void* B)
{
    return (int)A > (int)B;
}

int main(int argc, char const *argv[])
{
    heap_handle_t heap = heap_create(compare_value);

    for(int i=0;i<10;i++)
    {
        heap_add(heap,(void*)i);
    }
    
    void** dump = heap_dump(heap);
    for(int i=0;i<heap_get_length(heap);i++)
    {
        printf("%d ",(int)dump[i]);
    }
    printf("\n");
    heap_dump_free(dump);

    heap_delete(heap,(void*)5);

    dump = heap_dump(heap);
    for(int i=0;i<heap_get_length(heap);i++)
    {
        printf("%d ",(int)dump[i]);
    }
    printf("\n");
    heap_dump_free(dump);

    heap_delete(heap,(void*)12);

    dump = heap_dump(heap);
    for(int i=0;i<heap_get_length(heap);i++)
    {
        printf("%d ",(int)dump[i]);
    }
    printf("\n");
    heap_dump_free(dump);


    while(heap_get_length(heap)>0)
    {
        int v = (int)heap_pop(heap);
    }

    heap_free(heap,NULL);
    return 0;
}
