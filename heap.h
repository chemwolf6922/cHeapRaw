#ifndef __HEAP_H
#define __HEAP_H

#include <stdint.h>
#include <stdbool.h>

typedef void* heap_handle_t;
// relation of leaf->top
typedef bool (*heap_compare_t)(void* A, void* B);
typedef void (*heap_free_value_t)(void* value);

heap_handle_t heap_create(heap_compare_t compare);
// If free_value is NULL, no operation will be applied to the assigned value
void heap_free(heap_handle_t handle,heap_free_value_t free_value);
bool heap_add(heap_handle_t handle,void* value);
void* heap_pop(heap_handle_t handle);
void* heap_get(heap_handle_t handle);
int heap_get_length(heap_handle_t handle);


#endif