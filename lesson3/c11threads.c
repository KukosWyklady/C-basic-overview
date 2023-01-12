// https://en.cppreference.com/w/c/thread

#include <threads.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>

#define N 10

static tss_t thread_global_data; // create before thread start, each thread has diff copy
static once_flag tprint_flag = ONCE_FLAG_INIT;
static mtx_t mutex;

static _Thread_local int c; // thread safe, each thread has diff copy

static void thread_data_init(void)
{
    int *data = malloc(sizeof(*data) * N);
    for (size_t i = 0; i < N; ++i)
        data[i] = (int)(i + 1);

    tss_set(thread_global_data, data);

    printf("before C = %d\n", c);
    c = rand() % 100;
    printf("after C = %d\n", c);
}

static void tprint(void)
{
    printf("TPRINT\n");
}

static int thread_func(void* arg)
{
    (void)arg;

    call_once(&tprint_flag, tprint);
    thread_data_init();

    thrd_yield(); // hint for scheduler that this thread should not be execute right now
    thrd_sleep(&(struct timespec){.tv_sec = rand() % 3 + 1}, NULL); // sleep 1 sec

    int *data = tss_get(thread_global_data);

    mtx_lock(&mutex); // for printf
    for (size_t i = 0; i < N; ++i)
        printf("%4d", data[i]);
    printf("\n");

    mtx_unlock(&mutex);

    // tss is thread safe
    for (size_t i = 0; i < N; ++i)
        data[i] += rand() % 100;

    tss_set(thread_global_data, data);

    data = tss_get(thread_global_data);

    mtx_lock(&mutex); // for printf
    for (size_t i = 0; i < N; ++i)
        printf("%4d", data[i]);
    printf("\n");

    mtx_unlock(&mutex);

    return 0;
}


static void thread_test(void)
{
    thrd_t t1;
    thrd_t t2;

    tss_create(&thread_global_data, free); // auto free with "free"
    mtx_init(&mutex, mtx_plain);

    if (thrd_create(&t1, thread_func, NULL) != thrd_success)
        return;

    if (thrd_create(&t2, thread_func, NULL) != thrd_success)
        return;

    thrd_join(t1, NULL);
    // thrd_detach(t2);
    thrd_join(t2, NULL);
}

// https://en.cppreference.com/w/c/atomic
static void atomic_operations(void)
{
    // _Atomic int a = ATOMIC_VAR_INIT(10); // C11 UB
    _Atomic int a = 10; // C17
    _Atomic int b = 15; // C17

    a += 2; // UB _Atomic is not atomic!!!

    atomic_fetch_add(&a, 2);
    atomic_fetch_add(&a, atomic_load(&b));
    atomic_store(&b, atomic_load(&a));
}


void c11threads_main(void)
{
    srand(time((NULL)));

    atomic_operations();
    thread_test();
}