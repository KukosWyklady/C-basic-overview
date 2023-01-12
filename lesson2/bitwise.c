#include <stdio.h>
#include <stddef.h>
#include <limits.h>
#include <stdint.h>
#include <sys/types.h>

#define BIT_MASK(s, e)           (((1uL << ((e) - (s) + 1)) - 1) << (s))
#define BIT_GET_MASK(n, s, e)    (((n) & (BIT_MASK(s, e))) >> s)
#define BIT_SET_MASK(n, s, e)    ((n) | (BIT_MASK(s, e)))
#define BIT_CLEAR_MASK(n, s, e)  ((n) & ~(BIT_MASK(s, e)))
#define BIT_TOGGLE_MASK(n, s, e) ((n) ^ (BIT_MASK(s, e)))

/*
#define BIT_N(n)         (1uL << (n))
#define BIT_GET(n, k)    (((n) & BIT_N(k)) >> (k))
#define BIT_SET(n, k)    ((n) | (BIT_N(k)))
#define BIT_CLEAR(n, k)  ((n) & ~(BIT_N(k)))
#define BIT_TOGGLE(n, k) ((n) ^ (BIT_N(k)))
*/

#define BIT_N(n)         BIT_MASK(n, n)
#define BIT_GET(n, k)    BIT_GET_MASK(n, k, k)
#define BIT_SET(n, k)    BIT_SET_MASK(n, k, k)
#define BIT_CLEAR(n, k)  BIT_CLEAR_MASK(n, k, k)
#define BIT_TOGGLE(n, k) BIT_TOGGLE_MASK(n, k, k)

static void bit_print(unsigned int a);

static void bitwise_defined(void);
static void bit_macros(void);
static void bit_endianness(void);
static void bit_shifts(void);
static void bit_fields(void);

static void bit_print(unsigned int a)
{
    // get number of chars required for printing
    size_t len = (size_t)snprintf(NULL, 0, "%u (%X)", a, a);

    printf("%u (%X)%*s= ", a, a, 30 - (int)len, " ");
    for (ssize_t i = sizeof(a) * CHAR_BIT - 1; i >= 0; --i)
    {
        putchar('0' + (int)BIT_GET(a, i));
        if (i % 8 == 0)
            putchar(' ');
    }
    putchar('\n');
}

static void bit_macros(void)
{
    unsigned int a = 10;

    printf("A:\t\t");
    bit_print(a);

    printf("GET(A, 0)\t");
    bit_print(BIT_GET(a, 0));
    printf("GET(A, 1)\t");
    bit_print(BIT_GET(a, 1));

    printf("SET(A, 0)\t");
    bit_print(BIT_SET(a, 0));

    printf("CLEAR(A, 1)\t");
    bit_print(BIT_CLEAR(a, 1));

    printf("TOGGLE(A, 5)\t");
    bit_print(BIT_TOGGLE(a, 5));

    printf("GET(A, 0, 4)\t");
    bit_print(BIT_GET_MASK(a, 0, 4));

    printf("SET(A, 3, 10)\t");
    bit_print(BIT_SET_MASK(a, 3, 10));

    printf("CLEAR(A, 0, 30)\t");
    bit_print(BIT_CLEAR_MASK(a, 0, 30));

    printf("TOGGLE(A, 0, 3)\t");
    bit_print(BIT_TOGGLE_MASK(a, 0, 3));

    printf("\n");
}

static void bit_fields(void)
{
    struct S
    {
        uint32_t x:3;
        uint32_t y:10;
        uint32_t pad:32-13;
    } s = {0};

    // ISO C99 6.7.7 Type definitions 6
    struct N
    {
        int32_t x:3;
        int32_t  :0; // x i y are not the same
        int32_t y:10;
        int32_t pad:32-10;
    } n = {0};

    s.x = 6;
    n.x = -3;
    printf("s.x = %u (%X), us.x = %d (%X)\n", s.x, s.x, n.x, n.x);

    s.x = 100; // UB, truncation but how cpu will truncate it? DUNNO
    n.x = 6; // UB, 1st bit is for sign not data
    printf("s.x = %u (%X), us.x = %d (%X)\n", s.x, s.x, n.x, n.x);

    s.x = -1; // UB. How to trundate it?
    n.x = 2; // OK
    printf("s.x = %u (%X), us.x = %d (%X)\n", s.x, s.x, n.x, n.x);

    union U
    {
        uint32_t x:12; // uint32_t x
        uint32_t y:20; // uint32_t y | this is not part of uint32_t x
        uint32_t a; // uint32_t a
    } u = {0};

    u.x = 100;
    u.y = (1 << 15) - 1;

    printf("u.x\t");
    bit_print(u.x);

    printf("u.y\t");
    bit_print(u.y);

    printf("u.a\t");
    bit_print(u.a);

    printf("\n");

    /*
        struct S2
        {
            uint32_t x:u.a // ERROR. RUN TIME
            uint32_t y:10 * !!1 + sizeof(int) * 4 // OK COMPILE TIME
            uint32_t t:10[10] // ERROR There is no arr of bitfields
            uint32_t t2[10]:10 // ERROR there is no bitfields inside array
        } s2;
    */
}

static void bit_endianness(void)
{
    union U
    {
        uint32_t a;
        uint8_t b[4];
    } u = {0};

    // ADDRESS ------------------------->
    // BE = 00 00 00 FF, LE = FF 00 00 00
    u.a = 0xFF;

    u.b[0] == 0xFF ? (printf("Little Endian\n")) : (printf("Big Endian\n"));

    printf("\n");

    /*
        The order of allocation of bit-fields within a unit (high-order to
        low-order or low-order to high-order) is implementation-defined.
    */
    union U2
    {
        struct B
        {
            uint32_t y:10;
            uint32_t x:3;
            uint32_t pad:32-13;
        } b;
        uint32_t a;
    } u2 = {0};

    u2.b.y = 0xB3;
    u2.b.x = 6;

    printf("u2.a\t");
    bit_print(u2.a); // UB. Depends on endianess

    printf("u2.b.x\t");
    bit_print(u2.b.x); // OK. We wrote to u2.b.x so this is defined

    printf("u2.b.y\t");
    bit_print(u2.b.y); // OK. We wrote to u2.b.y so this is defined
}

static void bitwise_defined(void)
{
    // ADDRESS ------------------------->
    // BE = DE AD BE EF, LE = EF BE AD DE
    unsigned int a = 0xdeadbeef;
    // In case of defined bitwise operations we dont care about endiannes, cpu will do the thing for us

    printf("a\t\t");
    bit_print(a);

    printf("~a\t\t");
    bit_print(~a);

    printf("a | 0xff\t");
    bit_print(a | 0xFF);

    printf("a & 0x5\t\t");
    bit_print(a & 0x5);

    printf("a ^ 0x5\t\t");
    bit_print(a ^ 0x5); // XOR

    printf("a << 3\t");
    bit_print(a << 3); // a * 8

    printf("a >> 3\t");
    bit_print(a >> 3); // a / 8

    printf("\n");
}




/*
    E1 >> E2
    If E1 has a signed type  and a negative value, the resulting value is implementation-defined.
    E1 << E2
    If E1 has a signed type  and a negative value, then this is undefined behavior.
*/
static void bit_shifts(void)
{
    uint32_t a = 0xdeadbeef;

    printf("SAL i SHL\n");
    bit_print(a);
    __asm__ volatile
    (
        "sal %0"
        : "=r" (a) : "0" (a)
    );
    bit_print(a);

    a = 0xdeadbeef;
    __asm__ volatile
    (
        "shl %0"
        : "=r" (a) : "0" (a)
    );
    bit_print(a);

    /*
        SHR - logical always add 0
        SAR - arithmetic keep the last bit

        FOR signed >0 SAR == SHR.
        FOR signed <0 NOT.
        FOR unsigned NOT.

        2 asm op
        1 C syntax

        so UB :)

        0xF0 (1111 0000) shr 1 --> 0x78 (0111 1000)
        0xF0 (1111 0000) sar 1 --> 0xF8 (1111 1000)
        Or in signed decimal:

        -16 (1111 0000) shr 1 --> 120 (0111 1000), wrong
        -16 (1111 0000) sar 1 --> -32 (1111 1000), correct
        Unsigned decimal:

        240 (1111 0000) shr 1 --> 120 (0111 1000), correct
        240 (1111 0000) sar 1 --> 248 (1111 1000), wrong
    */
    printf("SAR i SHR\n");
    a = 0xdeadbeef;
    bit_print(a);
    __asm__ volatile
    (
        "sar %0"
        : "=r" (a) : "0" (a)
    );
    bit_print(a);

    a = 0xdeadbeef;
    __asm__ volatile
    (
        "shr %0"
        : "=r" (a) : "0" (a)
    );
    bit_print(a);

    printf("C << i >> uint32_t\n");

    a = 0xdeadbeef;
    a <<= 1;
    bit_print(a);

    a = 0xdeadbeef;
    a >>= 1;
    bit_print(a);

    printf("C << i >> int32_t\n");

    int32_t b = (int32_t)0xdeadbeef;
    b <<= 1;
    bit_print((unsigned)b);

    b = (int32_t)0xdeadbeef;
    b >>= 1;
    bit_print((unsigned)b);
}

void bitwise_main(void)
{
    bit_macros();
    bit_fields();
    bit_endianness();
    bitwise_defined();
    bit_shifts();
}
