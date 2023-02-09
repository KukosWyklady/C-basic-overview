#include "queue.h"
#include <assert.h>
#include <string.h>

void queue_main_test(void);

static void queue_create_test(void);
static void queue_empty_full_on_empty_test(void);
static void queue_single_enqueue_test(void);
static void queue_enqueue_to_full_test(void);
static void queue_single_dequeue_test(void);
static void queue_dequeue_to_empty_test(void);
static void queue_is_active_test(void);

static void queue_create_test(void)
{
    {
        register const size_t n_elem = 10;
        register const size_t elem_size = sizeof(int);

        Queue* const q = queue_new(n_elem, elem_size);

        assert(q != NULL);
        assert(queue_is_active(q) == true);

        const Queue_Observability counters = queue_get_observability(q);
        assert(memcmp(&counters, &(Queue_Observability){0}, sizeof(counters)) == 0);

        queue_delete(q);
    }

    {
        register const size_t n_elem = 0;
        register const size_t elem_size = sizeof(int);

        Queue* const q = queue_new(n_elem, elem_size);

        assert(q == NULL);

        const Queue_Observability counters = queue_get_observability(q);
        assert(memcmp(&counters, &(Queue_Observability){0}, sizeof(counters)) == 0);

        queue_delete(q);
    }

    {
        register const size_t n_elem = 10;
        register const size_t elem_size = 0;

        Queue* const q = queue_new(n_elem, elem_size);

        assert(q == NULL);

        const Queue_Observability counters = queue_get_observability(q);
        assert(memcmp(&counters, &(Queue_Observability){0}, sizeof(counters)) == 0);

        queue_delete(q);
    }

    {
        register const size_t n_elem = 0;
        register const size_t elem_size = 0;

        Queue* const q = queue_new(n_elem, elem_size);

        assert(q == NULL);

        const Queue_Observability counters = queue_get_observability(q);
        assert(memcmp(&counters, &(Queue_Observability){0}, sizeof(counters)) == 0);

        queue_delete(q);
    }
}

static void queue_is_active_test(void)
{
    assert(queue_is_active(NULL) == false);

    register const size_t max_elems = 10;
    register const size_t elem_size = sizeof(int);

    Queue* const q = queue_new(max_elems, elem_size);

    assert(q != NULL);
    assert(queue_is_active(q) == true);

    queue_delete(q);
}

static void queue_empty_full_on_empty_test(void)
{
    register const size_t n_elem = 10;
    register const size_t elem_size = sizeof(int);

    Queue* const q = queue_new(n_elem, elem_size);

    assert(q != NULL);
    assert(queue_is_active(q) == true);

    assert(queue_is_empty(q));
    assert(!queue_is_full(q));

    const Queue_Observability counters = queue_get_observability(q);
    assert(memcmp(&counters, &(Queue_Observability){0}, sizeof(counters)) == 0);

    queue_delete(q);
}

static void queue_single_enqueue_test(void)
{
    const size_t value_to_insert = 17;
    register const size_t n_elem = 10;
    register const size_t elem_size = sizeof(int);

    Queue* const q = queue_new(n_elem, elem_size);

    assert(q != NULL);
    assert(queue_is_active(q) == true);

    assert(queue_enqueue(NULL, &value_to_insert) != 0);
    assert(queue_enqueue(q, NULL) != 0);
    assert(queue_enqueue(NULL, NULL) != 0);

    assert(queue_is_empty(q));
    assert(!queue_is_full(q));

    assert(queue_enqueue(q, &value_to_insert) == 0);

    assert((volatile int)value_to_insert == 17);

    assert(!queue_is_empty(q));
    assert(!queue_is_full(q));

    const Queue_Observability counters = queue_get_observability(q);
    assert(memcmp(&counters, &(Queue_Observability){.no_enqueue = 1}, sizeof(counters)) == 0);

    queue_delete(q);
}

static void queue_enqueue_to_full_test(void)
{
    const size_t value_to_insert = 41;
    register const size_t n_elem = 10;
    register const size_t elem_size = sizeof(int);

    Queue* const q = queue_new(n_elem, elem_size);
    assert(q != NULL);
    assert(queue_is_active(q) == true);

    for (size_t i = 0; i < n_elem; ++i)
    {
        const int val = i;
        assert(queue_enqueue(q, &val) == 0);
        assert((volatile int)val == (int)i);

        assert(!queue_is_empty(q));
        assert(queue_is_full(q) == (i == n_elem - 1));

        Queue_Observability counters = queue_get_observability(q);
        assert(memcmp(&counters, &(Queue_Observability){.no_enqueue = i + 1}, sizeof(counters)) == 0);
    }

    assert(!queue_is_empty(q));
    assert(queue_is_full(q));

    assert(queue_enqueue(q, &value_to_insert) != 0);
    assert((volatile int)value_to_insert == 41);

    const Queue_Observability counters = queue_get_observability(q);
    assert(memcmp(&counters, &(Queue_Observability){.no_enqueue = n_elem, .no_enqueue_on_full = 1}, sizeof(counters)) == 0);

    queue_delete(q);
}

static void queue_single_dequeue_test(void)
{
    const int value_to_insert = 17;
    register const size_t n_elem = 10;
    register const size_t elem_size = sizeof(int);

    int val = 0;
    Queue* const q = queue_new(n_elem, elem_size);

    assert(q != NULL);
    assert(queue_is_active(q) == true);

    assert(queue_is_empty(q));
    assert(!queue_is_full(q));

    assert(queue_dequeue(q, &val) != 0);

    assert(queue_enqueue(q, &value_to_insert) == 0);
    assert(!queue_is_empty(q));
    assert(!queue_is_full(q));

    assert(queue_dequeue(NULL, &val) != 0);
    assert(queue_dequeue(q, NULL) != 0);
    assert(queue_dequeue(NULL, NULL) != 0);
    assert(queue_dequeue(q, &val) == 0);
    assert(val == value_to_insert);

    const Queue_Observability counters = queue_get_observability(q);
    assert(memcmp(&counters, &(Queue_Observability){.no_enqueue = 1, .no_dequeue = 1, .no_dequeue_on_empty = 1}, sizeof(counters)) == 0);

    assert(queue_is_empty(q));
    assert(!queue_is_full(q));

    queue_delete(q);
}

static void queue_dequeue_to_empty_test(void)
{
    register const size_t n_elem = 10;
    register const size_t elem_size = sizeof(int);

    Queue* const q = queue_new(n_elem, elem_size);
    assert(q != NULL);
    assert(queue_is_active(q) == true);

    for (size_t i = 0; i < n_elem; ++i)
    {
        const size_t val = i;
        assert(queue_enqueue(q, &val) == 0);
        assert(!queue_is_empty(q));
        assert(queue_is_full(q) == (i == n_elem - 1));

        const Queue_Observability counters = queue_get_observability(q);
        assert(memcmp(&counters, &(Queue_Observability){.no_enqueue = i + 1}, sizeof(counters)) == 0);
    }

    assert(!queue_is_empty(q));
    assert(queue_is_full(q));

    size_t counter = 0;
    while (!queue_is_empty(q))
    {
        int val = 0;

        assert(queue_dequeue(q, &val) == 0);
        assert(val == (int)counter);
        assert(!queue_is_full(q));

        ++counter;

        const Queue_Observability counters = queue_get_observability(q);
        assert(memcmp(&counters, &(Queue_Observability){.no_enqueue = n_elem, .no_dequeue = counter}, sizeof(counters)) == 0);
    }

    assert(counter == n_elem);
    const Queue_Observability counters = queue_get_observability(q);
    assert(memcmp(&counters, &(Queue_Observability){.no_enqueue = n_elem, .no_dequeue = n_elem}, sizeof(counters)) == 0);

    queue_delete(q);
}

void queue_main_test(void)
{
    queue_create_test();
    queue_is_active_test();
    queue_empty_full_on_empty_test();
    queue_single_enqueue_test();
    queue_enqueue_to_full_test();
    queue_single_dequeue_test();
    queue_dequeue_to_empty_test();
}