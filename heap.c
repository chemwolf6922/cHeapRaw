#include "heap.h"
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

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
    heap_set_pos_t set_pos;
    heap_get_pos_t get_pos;
}heap_t;

/* function predefines */
static heap_node_t* heap_get_top(heap_t* heap,int n,bool* is_left);
static heap_node_t* heap_adjust_backward(heap_t* heap,heap_node_t* node);
static heap_node_t* heap_adjust_forward(heap_t* heap,heap_node_t* node);
static inline void heap_free_single_node(heap_node_t* node,heap_free_value_t free_value, void* ctx);
static heap_node_t* heap_new_node(void* value);
static void heap_free_node_chain(heap_node_t* node,heap_free_value_t free_value,void* ctx);
static void heap_dump_node_chain(heap_node_t* node,void** dump,int* i);

/* public functions */

heap_handle_t heap_new(heap_compare_t compare, heap_set_pos_t set_pos, heap_get_pos_t get_pos)
{
    heap_t* heap = NULL;
    if(compare == NULL || set_pos == NULL || get_pos == NULL)
        goto error;
    heap = malloc(sizeof(heap_t));
    if(!heap)
        goto error;
    heap->compare = compare;
    heap->set_pos = set_pos;
    heap->get_pos = get_pos;
    heap->entry = NULL;
    heap->length = 0;
    return (heap_handle_t)heap;
error:
    if(heap)
        free(heap);
    return NULL;
}

void heap_free(heap_handle_t handle,heap_free_value_t free_value,void* ctx)
{
    heap_t* heap = (heap_t*)handle;
    if(heap)
    {
        heap_free_node_chain(heap->entry,free_value,ctx);
        free(heap);
    }
}

int heap_add(heap_handle_t handle,void* value)
{
    heap_node_t* new_node = NULL;
    heap_t* heap = (heap_t*)handle;
    if(!heap)
        goto error;
    
    new_node = heap_new_node(value);
    if(!new_node)
        goto error;
    heap->set_pos(value,(heap_pos_t)new_node);

    if(heap->entry == NULL)
    {
        heap->entry = new_node;
        heap->length ++;
    }
    else
    {
        bool is_left;
        heap_node_t* top_node = heap_get_top(heap,++heap->length,&is_left);
        if(is_left)
            top_node->left = new_node;
        else
            top_node->right = new_node;
        new_node->top = top_node;
        heap_adjust_backward(heap,new_node);
    }
    return 0;
error:
    heap_free_single_node(new_node,NULL,NULL);
    return -1;
}

int heap_delete(heap_handle_t handle,void* value)
{
    heap_t* heap = (heap_t*)handle;
    if(!heap)
        goto error;
    heap_node_t* node = heap->get_pos(value);
    if(!node)
        goto error;

    if(heap->length == 1)
    {
        heap_free_single_node(node,NULL,NULL);
        heap->entry = NULL;
        heap->length --;
        goto finish;
    }
    /** find tail node */
    bool is_left;
    heap_node_t* tail_top_node = heap_get_top(heap,heap->length,&is_left);
    heap_node_t* tail_node = is_left?tail_top_node->left:tail_top_node->right;
    /** move tail value to node */
    node->value = tail_node->value;
    heap->set_pos(node->value,(heap_pos_t)node);
    /** free & detach tail node */
    heap_free_single_node(tail_node,NULL,NULL);
    heap->length --;
    if(is_left)
        tail_top_node->left = NULL;
    else
        tail_top_node->right = NULL;
    /** adjust heap structure */
    if(tail_node != node)
    {
        node = heap_adjust_backward(heap,node);
        heap_adjust_forward(heap,node);
    }  

finish:
    return 0;
error:
    return -1;
}

void* heap_pop(heap_handle_t handle)
{
    heap_t* heap = (heap_t*)handle;
    if(!heap)
        return NULL;
    void* result = NULL;
    if(heap->entry != NULL)
        result = heap->entry->value;
    if(heap->length == 1)
    {
        heap->length--;
        heap_free_single_node(heap->entry,NULL,NULL);
        heap->entry = NULL;
    }
    else if(heap->length > 1)
    {
        bool is_left;
        heap_node_t* tail_top_node = heap_get_top(heap,heap->length,&is_left);
        heap_node_t* tail_node = is_left?tail_top_node->left:tail_top_node->right;
        /** move tail value to entry */
        heap->entry->value = tail_node->value;
        heap->set_pos(heap->entry->value,heap->entry);
        /** free and detach the tail node */
        heap_free_single_node(tail_node,NULL,NULL);
        if(is_left)
            tail_top_node->left = NULL;
        else
            tail_top_node->right = NULL;
        heap->length--;
        /** adjust the new entry value */
        heap_adjust_forward(heap,heap->entry);
    }
    return result;
error:
    return NULL;
}

void* heap_get(heap_handle_t handle)
{
    heap_t* heap = (heap_t*)handle;
    if(!heap)
        return NULL;
   
    void* result = NULL;
    if(heap->entry != NULL)
        result = heap->entry->value;
    return result;
}

int heap_get_length(heap_handle_t handle)
{
    heap_t* heap = (heap_t*)handle;
    if(!heap)
        return -1;
    
    return heap->length;
}

void** heap_dump(heap_handle_t handle)
{
    heap_t* heap = (heap_t*)handle;
    if(!heap)
        return NULL;
   
    if(heap->length == 0)
        return NULL;
    void** dump = malloc(sizeof(void*)*heap->length);
    int i = 0;
    heap_dump_node_chain(heap->entry,dump,&i);
    return dump;
}

void heap_dump_free(void** dump)
{
    free(dump);
}

/* private functions */
static heap_node_t* heap_get_top(heap_t* heap,int n,bool* is_left)
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

static heap_node_t* heap_adjust_backward(heap_t* heap,heap_node_t* node)
{
    if(node->top == NULL)
        return node;
    if(heap->compare(node->top->value,node->value))
    {
        void* temp_value = node->value;
        node->value = node->top->value;
        node->top->value = temp_value;
        heap->set_pos(node->value,(heap_pos_t)node);
        heap->set_pos(node->top->value,(heap_pos_t)node->top);
        return heap_adjust_backward(heap,node->top);
    }
    else
    {
        return node;
    }
}

static heap_node_t* heap_adjust_forward(heap_t* heap,heap_node_t* node)
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
        heap->set_pos(node->value,(heap_pos_t)node);
        heap->set_pos(target_node->value,(heap_pos_t)target_node);
        return heap_adjust_forward(heap,target_node);
    }
    else
    {
        return node;
    }
}



static inline void heap_free_single_node(heap_node_t* node,heap_free_value_t free_value,void* ctx)
{
    if(node)
    {
        if(free_value)
            free_value(node->value,ctx);
        free(node);
    } 
}

static heap_node_t* heap_new_node(void* value)
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

static void heap_free_node_chain(heap_node_t* node,heap_free_value_t free_value,void* ctx)
{
    if(node == NULL)
        return;
    
    heap_node_t* left = node->left;
    heap_node_t* right = node->right;

    heap_free_single_node(node,free_value,ctx);
    
    heap_free_node_chain(left,free_value,ctx);
    heap_free_node_chain(right,free_value,ctx);
}

static void heap_dump_node_chain(heap_node_t* node,void** dump,int* i)
{
    if(node == NULL)
        return;
    dump[*i] = node->value;
    *i = *i + 1;
    heap_dump_node_chain(node->left,dump,i);
    heap_dump_node_chain(node->right,dump,i);
}
