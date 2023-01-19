#define _GNU_SOURCE // sysycall

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <time.h>

// https://www.geeksforgeeks.org/mutex-lock-for-linux-thread-synchronization/
// https://www.geeksforgeeks.org/difference-between-binary-semaphore-and-mutex/
// https://en.wikipedia.org/wiki/Spinlock
typedef void* (*pthread_f)(void*);

#define THREAD_NUMBERS 2

// static pthread_spinlock_t spinlock;
static pthread_mutex_t mutex;

static void test(pthread_f func);
static void* thread_func_unsync(void* arg);
static void* thread_func_sync(void* arg);
static void* thread_func_force_unlock(void* arg);

static void* thread_func_unsync(void* arg)
{
    (void)arg;

    pid_t tid = syscall(__NR_gettid);
    printf("[%d] OPEN \n", tid);

    int sec = rand() % 5;
    printf("[%d] WORK %d sec\n", tid, sec);
    sleep(sec);
    printf("[%d] FINISH \n", tid);

    printf("[%d] CLOSE\n", tid);

	return NULL;
}

static void* thread_func_sync(void* arg)
{
    (void)arg;

    pid_t tid = syscall(__NR_gettid);

    pthread_mutex_lock(&mutex); // has owner !!! while (!unlocked) { sleep(); } // IDLE station
    // pthread_spin_lock(&spinlock); // has owner !!! while (!unlocked) { } // 100% CPU Utilization
    printf("[%d] OPEN \n", tid);

    int sec = rand() % 5;
    printf("[%d] WORK %d sec\n", tid, sec);
    sleep(sec);
    printf("[%d] FINISH \n", tid);

    printf("[%d] CLOSE\n", tid);

    pthread_mutex_unlock(&mutex);
    // pthread_spin_unlock(&spinlock);

	return NULL;
}

static void* thread_func_force_unlock(void* arg)
{
    (void)arg;

    pid_t tid = syscall(__NR_gettid);

    // force unlock UB
    // If the mutex type is PTHREAD_MUTEX_NORMAL [...] If a thread attempts to unlock a mutex that it has not locked or a mutex which is unlocked, undefined behavior results.
    pthread_mutex_unlock(&mutex);
    // pthread_spin_unlock(&spinlock);

    pthread_mutex_lock(&mutex);
    // pthread_spin_lock(&spinlock);
    printf("[%d] OPEN \n", tid);

    int sec = rand() % 5;
    printf("[%d] WORK %d sec\n", tid, sec);
    sleep(sec);
    printf("[%d] FINISH \n", tid);

    printf("[%d] CLOSE\n", tid);

    pthread_mutex_unlock(&mutex);
    // pthread_spin_unlock(&spinlock);

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

void mutex_main(void)
{
    srand(time(NULL));

    printf("TEST UNSYNC\n\n");
    test(thread_func_unsync);
    printf("\n\n");

    printf("TEST SYNC\n\n");
    pthread_mutex_init(&mutex, NULL);
    // pthread_spin_init(&spinlock, 0);
    test(thread_func_sync);
    pthread_mutex_destroy(&mutex);
    // pthread_spin_destroy(&spinlock);
    printf("\n\n");

    printf("TEST FORCE UNLOCK\n\n");
    pthread_mutex_init(&mutex, NULL);
    // pthread_spin_init(&spinlock, 0);
    pthread_mutex_lock(&mutex);
    // pthread_spin_lock(&spinlock);
    test(thread_func_force_unlock);
    pthread_mutex_destroy(&mutex);
    // pthread_spin_destroy(&spinlock);
    printf("\n\n");
}
