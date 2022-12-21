#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <inttypes.h>

static void f1(int *t);
static void f2(int t[]);
static void f3(int t[5]);
static void f4(int (*t)[5]);
static void fVla(size_t n, int t[*]);
static void fVla2D(size_t n, size_t m, int t[*][*]); // [*] mean VLA
static void fC99(int t[static 5]);

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))

static void f1(int *t)
{
    // 8
    printf("F1: SIZE=%zu\n", sizeof(t));
}

static void f2(int t[])
{
    // decay --> 8
    printf("F2: SIZE=%zu\n", sizeof(t));
}

static void f3(int t[5])
{
    // decay --> 8
    printf("F3: SIZE=%zu\n", sizeof(t));
}

static void f4(int (*t)[5])
{
    // sizeof(*t) --> sizeof(int[5]) --> 20
    printf("F4: SIZE=%zu\n", sizeof(*t));
}

static void fVla(size_t n, int t[n])
{
    // decay --> 8
    printf("FVLA: SIZE=%zu\n", sizeof(t));
}

static void fVla2D(size_t n, size_t m, int t[n][m])
{
    // sizeof(t) decay --> 8, sizeof(t[0]) = sizeof(int[m]) = 16
    printf("FVLA2D SIZE=%zu | SIZE t[0]=%zu\n", sizeof(t), sizeof(t[0]));

    // VLA is like int* not int**
    printf("\t&t[0][0] = %p\n", (void *)&t[0][0]);
    printf("\t&t[0][1] = %p(%" PRIdPTR ")\n", (void *)&t[0][1], (intptr_t)&t[0][1] - (intptr_t)&t[0][0]);
    printf("\t&t[1][0] = %p(%" PRIdPTR ")\n", (void *)&t[1][0], (intptr_t)&t[1][0] - (intptr_t)&t[0][0]);
}

static void fC99(int t[static 5])
{
    // decay --> 8
    printf("FC99 SIZE=%zu\n", sizeof(t));
}

void arr_ptr_main(void)
{
    int t[] = {1, 2, 3, 4, 5};
    f1(t);
    f2(t);
    f3(t);

    // correct way to create object (*)[5]
    int (*t5)[5] = malloc(sizeof(*t5));
    f4(t5);
    free(t5);

    // VLA
    size_t size = 10;
    int ttt[size];
    fVla(size, ttt);

    // VLA2D is waiting for int* or int[*][*] not int**
    int matrix[3][4] = {{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}};
    fVla2D(3, 4, matrix);

    fC99(t);
}
