#define _GNU_SOURCE // sysycall

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <time.h>
#include <stdbool.h>

#define WAREHOUSE_CAPACITY  5

typedef struct Warehouse
{
    int buffer[WAREHOUSE_CAPACITY];
    size_t size;
    size_t head;
    size_t tail;
    pthread_mutex_t mutex;
    pthread_cond_t can_produce;
    pthread_cond_t can_consume;
} Warehouse;


static Warehouse* warehouse_create(void);
static void warehouse_destroy(Warehouse* wh);
static bool warehouse_is_full(const Warehouse* wh);
static bool warehouse_is_empty(const Warehouse* wh);
static void warehouse_put(Warehouse* wh, int product);
static int warehouse_get(Warehouse* wh);
static void warehouse_lock(Warehouse* wh);
static void warehouse_unlock(Warehouse* wh);
static void warehouse_call_producer(Warehouse* wh);
static void warehouse_call_consumer(Warehouse* wh);
static void warehouse_wait_for_producer(Warehouse* wh);
static void warehouse_wait_for_consumer(Warehouse* wh);

static void* thread_producer(void *arg);
static void* thread_consumer(void *arg);

static Warehouse* warehouse_create(void)
{
    Warehouse* warehouse = malloc(sizeof(*warehouse));
    if (warehouse == NULL)
        return NULL;

    *warehouse = (Warehouse){.buffer = {0},
                             .size = 0,
                             .head = 0,
                             .tail = 0,
                             .mutex = PTHREAD_MUTEX_INITIALIZER,
                             .can_produce = PTHREAD_COND_INITIALIZER,
                             .can_consume = PTHREAD_COND_INITIALIZER
                            };

    return warehouse;
}

static void warehouse_destroy(Warehouse* wh)
{
    pthread_cond_destroy(&wh->can_consume);
    pthread_cond_destroy(&wh->can_produce);
    pthread_mutex_destroy(&wh->mutex);

    free(wh);
}

static bool warehouse_is_full(const Warehouse* wh)
{
    return wh->size == WAREHOUSE_CAPACITY;
}

static bool warehouse_is_empty(const Warehouse* wh)
{
    return wh->size == 0;
}

static void warehouse_put(Warehouse* wh, int product)
{
    if (warehouse_is_full(wh))
        return;

    wh->buffer[wh->head] = product;
    wh->head = (wh->head + 1) % WAREHOUSE_CAPACITY;
    ++wh->size;
}

static int warehouse_get(Warehouse* wh)
{
    if (warehouse_is_empty(wh))
        return -1;

    const int product = wh->buffer[wh->tail];
    wh->tail = (wh->tail + 1) % WAREHOUSE_CAPACITY;
    --wh->size;

    return product;
}

static void warehouse_lock(Warehouse* wh)
{
    pthread_mutex_lock(&wh->mutex);
}

static void warehouse_unlock(Warehouse* wh)
{
    pthread_mutex_unlock(&wh->mutex);
}

static void warehouse_call_producer(Warehouse* wh)
{
    pthread_cond_signal(&wh->can_produce);
}

static void warehouse_call_consumer(Warehouse* wh)
{
    pthread_cond_signal(&wh->can_consume);
}

static void warehouse_wait_for_producer(Warehouse* wh)
{
    pthread_cond_wait(&wh->can_consume, &wh->mutex);
}

static void warehouse_wait_for_consumer(Warehouse* wh)
{
    pthread_cond_wait(&wh->can_produce, &wh->mutex);
}

static void* thread_producer(void *arg)
{
    Warehouse* wh = *(Warehouse**)arg;

    while (true)
    {

        pid_t tid = syscall(__NR_gettid);
        printf("\t[%d] PRODUCER\n", tid);

        int sec = rand() % 5;
        printf("\t[%d] Working %d sec\n", tid, sec);
        sleep(sec);

        int product = rand() % 100;
        printf("\t[%d] Fisnish with product: %d\n", tid, product);

        printf("\t[%d] Waiting for warehouse\n", tid);

        warehouse_lock(wh);
        if (warehouse_is_full(wh))
        {
            printf("\t[%d] FULL, still waiting\n", tid);
            warehouse_wait_for_consumer(wh);
        }

        printf("\t[%d] There is a space for product %d\n", tid, product);
        warehouse_put(wh, product);

        warehouse_call_consumer(wh);
        warehouse_unlock(wh);
        printf("\t[%d] Getting out from warehouse\n", tid);
    }

    return NULL;
}

static void* thread_consumer(void *arg)
{
    Warehouse* wh = *(Warehouse**)arg;

    while(true)
    {
        pid_t tid = syscall(__NR_gettid);

        printf("[%d] CONSUMER \n", tid);
        warehouse_lock(wh);

        if (warehouse_is_empty(wh))
        {
            printf("[%d] Empty, still waiting\n", tid);
            warehouse_wait_for_producer(wh);
        }

        const int product = warehouse_get(wh);
        printf("[%d] Taking product %d\n", tid, product);

        warehouse_call_producer(wh);
        warehouse_unlock(wh);

        printf("[%d] Getiing out from warehouse\n", tid);

        int sec = rand() % 5;
        printf("[%d] Working with product %d sec\n", tid, sec);
        sleep(sec);
    }

    return NULL;
}

void pcp_main(void)
{
    srand(time(NULL));
    Warehouse* wh = warehouse_create();

    pthread_t prod, cons;
    pthread_create(&prod, NULL, thread_producer, (void*)&wh);
    pthread_create(&cons, NULL, thread_consumer, (void*)&wh);

    pthread_join(prod, NULL);
    pthread_join(cons, NULL);

    warehouse_destroy(wh);
}