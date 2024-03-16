#include<stdio.h>
#include<omp.h>
int main()
{
    static long num_steps = 100000;
    double step = 1.0/(double)(num_steps);
    double pi = 0.0;
    long i;
    double start_time,end_time;
    start_time = omp_get_wtime();
    #pragma omp parallel for reduction(+:pi)
    
        for(i = 0;i<num_steps;i++)
        {
            double x = (i+0.5)*step;
            pi+=(4.0/(1+x*x))*step;
        }    
    
    end_time = omp_get_wtime();
    printf("%f\n",pi);
    printf("Runtime : %f",end_time-start_time);
return 0;
}