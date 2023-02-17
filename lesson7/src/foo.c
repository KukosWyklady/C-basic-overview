#include <foo.h>
#include <stdio.h>

int foo_add(int x, int y)
{
    return x + y;
}

int foo_sub(int x, int y)
{
    return x - y;
}

int foo_mul(int x, int y)
{
    return x * y;
}

int foo_div(int x, int y)
{
    if (y == 0)
    {
        printf("[ERROR] y must be non-zero, y = %d\n", y);
        return 0;
    }

    if ((x / y) * y != x)
        printf("[WARNING] precision lost x = %d, y = %d, result = %d\n", x, y, x / y);

    return x / y;
}
