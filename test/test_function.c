#include <stdio.h>
#include <assert.h>
#include "test_common.h"

static int tests_passed = 0;
static int tests_failed = 0;

#define ASSERT_EQ(actual, expected, msg) do { \
    if ((actual) == (expected)) { \
        tests_passed++; \
    } else { \
        tests_failed++; \
        printf("  FAIL: %s (expected %d, got %d)\n", msg, (int)(expected), (int)(actual)); \
    } \
} while(0)

#define ASSERT_NULL(ptr, msg) do { \
    if ((ptr) == NULL) { \
        tests_passed++; \
    } else { \
        tests_failed++; \
        printf("  FAIL: %s (expected NULL)\n", msg); \
    } \
} while(0)

#define ASSERT_NOT_NULL(ptr, msg) do { \
    if ((ptr) != NULL) { \
        tests_passed++; \
    } else { \
        tests_failed++; \
        printf("  FAIL: %s (expected non-NULL)\n", msg); \
    } \
} while(0)

static void test_empty_heap(void)
{
    printf("test_empty_heap\n");
    heap_handle_t heap = heap_new(compare_value, set_pos, get_pos);

    ASSERT_EQ(heap_get_length(heap), 0, "new heap length is 0");
    ASSERT_NULL(heap_pop(heap), "pop from empty heap returns NULL");
    ASSERT_NULL(heap_get(heap), "get from empty heap returns NULL");

    heap_free(heap, NULL, NULL);
}

static void test_single_element(void)
{
    printf("test_single_element\n");
    heap_handle_t heap = heap_new(compare_value, set_pos, get_pos);

    test_value_t v1 = {.v = 42};
    heap_add(heap, &v1);

    ASSERT_EQ(heap_get_length(heap), 1, "length after one add");
    ASSERT_EQ(((test_value_t*)heap_get(heap))->v, 42, "get returns the element");
    ASSERT_EQ(((test_value_t*)heap_pop(heap))->v, 42, "pop returns the element");
    ASSERT_EQ(heap_get_length(heap), 0, "length after pop");
    ASSERT_NULL(heap_pop(heap), "pop from now-empty heap returns NULL");

    heap_free(heap, NULL, NULL);
}

static void test_min_heap_ordering(void)
{
    printf("test_min_heap_ordering\n");
    heap_handle_t heap = heap_new(compare_value, set_pos, get_pos);

    test_value_t v1 = {.v = 5};
    test_value_t v2 = {.v = 3};
    test_value_t v3 = {.v = 8};
    test_value_t v4 = {.v = 1};
    test_value_t v5 = {.v = 10};

    heap_add(heap, &v1);
    heap_add(heap, &v2);
    heap_add(heap, &v3);
    heap_add(heap, &v4);
    heap_add(heap, &v5);

    ASSERT_EQ(heap_get_length(heap), 5, "length after 5 adds");

    /* min heap: should pop in ascending order */
    ASSERT_EQ(((test_value_t*)heap_pop(heap))->v, 1, "pop 1st smallest");
    ASSERT_EQ(((test_value_t*)heap_pop(heap))->v, 3, "pop 2nd smallest");
    ASSERT_EQ(((test_value_t*)heap_pop(heap))->v, 5, "pop 3rd smallest");
    ASSERT_EQ(((test_value_t*)heap_pop(heap))->v, 8, "pop 4th smallest");
    ASSERT_EQ(((test_value_t*)heap_pop(heap))->v, 10, "pop 5th smallest");
    ASSERT_EQ(heap_get_length(heap), 0, "length after popping all");

    heap_free(heap, NULL, NULL);
}

static void test_delete(void)
{
    printf("test_delete\n");
    heap_handle_t heap = heap_new(compare_value, set_pos, get_pos);

    test_value_t v1 = {.v = 1};
    test_value_t v2 = {.v = 10};
    test_value_t v3 = {.v = 2};
    test_value_t v4 = {.v = 11};
    test_value_t v5 = {.v = 12};
    test_value_t v6 = {.v = 4};
    test_value_t v7 = {.v = 3};

    heap_add(heap, &v1);
    heap_add(heap, &v2);
    heap_add(heap, &v3);
    heap_add(heap, &v4);
    heap_add(heap, &v5);
    heap_add(heap, &v6);
    heap_add(heap, &v7);

    ASSERT_EQ(heap_get_length(heap), 7, "length after 7 adds");

    /* delete middle element */
    heap_delete(heap, &v4);
    ASSERT_EQ(heap_get_length(heap), 6, "length after delete");

    /* delete top element */
    heap_delete(heap, &v1);
    ASSERT_EQ(heap_get_length(heap), 5, "length after delete top");

    /* delete last/largest element */
    heap_delete(heap, &v5);
    ASSERT_EQ(heap_get_length(heap), 4, "length after delete last");

    /* remaining: 2, 3, 4, 10 */
    ASSERT_EQ(((test_value_t*)heap_pop(heap))->v, 2, "pop after deletes 1st");
    ASSERT_EQ(((test_value_t*)heap_pop(heap))->v, 3, "pop after deletes 2nd");
    ASSERT_EQ(((test_value_t*)heap_pop(heap))->v, 4, "pop after deletes 3rd");
    ASSERT_EQ(((test_value_t*)heap_pop(heap))->v, 10, "pop after deletes 4th");
    ASSERT_NULL(heap_pop(heap), "pop from empty after all ops");

    heap_free(heap, NULL, NULL);
}

static void test_get_without_remove(void)
{
    printf("test_get_without_remove\n");
    heap_handle_t heap = heap_new(compare_value, set_pos, get_pos);

    test_value_t v1 = {.v = 7};
    test_value_t v2 = {.v = 2};

    heap_add(heap, &v1);
    heap_add(heap, &v2);

    /* get should return top without removing */
    ASSERT_EQ(((test_value_t*)heap_get(heap))->v, 2, "get returns min");
    ASSERT_EQ(heap_get_length(heap), 2, "length unchanged after get");
    ASSERT_EQ(((test_value_t*)heap_get(heap))->v, 2, "get again returns same");

    heap_free(heap, NULL, NULL);
}

static void test_duplicate_values(void)
{
    printf("test_duplicate_values\n");
    heap_handle_t heap = heap_new(compare_value, set_pos, get_pos);

    test_value_t v1 = {.v = 5};
    test_value_t v2 = {.v = 5};
    test_value_t v3 = {.v = 3};
    test_value_t v4 = {.v = 3};

    heap_add(heap, &v1);
    heap_add(heap, &v2);
    heap_add(heap, &v3);
    heap_add(heap, &v4);

    ASSERT_EQ(heap_get_length(heap), 4, "length with duplicates");
    ASSERT_EQ(((test_value_t*)heap_pop(heap))->v, 3, "pop 1st duplicate");
    ASSERT_EQ(((test_value_t*)heap_pop(heap))->v, 3, "pop 2nd duplicate");
    ASSERT_EQ(((test_value_t*)heap_pop(heap))->v, 5, "pop 3rd duplicate");
    ASSERT_EQ(((test_value_t*)heap_pop(heap))->v, 5, "pop 4th duplicate");

    heap_free(heap, NULL, NULL);
}

static void test_add_after_pop(void)
{
    printf("test_add_after_pop\n");
    heap_handle_t heap = heap_new(compare_value, set_pos, get_pos);

    test_value_t v1 = {.v = 10};
    test_value_t v2 = {.v = 20};
    test_value_t v3 = {.v = 5};

    heap_add(heap, &v1);
    heap_add(heap, &v2);

    ASSERT_EQ(((test_value_t*)heap_pop(heap))->v, 10, "pop first");

    /* add a smaller element after popping */
    heap_add(heap, &v3);
    ASSERT_EQ(heap_get_length(heap), 2, "length after pop+add");
    ASSERT_EQ(((test_value_t*)heap_pop(heap))->v, 5, "new min after re-add");
    ASSERT_EQ(((test_value_t*)heap_pop(heap))->v, 20, "remaining element");

    heap_free(heap, NULL, NULL);
}

int main(void)
{
    test_empty_heap();
    test_single_element();
    test_min_heap_ordering();
    test_delete();
    test_get_without_remove();
    test_duplicate_values();
    test_add_after_pop();

    printf("\nResults: %d passed, %d failed\n", tests_passed, tests_failed);
    return tests_failed > 0 ? 1 : 0;
}
