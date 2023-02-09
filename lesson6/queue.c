#include "queue.h"
#include <stdlib.h>
#include <string.h>

#define QUEUE_SERIAL_NUMBER 0xdeadbeef

struct Queue
{
    uint32_t serial_number;

    size_t head;
    size_t tail;
    size_t num_elem;
    size_t max_elem;
    size_t elem_size;

    Queue_Observability counters;
    uint8_t buffer[]; /* FAM */
};

Queue* queue_new(const size_t max_elem, const size_t elem_size)
{
    if (max_elem == 0)
        return NULL;

    if (elem_size == 0)
        return NULL;

    Queue* const q = calloc(1, sizeof(*q) + elem_size * max_elem);
    if (q == NULL)
        return NULL;

    q->max_elem = max_elem;
    q->elem_size = elem_size;
    q->head = 0;
    q->tail = 0;
    q->num_elem = 0;
    q->counters = (Queue_Observability){0};

    q->serial_number = QUEUE_SERIAL_NUMBER;

    return q;
}

void queue_delete(Queue* const q)
{
    if (q == NULL)
        return;

    free(q);
}

bool queue_is_active(const Queue* const q)
{
    if (q == NULL)
        return false;

    return q->serial_number == QUEUE_SERIAL_NUMBER;
}

bool queue_is_empty(const Queue* const q)
{
    if (q == NULL)
        return true;

    if (!queue_is_active(q))
        return true;

    return q->num_elem == 0;
}

bool queue_is_full(const Queue* const q)
{
    if (q == NULL)
        return false;

    if (!queue_is_active(q))
        return false;

    return q->num_elem == q->max_elem;
}

int queue_enqueue(Queue* const restrict q, const void* const restrict elem)
{
    if (q == NULL)
        return -1;

    if (elem == NULL)
        return -1;

    if (!queue_is_active(q))
        return -1;

    if (queue_is_full(q))
    {
        ++q->counters.no_enqueue_on_full;
        return 1;
    }

    uint8_t* const ptr = &q->buffer[q->head * q->elem_size];
    memcpy(ptr, elem, q->elem_size);

    ++q->head;
    if (q->head >= q->max_elem)
        q->head = 0;

    ++q->num_elem;
    ++q->counters.no_enqueue;

    return 0;
}

int queue_dequeue(Queue* const restrict q, void* const restrict elem)
{
    if (q == NULL)
        return -1;

    if (elem == NULL)
        return -1;

    if (!queue_is_active(q))
        return -1;

    if (queue_is_empty(q))
    {
        ++q->counters.no_dequeue_on_empty;
        return 1;
    }

    const uint8_t* const ptr = &q->buffer[q->tail * q->elem_size];
    memcpy(elem, ptr, q->elem_size);

    ++q->tail;
    if (q->tail >= q->max_elem)
        q->tail = 0;

    --q->num_elem;
    ++q->counters.no_dequeue;

    return 0;
}

Queue_Observability queue_get_observability(const Queue* const q)
{
    if (q == NULL)
        return (Queue_Observability){0};

    return q->counters;
}