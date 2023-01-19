extern void interleaving_main(void);
extern void mutex_main(void);
extern void thread_safe_function_main(void);
extern void semaphore_main(void);
extern void pcp_main(void);
extern void omp_main();


int main(void)
{
    interleaving_main();
    mutex_main();
    thread_safe_function_main();
    semaphore_main();
    pcp_main();
    omp_main();

    return 0;
}

// gcc -fopenmp *.c -o main.out -lpthread