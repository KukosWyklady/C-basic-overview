#include <stdio.h>
#include <stddef.h>

/*
    Memory Bank Layout
     1 2 3 4 5 6 7 8
    _________________
    |_|_|_|_|_|_|_|_|   0x30
    |_|_|_|_|_|_|_|_|   0x28
    |_|_|_|_|_|_|_|_|   0x20
    |_|_|_|_|_|_|_|_|   0x18
    |_|_|_|_|_|_|_|_|   0x10
    |_|_|_|_|_|_|_|_|   0x8
    |_|_|_|_|_|_|_|_|   0x0
*/


/*
    Memory Bank Layout
     1 2 3 4 5 6 7 8
    _________________
    |_|_|_|_|_|_|_|_|   0x30
    |_|_|_|_|_|_|_|_|   0x28
    |_|_|_|_|_|_|_|_|   0x20
    |_|_|_|_|_|_|_|C|   0x18
    |_|_|_|_|_|_|S|S|   0x10
    |_|_|_|_|I|I|I|I|   0x8
    |L|L|L|L|L|L|L|L|   0x0
*/


struct CharS
{
    char a;
};

struct IntS
{
    int a;
};

struct LongS
{
    long a;
};

struct PtrS
{
    void* a;
};

struct LongDoubleS
{
    long double a;
};


/*
    Memory Bank Layout
     1 2 3 4 5 6 7 8
    _________________
    |_|_|_|_|_|_|_|_|   0x30
    |_|_|_|_|_|_|_|_|   0x28
    |_|_|_|_|_|_|_|_|   0x20
    |_|_|_|_|_|_|_|_|   0x18
    |_|_|_|_|_|_|_|_|   0x10
    |C|C|C|C|C|C|C|C|   0x8
    |B|B|B|B|P|P|P|A|   0x0
*/

struct MixS1
{
    char a;
    int b;
    long c;
};

struct MixPadS1
{
    char a;
    char pad[3];
    int b;
    long c;
};


/*
    Memory Bank Layout
     1 2 3 4 5 6 7 8
    _________________
    |_|_|_|_|_|_|_|_|   0x30
    |_|_|_|_|_|_|_|_|   0x28
    |_|_|_|_|_|_|_|_|   0x20
    |_|_|_|_|_|_|_|_|   0x18
    |P|P|P|P|P|E|D|D|   0x10
    |C|C|C|C|C|C|C|C|   0x8
    |B|B|B|B|P|P|P|A|   0x0
*/

struct MixS2
{
    char a;
    int b;
    long c;
    short d;
    char e;
};

struct MixPadS2
{
    char a;
    char pad[3];
    int b;
    long c;
    short d;
    char e;
    char pad2[5];
};


/*
    Memory Bank Layout
     1 2 3 4 5 6 7 8
    _________________
    |_|_|_|_|_|_|_|_|   0x30
    |_|_|_|_|_|_|_|_|   0x28
    |_|_|_|_|_|_|_|_|   0x20
    |_|_|_|_|_|_|_|_|   0x18
    |_|_|_|_|_|_|_|_|   0x10
    |C|C|C|C|C|C|C|C|   0x8
    |B|B|B|B|D|D|E|A|   0x0
*/
struct MixS3
{
    char a;
    char e;
    short d;
    int b;
    long c;
};

struct MixPadS3
{
    char a;
    char e;
    short d;
    int b;
    long c;
};


/*
    Memory Bank Layout
     1 2 3 4 5 6 7 8
    _________________
    |_|_|_|_|_|_|_|_|   0x30
    |_|_|_|_|_|_|_|_|   0x28
    |_|_|_|_|_|_|_|_|   0x20
    |_|_|_|_|_|_|_|_|   0x18
    |B|B|B|B|B|B|B|B|   0x10
    |B|B|B|B|B|B|B|B|   0x8
    |P|P|P|P|P|P|P|A|   0x0
*/
struct MixS4
{
    char a;
    struct MixS1 b;
};

struct MixPadS4
{
    char a;
    char pad[7];
    struct MixS1 b;
};


/*
    Memory Bank Layout
     1 2 3 4 5 6 7 8
    _________________
    |_|_|_|_|_|_|_|_|   0x30
    |P|P|P|P|P|P|P|P|   0x28
    |P|P|P|P|P|P|P|E|   0x20
    |D|D|D|D|D|D|D|D|   0x18
    |C|C|C|C|P|P|P|B|   0x10
    |A|A|A|A|A|A|A|A|   0x8
    |A|A|A|A|A|A|A|A|   0x0
*/
struct MixS5
{
    struct LongDoubleS a;
    char b;
    struct IntS c;
    struct LongS d;
    char e;
};

struct MixPadS5
{
    struct LongDoubleS a;
    char b;
    char pad[3];
    struct IntS c;
    struct LongS d;
    char e;
    char pad2[15];
};

#define PRINT_SIZEOF(struct) printf("sizeof(%s)= %zu\n", #struct, sizeof(struct))
void alingment_main(void)
{
    PRINT_SIZEOF(struct CharS);
    PRINT_SIZEOF(struct IntS);
    PRINT_SIZEOF(struct LongS);
    PRINT_SIZEOF(struct PtrS);
    PRINT_SIZEOF(struct LongDoubleS);

    PRINT_SIZEOF(struct MixS1);
    PRINT_SIZEOF(struct MixPadS1);

    PRINT_SIZEOF(struct MixS2);
    PRINT_SIZEOF(struct MixPadS2);

    PRINT_SIZEOF(struct MixS3);
    PRINT_SIZEOF(struct MixPadS3);

    PRINT_SIZEOF(struct MixS4);
    PRINT_SIZEOF(struct MixPadS4);

    PRINT_SIZEOF(struct MixS5);
    PRINT_SIZEOF(struct MixPadS5);

    PRINT_SIZEOF(struct {char a; char b; char c; char t[2];});
}
