#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

/************** HOW COMPILER CAN OPTIMZE RESTRICT POINTERS ********************************/

/*
    no restrct, working on memory and reloading each time
	add	DWORD PTR [rdi], 5
	mov	eax, DWORD PTR [rsi]
	add	eax, 5
	mov	DWORD PTR [rsi], eax
	add	eax, DWORD PTR [rdi]
*/
static int sum1(int* a, int* b)
{
    *a += 5;
    *b += 5;

    return *a + *b;
}

/*
    restrict. Working on registers without reloading
	mov	eax, DWORD PTR [rdi]
	mov	ecx, DWORD PTR [rsi]
	add	eax, 5
	lea	edx, 5[rcx]
	mov	DWORD PTR [rdi], eax
	add	eax, edx
	mov	DWORD PTR [rsi], edx
*/
static int sum2(int* restrict a, int* restrict b)
{
    *a += 5;
    *b += 5;

    return *a + *b;
}

/*
    If a == b return 12 else return 11
    So the compiler cannot optimize it

	mov	DWORD PTR [rdi], 5
	mov	DWORD PTR [rsi], 6
	mov	eax, DWORD PTR [rdi]
	add	eax, 6
*/
static int sum3(int* a, int* b)
{
    *a = 5;
    *b = 6;

    return *a + *b;
}

/*
    Always 11, full optimization

    mov	DWORD PTR [rdi], 5
	mov	eax, 11
	mov	DWORD PTR [rsi], 6
*/
static int sum4(int* restrict a, int* restrict b)
{
    *a = 5;
    *b = 6;

    return *a + *b;
}

/*************************************************************************************************/

/* the same type, restrict needed by developer */
static void f1(int* restrict a, int* restrict b)
{

}

/* diff types, restrict added by compiler */
static void f2(int* a, double* b)
{

}

/* diff types, but char* is compatible with all types, restrict needed by developer */
static void f3(int* restrict a, char* restrict b)
{

}

/* diff types, but void* is compatible with all types, restrict needed by developer */
static void f4(int* restrict a, void* restrict b)
{

}

static void f5(int* restrict a, int* restrict b)
{
    /* Compiler can analyze _a and _b right now, so restrict is by default, with more complicated code restrict wont be by default */
    int* _a = a;
    int* _b = b;

    (void)_a;
    (void)_b;
}

static void f6(int* a, int* b)
{
    /* NOK, here are restricted but function is not restrict */
    int* restrict _a = a;
    int* restrict _b = b;

    (void)_a;
    (void)_b;

    /* UB, after this they wont be restrict */
    _a = _b;
}

/*
    Block scope
    Restrict inside this block
    Example t[100]; a = &t[0], b = &t[50];
*/
#define f1_MACRO(a, b) \
    do { \
        int* restrict ____a = (a); \
        int* restrict ____b = (b); \
        \
    } while (0)

/*
    FILE SCOPE.
    Always will be restrict
*/
int* restrict ptr1;
int* restrict ptr2;
size_t array_size;

// int dst[restrict] == int * restrict dst
static void sumup(int dst[restrict], int src1[restrict], int src2[restrict], size_t n)
{
    for (size_t i = 0; i < n; ++i)
        dst[i] = src1[i] + src2[i];
}

// Restrict inside a structure is not a structure scope!!!
typedef struct Foo
{
    int* restrict a;
    int* restrict b;
} Foo;

static void fFoo(Foo* foo1, Foo* foo2)
{
    /*
        Here we have restrict members, they are restrict in this function not structure
        This means:
        foo1->a != foo2->a i foo1->b != foo2->b
        foo1->a != foo2->b i foo1->b != foo1->a
    */
}

void restrict_main(void)
{
    // malloc has special attribute to tell compiler that pointer is restrict
    int* restrict t = malloc(100 * sizeof(*t));
    ptr1 = t;
    ptr2 = t + 50;
    array_size = 50;
    free(t);


    int dst[10];
    int src1[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    int src2[10] = {9, 8, 7, 6, 5, 4, 3, 2, 1, 0};

    // OK
    sumup(dst, src1, src2, 10);

    // NOK, but UB is harmless (only read)
    sumup(dst, src1, src1, 10);

    // NOK
    sumup(src1, src1, src2, 10);

    // OK
    sumup(src1 + 5, src1, src2, 5);
}
