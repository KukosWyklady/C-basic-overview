#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <time.h>
#include <assert.h>

_Static_assert(sizeof(int) == 4, "Why not 4bytes?");
static_assert(1 == 1, "WTF!");

struct Foo
{
    int n;
    union
    {
        uint32_t x;
        uint8_t arr[4];
    }; // anonymous (access like no union here: foo.x not foo.u.x)
};

// const int* is not compatible with int *
// int** is not with int* nor void*
// struct * is not ...
// a LOT of problems here :(
#define typename(x)  \
    _Generic((x),   _Bool: "_Bool", \
                    char: "char", \
                    signed char: "signed char", \
                    unsigned char: "unsigned char", \
                    short int: "short int", \
                    unsigned short int: "unsigned short int", \
                    int: "int", \
                    unsigned int: "unsigned int", \
                    long int: "long int", \
                    unsigned long int: "unsigned long int", \
                    long long int: "long long int", \
                    unsigned long long int: "unsigned long long int", \
                    float: "float", \
                    double: "double", \
                    long double: "long double", \
                    char *: "pointer to char", \
                    void *: "pointer to void", \
                    int *: "pointer to int", \
                    default: "other" \
            )

static void test_typename(void) {
    volatile size_t s;
    ptrdiff_t p;
    const intmax_t i;

    int ai[3] = {0};
    int* ptr = NULL;

    printf("size_t is '%s'\n", typename(s));
    printf("ptrdiff_t is '%s'\n", typename(p));
    printf("intmax_t is '%s'\n", typename(i));
    printf("ptr is '%s'\n", typename(ptr));

    printf("character constant is '%s'\n", typename('0'));
    printf("0x7FFFFFFF is '%s'\n", typename(0x7FFFFFFF));
    printf("0xFFFFFFFF is '%s'\n", typename(0xFFFFFFFF));
    printf("0x7FFFFFFFU is '%s'\n", typename(0x7FFFFFFFU));
    printf("array of int is '%s'\n", typename(ai));
}

_Noreturn void myexit(int error)
{
    exit(error);
}

void time_test(void)
{
    struct timespec ts;
    timespec_get(&ts, TIME_UTC);
    char buff[100];
    strftime(buff, sizeof buff, "%D %T", gmtime(&ts.tv_sec));
    printf("Current time: %s.%09ld UTC\n", buff, ts.tv_nsec);
    printf("Raw timespec.time_t: %jd\n", (intmax_t)ts.tv_sec);
    printf("Raw timespec.tv_nsec: %09ld\n", ts.tv_nsec);
}

void utils_main(void)
{
    test_typename();
    time_test();
}

// UTF supprt: https://en.cppreference.com/w/c/language/character_constant | https://en.cppreference.com/w/c/language/string_literal

// new funcitons with _s
// https://en.cppreference.com/w/c/string/byte/strncat
// errno_t strncat_s(char *restrict dest,
//                   rsize_t destsz,
//                   const char *restrict src,
//                   rsize_t count);