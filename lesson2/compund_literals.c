#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

typedef struct Foo
{
    int x;
    int y;
    int z;
} Foo;

static void f1(const int t[], size_t n);
static void f2(const int (*t)[4]);
static void f3(const Foo* foo);
static void f4(const char* day);
static void f5(int k);

static void f1(const int t[], size_t n)
{
    printf("T[%zu] =", n);
    for (size_t i = 0; i < n; ++i)
        printf("%4d", t[i]);
    printf("\n");
}

static void f2(const int (*t)[4])
{
    printf("T[4] = {%d %d %d %d}\n", (*t)[0], (*t)[1], (*t)[2], (*t)[3]);
}

static void f3(const Foo* foo)
{
    printf("Foo = {%d %d %d}\n", foo->x, foo->y, foo->z);
}

static void f4(const char* day)
{
    printf("DAY = %s\n", day);
}

void f5(int k)
{
    printf("K = %d\n", k);
}

#define DAYS \
    (const char* const []){"mon", "tue", "wed", "thu", "fri", "sat", "sun"}

#define K_SEQ \
    (int[]){100, 200, 300, 400, 500, 600, 700, 800, 900, 1000}

/***** MIN with type safe check ****/
#define min_normal(a, b) (a) < (b) ? (a) : (b)
#define min(a, b) check_types(a, b) * 0 + (a) < (b) ? (a) : (b)
#define check_types(a, b) (&(a) - &(b))

void compund_literals_main(void)
{
    f1((const int[]){1, 2, 3, 4}, 4);
    f2(&(const int[4]){1, 2, 3, 4});
    f3(&(const Foo){.x = 3, .y = 5, .z = 7});

    f4(DAYS[3]);
    volatile int k = 5;
    f5(K_SEQ[k]);

    printf("MIN(%d, %d) = %d\n", 2, 3, min((int){2}, (int){3}));
}
