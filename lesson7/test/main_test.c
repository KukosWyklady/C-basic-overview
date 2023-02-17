#include <assert.h>
#include <foo.h>

// #include <stdlib.h>

int main(void)
{
    // random tests

    // ADD
    assert(foo_add(0, 0) == 0);
    assert(foo_add(-1, 2) == 1);
    assert(foo_add(5, 3) == 8);

    // SUB
    assert(foo_sub(0, 0) == 0);
    assert(foo_sub(-1, 2) == -3);
    assert(foo_sub(5, 3) == 2);

    // MUL
    assert(foo_mul(0, 0) == 0);
    assert(foo_mul(-1, 2) == -2);
    assert(foo_mul(5, 3) == 15);

    // DIV
    assert(foo_div(0, 0) == 0);
    assert(foo_div(-1, 2) == 0);
    assert(foo_div(5, 3) == 1);

    // int *t = malloc(1000);
    // t[1111] = 1;

    return 0;
}
