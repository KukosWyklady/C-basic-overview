#include <stdio.h>

extern void queue_main_test(void);

int main(void)
{
    queue_main_test();
    return 0;
}

// gcc *.c -Wextra -Wall -std=c99 -o main.out