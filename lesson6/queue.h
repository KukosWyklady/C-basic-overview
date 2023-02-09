#ifndef QUEUE_H
#define QUEUE_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct Queue_Observability
{
    size_t no_enqueue;
    size_t no_enqueue_on_full;
    size_t no_dequeue;
    size_t no_dequeue_on_empty;
} Queue_Observability;

typedef struct Queue Queue;

Queue* queue_new(size_t n_elem, size_t elem_size);
void queue_delete(Queue* q);

bool queue_is_active(const Queue* q);

bool queue_is_empty(const Queue* q);
bool queue_is_full(const Queue* q);

int queue_enqueue(Queue* restrict q, const void* restrict elem);
int queue_dequeue(Queue* restrict q, void* restrict elem);

Queue_Observability queue_get_observability(const Queue* q);

#endif
