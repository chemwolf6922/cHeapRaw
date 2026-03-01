#include "heap.h"
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#define HEAP_DEFAULT_CAPACITY (16)
#define GET_PARENT_POS(pos) (((pos)-1)/2)
#define GET_LEFT_CHILD_POS(pos) ((pos)*2+1)
#define GET_RIGHT_CHILD_POS(pos) ((pos)*2+2)

typedef struct{
    void** values;
    size_t length;
    size_t capacity;
    heap_compare_t compare;
    heap_set_pos_t set_pos;
    heap_get_pos_t get_pos;
}heap_t;

/* function predefines */
static size_t heap_adjust_backward(heap_t* heap, size_t pos);
static size_t heap_adjust_forward(heap_t* heap, size_t node);

/* public functions */

heap_handle_t heap_new(heap_compare_t compare, heap_set_pos_t set_pos, heap_get_pos_t get_pos)
{
    heap_t* heap = NULL;
    if(compare == NULL || set_pos == NULL || get_pos == NULL)
        goto error;
    heap = malloc(sizeof(heap_t));
    if(!heap)
        goto error;
    memset(heap,0,sizeof(heap_t));
    heap->compare = compare;
    heap->set_pos = set_pos;
    heap->get_pos = get_pos;
    heap->length = 0;
    heap->capacity = HEAP_DEFAULT_CAPACITY;
    heap->values = malloc(sizeof(void*) * heap->capacity);
    if (!heap->values)
        goto error;
    return (heap_handle_t)heap;
error:
    if(heap)
    {
        if (heap->values)
            free(heap->values);
        free(heap);
    }
    return NULL;
}

void heap_free(heap_handle_t handle,heap_free_value_t free_value,void* ctx)
{
    heap_t* heap = (heap_t*)handle;
    if(heap)
    {
        if (heap->values)
        {
            for (size_t i = 0; i < heap->length; i++)
            {
                if (free_value)
                    free_value(heap->values[i], ctx);
            }
            free(heap->values);
        }
        free(heap);
    }
}

int heap_add(heap_handle_t handle,void* value)
{
    heap_t* heap = (heap_t*)handle;
    if (!heap || !value)
        return -1;

    if (heap->length == heap->capacity)
    {
        size_t new_capacity = heap->capacity * 2;
        void** new_values = realloc(heap->values, sizeof(void*) * new_capacity);
        if (!new_values)
            return -1;
        heap->values = new_values;
        heap->capacity = new_capacity;
    }

    size_t pos = heap->length;
    heap->length++;
    heap->values[pos] = value;
    heap->set_pos(value, pos);
    heap_adjust_backward(heap, pos);
    return 0;
}

int heap_delete(heap_handle_t handle,void* value)
{
    heap_t* heap = (heap_t*)handle;
    if (!heap || !value)
        return -1;

    size_t pos = heap->get_pos(value);
    if (pos >= heap->length)
        return -1;

    if (heap->length == 1)
    {
        heap->length--;
        /** No need to reduce capacity, as capacity is already at minimum if length == 1 */
        return 0;
    }

    /** Reduce heap length */
    heap->length--;
    /** Fix heap structure if pos is not the old tail */
    if (pos < heap->length)
    {
        /** Move old tail to node */
        heap->values[pos] = heap->values[heap->length];
        heap->set_pos(heap->values[pos], pos);
        /** Adjust structure */
        pos = heap_adjust_backward(heap, pos);
        heap_adjust_forward(heap, pos);
    }
    /** Reduce heap capacity */
    if (heap->length < heap->capacity / 4 && heap->capacity > HEAP_DEFAULT_CAPACITY)
    {
        size_t new_capacity = heap->capacity / 2;
        if (new_capacity < HEAP_DEFAULT_CAPACITY)
            new_capacity = HEAP_DEFAULT_CAPACITY;
        void** new_values = realloc(heap->values, sizeof(void*) * new_capacity);
        if (new_values)
        {
            heap->values = new_values;
            heap->capacity = new_capacity;
        }
    }
    return 0;
}

void* heap_pop(heap_handle_t handle)
{
    heap_t* heap = (heap_t*)handle;
    if(!heap)
        return NULL;
    if (heap->length == 0)
        return NULL;

    void* result = heap->values[0];
    heap_delete(handle, result);
    return result;
}

void* heap_get(heap_handle_t handle)
{
    heap_t* heap = (heap_t*)handle;
    if(!heap)
        return NULL;    
    if (heap->length == 0)
        return NULL;

    return heap->values[0];
}

int heap_get_length(heap_handle_t handle)
{
    heap_t* heap = (heap_t*)handle;
    if(!heap)
        return -1;
    
    return heap->length;
}

/* private functions */

static size_t heap_adjust_backward(heap_t* heap, size_t pos)
{
    if (pos == 0)
        return pos;
    size_t top_pos = GET_PARENT_POS(pos);
    if (heap->compare(heap->values[pos], heap->values[top_pos]))
        return pos;
    /** Flip pos and top_pos */
    void* temp_value = heap->values[pos];
    heap->values[pos] = heap->values[top_pos];
    heap->values[top_pos] = temp_value;
    heap->set_pos(heap->values[pos], pos);
    heap->set_pos(heap->values[top_pos], top_pos);
    return heap_adjust_backward(heap, top_pos);
}

static size_t heap_adjust_forward(heap_t* heap, size_t pos)
{
    size_t left_pos = GET_LEFT_CHILD_POS(pos);
    size_t right_pos = GET_RIGHT_CHILD_POS(pos);
    size_t min_pos = pos;
    if (left_pos < heap->length && heap->compare(heap->values[min_pos], heap->values[left_pos]))
    {
        min_pos = left_pos;
    }
    if (right_pos < heap->length && heap->compare(heap->values[min_pos], heap->values[right_pos]))
    {
        min_pos = right_pos;
    }
    if (min_pos == pos)
        return pos;
    
    /** Flip pos and min_pos */
    void* temp_value = heap->values[pos];
    heap->values[pos] = heap->values[min_pos];
    heap->values[min_pos] = temp_value;
    heap->set_pos(heap->values[pos], pos);
    heap->set_pos(heap->values[min_pos], min_pos);
    return heap_adjust_forward(heap, min_pos);
}
