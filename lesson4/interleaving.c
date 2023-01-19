#include <stdio.h>
#include <assert.h>

// only for compiler
#define sw_mem_barrier() __asm__ volatile ("" : : : "memory")

// for cpu
#define hw_mem_barrier() __asm__ volatile ("mfence" : : : "memory")

#define single_thread_atomic_op(op) \
    do { \
        hw_mem_barrier(); \
        op; \
        hw_mem_barrier(); \
    } while (0)

static void single_thread_code(void);
static int single_thread_add(int a, int b);

static int single_thread_add(int a, int b)
{
    // side effect
    // printf("%d + %d = %d\n", a, b, a + b);

    return a + b;
}


// pipieline: https://en.wikipedia.org/wiki/Instruction_pipelining
// instruction reordering: https://en.wikipedia.org/wiki/Tomasulo_algorithm
static void single_thread_code(void)
{
    int a = 10;
    int b = 15;

    ++a;
    --b;
    int c = a + b;
    int d = c++;
    (void)d;

    int e = a + b + c + d; // ((a + b) + c) + d
    (void)e;

    int f = (a + b) + (c + d);
    (void)f;
    hw_mem_barrier();
    ++a;
    hw_mem_barrier();
    --b;
    c = a + b;
    d = c++;

    single_thread_atomic_op(++a);
    single_thread_atomic_op(--b);
    single_thread_atomic_op(c = a + b);
    single_thread_atomic_op(d = c++);

    c = single_thread_add(a, b);
    d = single_thread_add(a, b);
    f = single_thread_add(c, d);
}

static int n; // .bss section
static void multi_thread_code(void)
{
    // assumption: 2 threads
    int a = 10;
    int b = 15;

    ++a; // inc
    --b; // dec
    int c = a + b; // add, movl
    int d = c++; // movl, inc
    (void)d;

    // T1 ++a
    // T1 --b
    // T2 ++a

    // T1 ++a
    // T2 ++a

    // is there a possibility that assert will crash?
    if (++n >= 5)
        n = 0;

    // never claim: n range is [0; 4]
    assert(n < 5);

    // n = 3
    // T1 ++n // = 4
    // T1 after if
    // T2 ++n // = 5
    // T1 assert -> NOK
    // T2 if (...) n = 0
    // T2 OK
    ++n;
    if (n >= 5)
        n = 0;

     // never claim: n range is [0; 4]
    assert(n < 5);

    // n = 3
    // T1 temp = 3
    // T1 ++temp // 4
    // T2 temp = 3
    // T2 ++temp // 4
    // T1 assert OK
    // T2 assert OK
    int temp = n;
    ++temp;
    if (temp >= 5)
        temp = 0;

    n = temp;

    // never claim: n range is [0; 4]
    assert(n < 5);
}

void interleaving_main(void)
{
    single_thread_code();
    multi_thread_code();
}