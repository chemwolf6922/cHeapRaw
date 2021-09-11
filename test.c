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
    struct timeval now;
    gettimeofday(&now,NULL);
    long long start_ts = now.tv_sec*1000000 + now.tv_usec;
    for(int i=0;i<1000000;i++)
    {
        heap_add(heap,(void*)random());
    }
    gettimeofday(&now,NULL);
    long long now_ts = now.tv_sec*1000000 + now.tv_usec;
    printf("%lld us\n",now_ts - start_ts);
    start_ts = now_ts;
    while(heap_get_length(heap)>0)
    {
        int v = (int)heap_pop(heap);
    }
    gettimeofday(&now,NULL);
    now_ts = now.tv_sec*1000000 + now.tv_usec;
    printf("%lld us\n",now_ts - start_ts);
    heap_free(heap,NULL);
    return 0;
}
