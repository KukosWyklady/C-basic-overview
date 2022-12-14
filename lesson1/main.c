#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "macros.h"

static int a;

static void foo_1(void)
{
    printf("%s\n", __func__);
}

static void foo_2(void)
{
    printf("%s\n", __func__);
}

#define CMP_TEMPLATE(type) \
static int TOKEN_CONCAT(cmp_, type)(const void *a, const void *b) { return *(type *)b - *(type *)a; }

CMP_TEMPLATE(int)
CMP_TEMPLATE(char)

#define MACRO_DEBUG(macro) printf("[MACRO DEBUG] " #macro " --> " TOKEN_TO_STRING(macro) "\n")

static void macro_test(void)
{
    int n = N;
    (void)n;
    print();

    printf("print() --> %s\n", TOKEN_TO_STRING(print()));
    TOKEN_CONCAT(foo_, 1)();

    printf("CMP_TEMPLATE(int) --> %s\n", TOKEN_TO_STRING(CMP_TEMPLATE(int)));

    MACRO_DEBUG(print());
    MACRO_DEBUG(CMP_TEMPLATE(int));
}

static void c_keywords(void)
{
    auto int v1 = 0; // auto = local on .stack. DO NOT USE THIS. NOT RESPECTED ANYMORE
    const int v2 = 0; // const = cannot change
    register int v3 = 0; // register CANNOT TAKE ADDRESS
    int* restrict v4 = 0; // since C99 // restrict POINTERS IN THIS SCOPE DONT OVBERLAP
    static int v5 = 0; // static = .bss | .data
    volatile int v6 = 0; // volatile = DO NOT CACHE + DO NOT OPTIMZIE IT
    extern int v7;
    const register int v8 = 100; // TRUE CONST IN C

    (void)v1;
    (void)v2;
    (void)v3;
    (void)v4;
    (void)v5;
    (void)v6;
    (void)v7;
    (void)v8;
}

static void c_types(void)
{

// https://en.wikipedia.org/wiki/C_data_types
// ANSI C defines the following sizes for the various primitive data types.

// char
// minimum signed range: -128 .. 127
// minimum unsigned range: 0 .. 255
// can be larger, but regardless of size, sizeof(char) == 1
// sizeof(unsigned char) == 1
// short
// minimum signed range: -32768 .. 32767
// minimum unsigned range: 0 .. 65535
// can be larger
// long
// minimum signed range: -2147483648 .. 2147483647
// minimum unsigned range: 0 .. 4294967295
// can be larger
// int
// must have, at a miminum, the same range as a short
// can have the same range as a long
// cannot have a range larger than a long
// short <= int, and int <= long, but short < long

    char c;
    unsigned char uc;
    signed char sc;

    short s;
    unsigned short us;

    int i;
    unsigned int ui;

    long l;
    unsigned long ul;

    long long ll; // C99
    unsigned long long ull; // C99

    float f;
    double d;
    long double ld;

    // stdbool.h since C99
    bool b;


    // stdint.h since C99
    int8_t i8;
    uint8_t ui8;
    int16_t i16;
    uint16_t ui16;
    int32_t i32;
    uint32_t ui32;
    int64_t i64;
    uint64_t ui64;

    uint_least8_t uil8;
    uint_fast8_t uif8;

    intmax_t im;
    uintmax_t uim;

    intptr_t ip;
    uintptr_t uip;

    // stddef.h since C99
    ptrdiff_t pd;
    size_t si;
    ssize_t sis;

#define PRINT_SIZE(expr) printf("sizeof(" TOKEN_TO_STRING((expr)) ") = %zu\n", sizeof(expr))

    // promotion
    PRINT_SIZE(c + 1);
    PRINT_SIZE(c + 1UL);
    PRINT_SIZE(ui32 + 1UL);
    PRINT_SIZE(i + d);

    // degradation
    PRINT_SIZE((short){i});
}

static void c_99_features(void)
{
    int n = 100;
    int t[n]; // VLA

    size_t row = 100;
    size_t column = 50;
    int *arr = malloc(sizeof(*arr) * column * row); // int arr[column][row]
    int (*vla2d)[column] = (int(*)[column])arr;
    vla2d[2][5] = 0; // translated to arr[2 * column + 5]

    struct  FAM
    {
        size_t len;
        uint8_t payload[]; // FAM
    };

    struct FAM *f = malloc(sizeof(*f) + 100 * sizeof(f->payload[0]));
    free(f);


    struct S
    {
        int a;
        int b;
        int c;
    };

    struct S s = {.a = 5, .c = 10}; // init list
    s = (struct S){.a = 11}; // Compund literal

    // inline function // since C99
}

int main(void)
{
    macro_test();

    c_types();
    c_99_features();

    return 0;
}
