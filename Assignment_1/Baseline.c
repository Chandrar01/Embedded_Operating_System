// include all dependencies
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#define MAX_VAL 1000000000

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

		for(double j = _number.start_N; j< _number.start_N + MAX_VAL; j++)
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

int main(int argc, char const *argv[])
{
	/* code */
	// variables to mesure time of summation

#if defined(LONG_RUN)
    time_t t_start,t_end;
#elif !defined(LONG_RUN)
    clock_t t_start,t_end;
#else
    time_t t_start,t_end;
#endif

	



	Number_t Num;

	// input variables
	long double N;
	long double Sum_Of_N = 0;


	// User input
	printf("Enter value of N \n");
	scanf("%Lf", &N);
	printf("N = %Lf\n", N);

	Num.start_N = 0;
	Num.end_N = N;

	// Start time of summation
#if defined(LONG_RUN)
    t_start = time(NULL);
#elif !defined(LONG_RUN)
    t_start = clock();
#else
    t_start = time(NULL);
#endif
	

	Sum_Of_N = Summation(Num);
	
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

