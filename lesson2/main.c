//  https://www.dii.uchile.cl/~daespino/files/Iso_C_1999_definition.pdf

#include <stdio.h>

extern void const_main(void);
extern void arr_ptr_main(void);
extern void restrict_main(void);
extern void bitwise_main(void);
extern void compund_literals_main(void);
extern void alingment_main(void);
extern void fam_main(void);

int main(void)
{
    printf("\nCONST MAIN\n");
    const_main();

    printf("\n\nARR PTR MAIN\n");
    arr_ptr_main();

    printf("\n\nRESTRICT MAIN\n");
    restrict_main();

    printf("\n\nBITWISE MAIN\n");
    bitwise_main();

    printf("\n\nCOMPUND LITERALS MAIN\n");
    compund_literals_main();

    printf("\n\nALINGMENT MAIN\n");
    alingment_main();

    printf("\n\n FAM MAIN\n");
    fam_main();

    return 0;
}
