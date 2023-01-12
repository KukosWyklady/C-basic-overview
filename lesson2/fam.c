#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

static void test_famincorrect(void);
static void test_famcorrect(void);
static void test_famextension(void);

static void sandbox(void);

typedef struct FAMIncorrect
{
    size_t len;
    int payload[1]; // this is not FAM
} FAMIncorrect;

static void test_famincorrect(void)
{
    //  OK char[1]
    FAMIncorrect fam1;
    (void)fam1;

    printf("sizeof(FAMIncorrect) = %zu\n", sizeof(FAMIncorrect));

    // UB | WARNING
    FAMIncorrect fam2 = {5, {1, 2, 3, 4, 5}};
    printf("FAMIncorrect stack\t");
    for (size_t i = 0; i < fam2.len; ++i)
    // UB
        printf("%4d", fam2.payload[i]);
    printf("\n");

    /* UB | problem with -1 */
    FAMIncorrect* fam3 = malloc(sizeof(*fam3) + (sizeof(fam3->payload[0]) * (5 - 1)));
    fam3->len = 5;
    for (size_t i = 0; i < fam3->len; ++i)
        fam3->payload[i] = (int)(i + 1);

    // UB
    printf("FAMIncorrect heap\t");
    for (size_t i = 0; i < fam3->len; ++i)
        printf("%4d", fam3->payload[i]);
    printf("\n");


    printf("\n\n");
    free(fam3);
}

typedef struct FAMCorrect
{
    size_t len;
    int payload[]; // FAM | since C99
} FAMCorrect;

// GNU C (GCC) static initialization | C99 UB
// FAMCorrect f = {5, {1, 2, 3, 4, 5}};


static void test_famcorrect(void)
{
    // OK there is no payload
    FAMCorrect fam1;
    (void)fam1;

    printf("sizeof(FAMCorrect) = %zu\n", sizeof(FAMCorrect));

    /* non-static initialization of flexible array member is not allowed */
    /* FAMCorrect fam2 = {5, {1, 2, 3, 4, 5}}; */
    /* OK only in GNU C */
    /* static FAMCorrect f = {5, {1, 2, 3, 4, 5}}; */

    /* sizeof(fam3->payload) --> ERROR sizeof(fam3->payload[0]) OK */
    FAMCorrect* fam3 = malloc(sizeof(*fam3) + (sizeof(fam3->payload[0]) * 5));
    fam3->len = 5;

    // OK C99 syntax with FAM
    for (size_t i = 0; i < fam3->len; ++i)
        fam3->payload[i] = (int)(i + 1);

    printf("FAMCorrect heap\t");
    for (size_t i = 0; i < fam3->len; ++i)
        printf("%4d", fam3->payload[i]);
    printf("\n");


    printf("\n\n");
    free(fam3);
}

/* FAM in GNU C 89 | DO NOT THIS RIGHT NOW */
typedef struct FamExtension
{
    size_t len;
    __extension__ int payload[0];
} FamExtension;

static void test_famextension(void)
{
    /* OK like normal FAM */
    FamExtension fam1;
    (void)fam1;

    printf("sizeof(FamExtension) = %zu\n", sizeof(FamExtension));

    /* UB | WARNING */
    FamExtension fam2 = {5, {1, 2, 3, 4, 5}};
    printf("FamExtension stack\t");
    for (size_t i = 0; i < fam2.len; ++i)
        printf("%4d", fam2.payload[i]);
    printf("\n");

    FamExtension* fam3 = malloc(sizeof(*fam3) + (sizeof(fam3->payload[0]) * 5));
    fam3->len = 5;

    /* OK GNU C extension */
    for (size_t i = 0; i < fam3->len; ++i)
        fam3->payload[i] = (int)(i + 1);

    printf("FamExtension heap\t");
    for (size_t i = 0; i < fam3->len; ++i)
        printf("%4d", fam3->payload[i]);
    printf("\n");

    printf("\n\n");
    free(fam3);
}

typedef struct NormalStruct
{
    size_t len;
    int *payload;
} NormalStruct;

static void sandbox(void)
{
    NormalStruct* normal;
    FAMCorrect* fam;

    const size_t n = 5;

    normal = malloc(sizeof(*normal));
    normal->payload = malloc(sizeof(*normal->payload) * n);
    normal->len = n;

    fam = malloc(sizeof(*fam) + sizeof(fam->payload[0]) * n);
    fam->len = n;

   free(normal->payload);
   free(normal);
   free(fam);
}



// ONLY 1 FAM at the end
struct Foo1
{
    size_t len;
    /* int t[]; flexible array member not at end of struct */
    int t2[];
};

// FAM required something before
/*
struct Foo2
{
    flexible array member in a struct with no named members
    int t[];
};
*/

/* FAM inside FAM is not allowed */
struct Foo3
{
    int a;
    int b;
    /* invalid use of structure with flexible array member */
    FAMCorrect fam;
};

/* Array of FAM is not allowed */
FAMCorrect t[100];  /* invalid use of structure with flexible array member */

void fam_main(void)
{
    test_famincorrect();
    test_famcorrect();
    test_famextension();

    sandbox();
}
