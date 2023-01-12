#include <stdio.h>
#include <stdalign.h>
#include <stdint.h>
#include <stdlib.h>

struct Foo1
{
    uint32_t y;
    alignas(128) uint32_t x;
}; // y + padding + x + padding = 256

struct Foo2
{
    alignas(128) uint32_t x;
    uint32_t y;
}; // x + y + padding to 128 = 128

alignas(1024) struct Foo3 // not working
{
    alignas(128) uint32_t x;
    uint32_t y;
};

#define PRINT_SIZE_AND_ALIGNMENT(x) printf("sizeof(%s) = %zu, alingment(%s) = %zu\n", #x, sizeof(x), #x, alignof(x))

static void memory_test(void)
{
    PRINT_SIZE_AND_ALIGNMENT(int);
    PRINT_SIZE_AND_ALIGNMENT(uint32_t);
    PRINT_SIZE_AND_ALIGNMENT(struct Foo1);
    PRINT_SIZE_AND_ALIGNMENT(struct Foo2);
    PRINT_SIZE_AND_ALIGNMENT(struct Foo3);

    alignas(16 * 16 * 16) struct Foo3 x;
    PRINT_SIZE_AND_ALIGNMENT(x);
    printf("&x = %p\n", (void *)&x);

    int* t = aligned_alloc(16 * 16 * 16 * 16, 100 * sizeof(*t));
    PRINT_SIZE_AND_ALIGNMENT(t);
    PRINT_SIZE_AND_ALIGNMENT(*t);

    printf("t = %p\n", (void *)t);

    free(t);
}

void memory_main(void)
{
    memory_test();
}
