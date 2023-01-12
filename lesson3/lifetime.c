#include <stdio.h>
#include <string.h>
#include <stddef.h>


struct Foo1
{
    int a;
    int b;
};

static struct Foo1 foo1_fabric(int a, int b)
{
    return (struct Foo1){.a = a, .b = b};
}


struct Foo2
{
    int n;
    int arr[2];
};

static struct Foo2 foo2_fabric(int n, int pattern)
{
    struct Foo2 foo2 = (struct Foo2){.n = n};
    for (size_t i = 0; i < sizeof(foo2.arr) / sizeof(foo2.arr[0]); ++i)
        foo2.arr[i] = pattern;

    return foo2;
}

/*

    Struct and union objects with array members (either direct or members of nested struct/union members)
    that are designated by non-lvalue expressions, have temporary lifetime.
    Temporary lifetime begins when the expression that refers to such object
    is evaluated and ends at the next sequence point
    (until C11)when the containing full expression or full declarator ends (since C11).

    Any attempt to modify an object with temporary lifetime results in undefined behavior.

*/


static void lifetime_c99(void)
{
    printf("Foo1 {%d %d}\n", foo1_fabric(10, 15).a, foo1_fabric(10, 15).b); // OK
    printf("Foo2 {%d {%d %d}}\n", foo2_fabric(10, 5).n, foo2_fabric(10, 5).arr[0], foo2_fabric(10, 5).arr[1]); // OK
    printf("Foo2 &arr %p\n", (void *)foo2_fabric(10, 5).arr); // harmless UB
    printf("Foo2 arr as string %s\n", (char *)foo2_fabric(10, 0).arr); // UB

    int *ptr = foo2_fabric(10, 15).arr; // UB
    (void)ptr;

    ptr = &foo2_fabric(10, 15).arr[0]; // UB
    (void)ptr;

    struct Foo2 foo2_temp = foo2_fabric(10, 5);
    printf("Foo2 {%d {%d %d}}\n", foo2_temp.n, foo2_temp.arr[0], foo2_temp.arr[1]); // OK
    printf("Foo2 &arr %p\n", (void *)foo2_temp.arr); // OK

    ptr = &foo2_temp.arr[0]; // OK
    (void)ptr;

    // printf("Foo1 {%d %d}\n", ++foo1_fabric(10, 15).a, --foo1_fabric(10, 15).b); // Compile time error
    printf("Foo2 {%d {%d %d}}\n", foo2_fabric(10, 5).n, foo2_fabric(10, 5).arr[0]++, --foo2_fabric(10, 5).arr[1]); // UB
}

static void lifetime_c11(void)
{
    printf("Foo1 {%d %d}\n", foo1_fabric(10, 15).a, foo1_fabric(10, 15).b); // OK
    printf("Foo2 {%d {%d %d}}\n", foo2_fabric(10, 5).n, foo2_fabric(10, 5).arr[0], foo2_fabric(10, 5).arr[1]); // OK

    // UB in C99 OK in C11
    printf("Foo2 &arr %p\n", (void *)foo2_fabric(10, 5).arr); // OK
    printf("Foo2 arr as string %s\n", (char *)foo2_fabric(10, 0).arr); // OK

    int *ptr = foo2_fabric(10, 15).arr; // UB
    (void)ptr;

    ptr = &foo2_fabric(10, 15).arr[0]; // UB
    (void)ptr;

    struct Foo2 foo2_temp = foo2_fabric(10, 5);
    printf("Foo2 {%d {%d %d}}\n", foo2_temp.n, foo2_temp.arr[0], foo2_temp.arr[1]); // OK
    printf("Foo2 &arr %p\n", (void *)foo2_temp.arr); // OK

    ptr = &foo2_temp.arr[0]; // OK
    (void)ptr;

    // printf("Foo1 {%d %d}\n", ++foo1_fabric(10, 15).a, --foo1_fabric(10, 15).b); // Compile time error
    printf("Foo2 {%d {%d %d}}\n", foo2_fabric(10, 5).n, foo2_fabric(10, 5).arr[0]++, --foo2_fabric(10, 5).arr[1]); // UB
}


void lifetime_main(void)
{
    lifetime_c99();
    lifetime_c11();
}