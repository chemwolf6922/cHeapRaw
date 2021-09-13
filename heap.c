#include "heap.h"
#include <stdlib.h>
#include <stddef.h>

typedef struct heap_node_s{
    void* value;
    struct heap_node_s* left;
    struct heap_node_s* right;
    struct heap_node_s* top;
}heap_node_t;

typedef struct{
    struct heap_node_s* entry;
    int length;
    heap_compare_t compare;
}heap_t;

/* function predefines */
heap_node_t* heap_get_top(heap_t* heap,int n,bool* is_left);
void heap_adjust_backward(heap_t* heap,heap_node_t* node);
void heap_adjust_forward(heap_t* heap,heap_node_t* node);
void heap_free_node(heap_node_t* node,heap_free_value_t free_value);
heap_node_t* heap_new_node(void* value);
heap_node_t* heap_find_node(heap_node_t* node,void* value,bool* found);
void heap_dump_node(heap_node_t* node,void** dump,int* i);

/* public functions */

heap_handle_t heap_create(heap_compare_t compare)
{
    if(compare == NULL)
        return NULL;
    heap_t* heap = malloc(sizeof(heap_t));
    if(!heap)
        return NULL;
    heap->compare = compare;
    heap->entry = NULL;
    heap->length = 0;
    return (heap_handle_t)heap;
}

void heap_free(heap_handle_t handle,heap_free_value_t free_value)
{
    if(handle == NULL)
        return;
    heap_t* heap = (heap_t*)handle;
    heap_free_node(heap->entry,free_value);
    free(heap);
}

bool heap_add(heap_handle_t handle,void* value)
{
    if(!handle)
        return false;
    heap_t* heap = (heap_t*)handle;
    heap_node_t* new_node = heap_new_node(value);
    if(!new_node)
        return false;
    if(heap->entry == NULL)
    {
        heap->entry = new_node;
        heap->length ++;
        return true;
    }
    bool is_left;
    heap_node_t* node = heap_get_top(heap,++heap->length,&is_left);
    if(is_left)
        node->left = new_node;
    else
        node->right = new_node;
    new_node->top = node;
    heap_adjust_backward(heap,new_node);
    return true;
}

bool heap_delete(heap_handle_t handle,void* value)
{
    if(!handle)
        return false;
    heap_t* heap = (heap_t*)handle;
    bool found = false;
    heap_node_t* node = heap_find_node(heap->entry,value,&found);
    if(!node)
        return false;
    if(heap->length == 1)
    {
        heap_free_node(node,NULL);
        heap->entry = NULL;
        heap->length --;
        return true;
    }
    bool is_left;
    heap_node_t* tail_top_node = heap_get_top(heap,heap->length,&is_left);
    heap_node_t* tail_node = is_left?tail_top_node->left:tail_top_node->right;
    node->value = tail_node->value;
    heap_free_node(tail_node,NULL);
    heap->length --;
    if(is_left)
        tail_top_node->left = NULL;
    else
        tail_top_node->right = NULL;
    heap_adjust_forward(heap,node);
    return true;
}

void* heap_pop(heap_handle_t handle)
{
    if(!handle)
        return NULL;
    heap_t* heap = (heap_t*)handle;
    void* result = NULL;
    if(heap->entry != NULL)
        result = heap->entry->value;
    if(heap->length == 1)
    {
        heap->length--;
        heap_free_node(heap->entry,NULL);
        heap->entry = NULL;
    }
    else if(heap->length > 1)
    {
        bool is_left;
        heap_node_t* top_node = heap_get_top(heap,heap->length,&is_left);
        heap_node_t* node = is_left?top_node->left:top_node->right;
        heap->entry->value = node->value;
        heap_free_node(node,NULL);
        if(is_left)
            top_node->left = NULL;
        else
            top_node->right = NULL;
        heap->length--;
        heap_adjust_forward(heap,heap->entry);
    }
    return result;
}

void* heap_get(heap_handle_t handle)
{
    if(!handle)
        return NULL;
    heap_t* heap = (heap_t*)handle;
    void* result = NULL;
    if(heap->entry != NULL)
        result = heap->entry->value;
    return result;
}

int heap_get_length(heap_handle_t handle)
{
    if(!handle)
        return -1;
    heap_t* heap = (heap_t*)handle;
    return heap->length;
}

void** heap_dump(heap_handle_t handle)
{
    if(!handle)
        return NULL;
    heap_t* heap = (heap_t*)handle;
    if(heap->length == 0)
        return NULL;
    void** dump = malloc(sizeof(void*)*heap->length);
    int i = 0;
    heap_dump_node(heap->entry,dump,&i);
    return dump;
}

void heap_dump_free(void** dump)
{
    free(dump);
}

/* private functions */
heap_node_t* heap_get_top(heap_t* heap,int n,bool* is_left)
{
    if(n==1)
        return NULL;
    heap_node_t* node = heap->entry;
    int layers = 32 - __builtin_clz(n);
    for(int i=layers-2;i>0;i--)
    {
        if((n>>i)&1)
        {
            node = node->right;
        }
        else
        {
            node = node->left;
        }
    }
    *is_left = !(n&1);
    return node;
}

void heap_adjust_backward(heap_t* heap,heap_node_t* node)
{
    if(node->top == NULL)
        return;
    if(heap->compare(node->top->value,node->value))
    {
        void* temp_value = node->value;
        node->value = node->top->value;
        node->top->value = temp_value;
        heap_adjust_backward(heap,node->top);
    }
}

void heap_adjust_forward(heap_t* heap,heap_node_t* node)
{
    heap_node_t* target_node = NULL;
    if(node->left != NULL && node->right != NULL)
    {
        heap_node_t* less_node = NULL;
        if(heap->compare(node->left->value,node->right->value))
        {
            less_node = node->right;
        }
        else
        {
            less_node = node->left;
        }
        if(heap->compare(node->value,less_node->value))
        {
            target_node = less_node;
        }
    }
    else if(node->left != NULL && node->right == NULL)
    {
        if(heap->compare(node->value,node->left->value))
        {
            target_node = node->left;
        }
    }
    else if(node->left == NULL && node->right != NULL)
    {
        if(heap->compare(node->value,node->right->value))
        {
            target_node = node->right;
        }
    }
    if(target_node != NULL)
    {
        void* temp_value = node->value;
        node->value = target_node->value;
        target_node->value = temp_value;
        heap_adjust_forward(heap,target_node);
    }
}

void heap_free_node(heap_node_t* node,heap_free_value_t free_value)
{
    if(node == NULL)
        return;
    
    heap_node_t* left = node->left;
    heap_node_t* right = node->right;

    if(free_value != NULL)
        free_value(node->value);
    free(node);
    
    heap_free_node(left,free_value);
    heap_free_node(right,free_value);
}

heap_node_t* heap_new_node(void* value)
{
    heap_node_t* node = malloc(sizeof(heap_node_t));
    if(!node)
        return NULL;
    node->left = NULL;
    node->right = NULL;
    node->top = NULL;
    node->value = (void*)value;
    return node;
}

heap_node_t* heap_find_node(heap_node_t* node,void* value,bool* found)
{
    if(*found)
        return NULL;
    if(node == NULL)
        return NULL;
    if(node->value == value)
    {
        *found = true;
        return node;
    }
    heap_node_t* find_left = heap_find_node(node->left,value,found);
    if(find_left)
        return find_left;
    heap_node_t* find_right = heap_find_node(node->right,value,found);
    if(find_right)
        return find_right;
    return NULL;
}

void heap_dump_node(heap_node_t* node,void** dump,int* i)
{
    if(node == NULL)
        return;
    dump[*i] = node->value;
    *i = *i + 1;
    heap_dump_node(node->left,dump,i);
    heap_dump_node(node->right,dump,i);
}
