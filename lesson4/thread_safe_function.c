#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <time.h>
#include <assert.h>

typedef void* (*pthread_f)(void*);

#define THREAD_NUMBERS 2

typedef struct Foo
{
    pthread_mutex_t mutex;
    int n;
} Foo;

static Foo foo;

static void foo_init(Foo* foo);
static void foo_deinit(Foo* foo);
static void foo_add(Foo* foo, int n);
static void foo_setn(Foo* foo, int n);
static int foo_get_n(Foo* foo);

static void* thread_func_sync(void* arg);
static void test(pthread_f func);

static void foo_init(Foo* foo)
{
    foo->n = 0;
    pthread_mutex_init(&foo->mutex, NULL);
}

static void foo_deinit(Foo* foo)
{
    foo->n = 0;
    pthread_mutex_destroy(&foo->mutex);
}

static void foo_add(Foo* foo, int n)
{
    pthread_mutex_lock(&foo->mutex);

    foo->n += n;

    pthread_mutex_unlock(&foo->mutex);
}

static void foo_setn(Foo* foo, int n)
{
    pthread_mutex_lock(&foo->mutex);

    foo->n = n;

    pthread_mutex_unlock(&foo->mutex);
}

static int foo_get_n(Foo* foo)
{
    pthread_mutex_lock(&foo->mutex);

    int temp = foo-> n;

    pthread_mutex_unlock(&foo->mutex);

    return temp;
}

static void* thread_func_sync(void* arg)
{
    (void)arg;

    // OK or NOK?
    // foo->n = 3
    // T1 foo_add() // = 4
    // T1 after if
    // T2 foo_add() // = 5
    // T1 assert -> NOK
    // T2 if (...) n = 0
    // T2 OK
    foo_add(&foo, 1);
    if (foo_get_n(&foo) == 5)
        foo_setn(&foo, 0);

    assert(foo_get_n(&foo) < 5);

    return NULL;
}

static void test(pthread_f func)
{
    pthread_t tid[THREAD_NUMBERS];

    for (int i = 0; i < THREAD_NUMBERS; ++i)
        (void)pthread_create(&(tid[i]), NULL, func, NULL);

    for (int i = 0; i < THREAD_NUMBERS; ++i)
	    pthread_join(tid[i], NULL);
}

void thread_safe_function_main(void)
{
    foo_init(&foo);

    test(thread_func_sync);

    foo_deinit(&foo);
}
