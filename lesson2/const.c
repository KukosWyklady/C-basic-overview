#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

struct Foo
{
    int a;
    const int b;
    const char* const name;
};

static void func1(void);
static void func2(void);

// declaraction no obvious const like const int, int * const t
static void func3(const int t[], const struct Foo* foo, int a);
static const char* func4(const struct Foo* foo);

static void func1(void)
{
    int var1 = 5;
    var1++; // OK

    const int var2 = 5;
    // var2++; // ERROR

    // const is not constexpr, VLA
    int t[var2];
    (void)t;

    int* const ptr1 = &var1; // OK
    (*ptr1)++; // OK var can be changed
    // ptr++; // ERROR

    // ptr1 = &var2; // ERROR ptr pointing for const data

    const int* ptr2;
    ptr2 = &var1; // OK
    // (*ptr2)++; // ERROR
    ptr2 = &var2; // OK
    ptr2++; // OK, data are const, not ptr

    /* only reading as *ptr3 is allowed (ptr[i] also is allowed) */
    const int* const ptr3 = &var2;
    (void)ptr3;

    char *str = NULL;
    // const char** str_p = &str; // Warning char* and const char* are not the same
    char* const* str_p = &str; // OK
    (void)str_p;

    const int* const intp = NULL;
    const int * const * const intpp = &intp; // OK

    // .ro data
    const char* str2 = "Something";
    (void)str2;

    // .ro data
    char *str3 = "Something";
    (void)str3;

    // .stack
    char str4[] = "Something";
    (void)str4;

    // .stack
    const char str5[] = "Something";
    (void)str5;
}

static void func2(void)
{
    const struct Foo f1 = {.a = 5, .b = 10, .name = "Foo1"};
    // const struct Foo f1;
    // f1.a = 5; // ERROR
    (void)f1;

    const struct Foo f2 = {.a = 10, .b = 15, .name = "Foo2"};
    // f1 = f2; // ERROR

    // OK implicit cast  to void* :(
    memcpy(&f1, &f2, sizeof(f1));
    printf("Foo1 = {%d, %d, %s}\n", f1.a, f1.b, f1.name);

    const struct Foo* f3 = malloc(sizeof(*f3));
    // f3->a = 10; // ERROR

    free(f3);
}

// declaration const int OK, int * const OK :)
static void func3(const int t[const], const struct Foo* const foo, const int a)
{
    (void)t;
    (void)foo;
    (void)a;
}

// we can return const
static const char* func4(const struct Foo* const foo)
{
    return foo->name;
}

void const_main(void)
{
    func1();
    func2();
    func3((int[]){1, 2, 3}, &(const struct Foo){.a = 5, .b = 10, .name = "FOO"}, 10);

    const char* name1 = func4(&(const struct Foo){.a = 5, .b = 10, .name = "FOO"});
    (void)name1;
}
