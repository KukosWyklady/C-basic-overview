#define _GNU_SOURCE // sysycall

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <time.h>
#include <semaphore.h>

//  https://www.geeksforgeeks.org/use-posix-semaphores-c/

typedef void* (*pthread_f)(void*);

#define THREAD_NUMBERS 2

static sem_t semaphore;

static void test(pthread_f func);
static void* thread_func_sync(void* arg);

static void* thread_func_sync(void* arg)
{
    (void)arg;

    pid_t tid = syscall(__NR_gettid);

    sem_wait(&semaphore);
    printf("[%d] OPEN \n", tid);

    int sec = rand() % 5;
    printf("[%d] WORK %d sec\n", tid, sec);
    sleep(sec);
    printf("[%d] FINISH \n", tid);

    printf("[%d] CLOSE\n", tid);
    sem_post(&semaphore);

	return NULL;
}

static void* thread_func_force_unlock(void* arg)
{
    (void)arg;

    pid_t tid = syscall(__NR_gettid);

    // force unlock, THIS IS OK!!!!
    sem_post(&semaphore);

    sem_wait(&semaphore);
    printf("[%d] OPEN \n", tid);

    int sec = rand() % 5;
    printf("[%d] WORK %d sec\n", tid, sec);
    sleep(sec);
    printf("[%d] FINISH \n", tid);

    printf("[%d] CLOSE\n", tid);

    sem_post(&semaphore);

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

void semaphore_main(void)
{
    srand(time(NULL));

    printf("TEST SYNC\n\n");
    sem_init(&semaphore, 1, THREAD_NUMBERS - 1);
    test(thread_func_sync);
    sem_destroy(&semaphore);
    printf("\n\n");

    printf("TEST FORCE UNLOCK\n\n");
    sem_init(&semaphore, 1, THREAD_NUMBERS - 1);
    sem_wait(&semaphore);
    test(thread_func_force_unlock);
    sem_destroy(&semaphore);
    printf("\n\n");
}
