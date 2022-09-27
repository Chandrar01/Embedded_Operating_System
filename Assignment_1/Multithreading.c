// include all dependencies
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#define MAX_VAL 1000000000
#define MAX_NUM_THREADS  8

#define LONG_RUN 1

// define a structure to hold start and end value 
// of numbers to be added in sequential order
typedef struct Number
{
	long double start_N;
	long double end_N;

}Number_t;

// this function returns the sum of number
long double Summation(Number_t number)
{
	long double sum = 0;

	Number_t _number = number;
	
	long double _sum = 0;
	long double diff =_number.end_N-_number.start_N ;

	while(diff > MAX_VAL)
	{
		_sum = 0;

		for(double j = _number.start_N; j<= _number.start_N + MAX_VAL; j++)
		{

			_sum = _sum + j;
		}	

		_number.start_N = _number.start_N + MAX_VAL;

		diff = _number.end_N-_number.start_N ;

		sum += _sum;
	}

	_sum = 0;
	
	for(double i = _number.start_N; i<_number.end_N+1; i++)
	{
			_sum = _sum + i;
	}	

	sum +=_sum;
	
	return sum;
}

// this thread returns sum of numbers 
// between start_N and end_N
void* Summation_thread(void* arg)
{
	long double* returnVal = (long double*)malloc(sizeof(long double));

	Number_t num = *((Number_t*)arg);

	printf("This is a child thread with %Lf  %Lf\n", num.start_N, num.end_N);

	*returnVal = Summation((*(Number_t*)arg));

	printf("this sum = %Lf\n", *returnVal);

	pthread_exit((void*)returnVal);
}

int main(int argc, char const *argv[])
{
	// variables to mesure time of summation
#if defined(LONG_RUN)
    time_t t_start,t_end;
#elif !defined(LONG_RUN)
    clock_t t_start,t_end;
#else
    time_t t_start,t_end;
#endif

	// thread ids
	pthread_t thread_Id[MAX_NUM_THREADS];
	Number_t Number[MAX_NUM_THREADS];

	// input variables
	long double N = 0;
	int NUM_THREADS = 0;

	// output variable
	long double* Sum = NULL;
	long double Sum_Of_N = 0;

	// User input
	printf("Enter value of N \n");
	scanf("%Lf", &N);
	printf("N = %Lf\n", N);

	printf("\n");

	printf("Enter Number of threads NUM_THREAD \n");
	scanf("%d", &NUM_THREADS);
	printf("NUM_THREAD = %d\n", NUM_THREADS);

	// Start time of summation
#if defined(LONG_RUN)
    t_start = time(NULL);
#elif !defined(LONG_RUN)
    t_start = clock();
#else
    t_start = time(NULL);
#endif

	// Thread creation
	for(int i = 0; i < NUM_THREADS; i++)
	{
		Number[i].start_N = (i*N/NUM_THREADS)+1;
		Number[i].end_N = (i+1)*N/NUM_THREADS;
		pthread_create(thread_Id+i,NULL,Summation_thread,(void*)(Number+i));
	}

	// thread join 
	for(int i = 0; i<NUM_THREADS; i++)
	{
		pthread_join(thread_Id[i],(void**)&Sum);
		Sum_Of_N = Sum_Of_N + *(Sum);
	}


	// End time of Summation
#if defined(LONG_RUN)
    t_end = time(NULL);
#elif !defined(LONG_RUN)
    t_end = clock();
#else
    t_end = time(NULL);
#endif

	// Print finlal Answer
	printf("Sum of N = %Lf\n", Sum_Of_N);

	// Print Total Time of execution 
#if defined(LONG_RUN)
	printf("T: Time taken to print Sum of N numbers is %0.2f seconds",
           difftime(t_end,t_start));
#elif !defined(LONG_RUN)
	printf("C: Time taken to print Sum of N numbers is %f seconds",
           ((float)(t_end-t_start))/CLOCKS_PER_SEC);
#else
	printf("T: Time taken to print Sum of N numbers is %0.2f seconds",
           difftime(t_end,t_start));
#endif

	return 0;
}

