// https://www.open-std.org/jtc1/sc22/wg14/www/docs/n1548.pdf

extern void lifetime_main(void);
extern void utils_main(void);
extern void memory_main(void);
extern void c11threads_main(void);

int main(void)
{
    lifetime_main();
    utils_main();
    memory_main();
    c11threads_main();

    return 0;
}

// gcc *.c -Wall -Wextra -std=c11 -pedantic -o main.out -lpthread