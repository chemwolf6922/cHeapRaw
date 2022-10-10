#ifndef __HEAP_H
#define __HEAP_H

#include <stdint.h>
#include <stdbool.h>

typedef void* heap_handle_t;
typedef void* heap_pos_t;
/** relation of leaf->top */
typedef bool (*heap_compare_t)(void* A, void* B);
typedef void (*heap_set_pos_t)(void* value, heap_pos_t pos);
typedef heap_pos_t (*heap_get_pos_t)(void* value);
typedef void (*heap_free_value_t)(void* value, void* ctx);

heap_handle_t heap_new(heap_compare_t compare, heap_set_pos_t set_pos, heap_get_pos_t get_pos);
/** If free_value is NULL, no operation will be applied to the assigned value */
void heap_free(heap_handle_t handle,heap_free_value_t free_value,void* ctx);
/** Make sure that value is valid and can be used in set_pos */
int heap_add(heap_handle_t handle,void* value);
/** Make sure that value is still valid!! */
int heap_delete(heap_handle_t handle,void* value);
/** remove the top value and return it */
void* heap_pop(heap_handle_t handle);
/** return the top value without removing it */
void* heap_get(heap_handle_t handle);
int heap_get_length(heap_handle_t handle);
/** for debugging */
void** heap_dump(heap_handle_t handle);
void heap_dump_free(void** dump);

#endif