#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void* heap_handle_t;
typedef size_t heap_pos_t;
typedef bool (*heap_compare_t)(void* A, void* B);
typedef void (*heap_set_pos_t)(void* value, heap_pos_t pos);
typedef heap_pos_t (*heap_get_pos_t)(void* value);
typedef void (*heap_free_value_t)(void* value, void* ctx);

/**
 * @brief Create a new heap
 * 
 * @param compare  Relation of leaf(A) and top(B). So true == A > B -> min heap, true == A < B -> max heap
 * @param set_pos Set the position of the value to the value.
 * @param get_pos Get the position of the value from the value.
 * @return heap_handle_t 
 */
heap_handle_t heap_new(heap_compare_t compare, heap_set_pos_t set_pos, heap_get_pos_t get_pos);

/**
 * @brief Free the heap and all the nodes in it. If free_value is not NULL, it will be called to free the value of each node.
 * 
 * @param handle The heap handle to be freed.
 * @param free_value The function to free the value of each node. It will be called with the value and ctx as parameters.
 * @param ctx The context to be passed to free_value when it is called.
 */
void heap_free(heap_handle_t handle,heap_free_value_t free_value,void* ctx);

/**
 * @brief Add a value to the heap.
 * 
 * @param handle 
 * @param value 
 * @return int 
 */
int heap_add(heap_handle_t handle,void* value);

/**
 * @brief Delete a value from the heap.
 * 
 * @param handle 
 * @param value 
 * @return int 
 */
int heap_delete(heap_handle_t handle,void* value);

/**
 * @brief Pop the top value from the heap and return it. The caller is responsible for freeing the returned value if necessary.
 * 
 * @param handle 
 * @return void* 
 */
void* heap_pop(heap_handle_t handle);

/**
 * @brief Get the top value from the heap without removing it. The value is owned by the heap and should not be freed by the caller.
 * 
 * @param handle 
 * @return void* 
 */
void* heap_get(heap_handle_t handle);

/**
 * @brief Get the number of values in the heap.
 * 
 * @param handle 
 * @return int 
 */
int heap_get_length(heap_handle_t handle);

#ifdef __cplusplus
}
#endif
