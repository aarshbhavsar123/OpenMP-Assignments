#include<stdio.h>
#include<omp.h>
#define NUM_THREAD 4
#define PAD 8
static long num_steps = 100000;
double step;
int main()
{   
    int i,nthreads;double pi;
    step = 1.0/num_steps;
    omp_set_num_threads(NUM_THREAD);
    double start_time, end_time;
    start_time = omp_get_wtime();
    #pragma omp parallel
    {
        int i,id,nthrds;
        double x;
        double sum = 0.0;
        id = omp_get_thread_num();
        nthrds = omp_get_num_threads();
        if(id == 0)
        {
            nthreads = nthrds;
        }
        sum= 0.0;
        for(int i = id;i<num_steps;i+=nthrds)
        {
            x = (i)*step;
            sum+=(4.0)/(1.0+x*x);
        }
        #pragma omp critical
        pi+=sum*step;
    }
    end_time = omp_get_wtime();
    printf("PI = %f\n",pi);
    printf("Runtime: %f seconds\n", end_time - start_time);
    return 0;
}