#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <semaphore.h>

#define MAX_VAL 1000000000
#define MAX_NUM_TASKS  8

#define LONG_RUN 1

//create semaphore

sem_t mutex;

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
	// initialize semaphore
	sem_init(&mutex, 1, 0);


	// variables to mesure time of summation
#if defined(LONG_RUN)
    time_t t_start,t_end;
#elif !defined(LONG_RUN)
    clock_t t_start,t_end;
#else
    time_t t_start,t_end;
#endif

	// process ids
	int status;
	pid_t pid[MAX_NUM_TASKS];
	Number_t Number[MAX_NUM_TASKS];

	// I assume Although maximum number of tasks are 8
	// This means parent and child altogether 
	// there would be 8 processes/tasks running concurrently


	// input variables
	long double N = 0;
	int NUM_TASKS = 0;

	// output variable
	long double Sum_Of_N = 0;

	// User input
	printf("Enter value of N \n");
	scanf("%Lf", &N);
	printf("N = %Lf\n", N);

	printf("\n");

	printf("Enter Number of tasks NUM_TASKS \n");
	scanf("%d", &NUM_TASKS);
	printf("NUM_TASKS = %d\n", NUM_TASKS);

	int Num_Of_forks = 0;

	switch(NUM_TASKS)
	{
		case 8:
				Num_Of_forks = 3;
				break;

		case 4:
				Num_Of_forks = 2;
				break;

		case 2:
				Num_Of_forks = 1;
				break;

		default:
				break;		

	}

	// Number struct creation
	for(int i = 0; i < NUM_TASKS; i++)
	{
		Number[i].start_N = (i*N/NUM_TASKS)+1;
		Number[i].end_N = (i+1)*N/NUM_TASKS;

		printf("for %d first = %Lf seconf = %Lf\n", i, Number[i].start_N, Number[i].end_N );
	}

	// pipes to send information
	int fd[2];
	pipe(fd);

	long double send = 0;
	long double receive = 0;

	// Start time of summation
#if defined(LONG_RUN)
    t_start = time(NULL);
#elif !defined(LONG_RUN)
    t_start = clock();
#else
    t_start = time(NULL);
#endif

	// create tasks
	for(int i=0;i<Num_Of_forks;i++)
	{
		pid[i] = fork();
	}


	// find all the processes

	switch(NUM_TASKS)
	{
		case 8:
		{
			if(pid[0]==0 && pid[1]==0 && pid[2]==0) // sender
			{
				//1 CCC
				// close read end
				close(fd[0]);
				send = Summation(Number[0]);
				// send result over pipe to parent process
				// This is a critical section
				sem_wait(&mutex);

					write(fd[1],&send,sizeof(send));
					printf("child 7 sum = %Lf\n", send);

				sem_post(&mutex);



			}
			else if(pid[0]==0 && pid[1]==0 && pid[2]>0) // sender
			{
				//2 CCP
				// close read end
				close(fd[0]);
				
				send = Summation(Number[1]);
				// send result over pipe to parent process
				// send result over pipe to parent process
				// This is a critical section
				sem_wait(&mutex);

					write(fd[1],&send,sizeof(send));
					printf("child 6 sum = %Lf\n", send);

				sem_post(&mutex);

			}
			else if(pid[0]==0 && pid[1]>0 && pid[2]==0) // sender
			{
				//3 CPC
				// close read end
				close(fd[0]);
				
				send = Summation(Number[2]);
				// send result over pipe to parent process
				// send result over pipe to parent process
				// This is a critical section
				sem_wait(&mutex);

					write(fd[1],&send,sizeof(send));
					printf("child 5 sum = %Lf\n", send);

				sem_post(&mutex);

			}
			else if(pid[0]==0 && pid[1]>0 && pid[2]>0) // sender
			{
				//4 CPP
				// close read end
				close(fd[0]);
				
				send = Summation(Number[3]);
				// send result over pipe to parent process
				// send result over pipe to parent process
				// send result over pipe to parent process
				// This is a critical section
				sem_wait(&mutex);

					write(fd[1],&send,sizeof(send));
					printf("child 4 sum = %Lf\n", send);

				sem_post(&mutex);
			}
			else if(pid[0]>0 && pid[1]==0 && pid[2]==0) // sender
			{
				//5 PCC
				// close read end
				close(fd[0]);
				
				send = Summation(Number[4]);
				// send result over pipe to parent process
				// send result over pipe to parent process
				// This is a critical section
				sem_wait(&mutex);

					write(fd[1],&send,sizeof(send));
					printf("child 3 sum = %Lf\n", send);

				sem_post(&mutex);

			}
			else if(pid[0]>0 && pid[1]==0 && pid[2]>0) // sender
			{
				//6 PCP
				// close read end
				close(fd[0]);
				
				send = Summation(Number[5]);
				// send result over pipe to parent process
				// send result over pipe to parent process
				// This is a critical section
				sem_wait(&mutex);

					write(fd[1],&send,sizeof(send));
					printf("child 2 sum = %Lf\n", send);

				sem_post(&mutex);

			}
			else if(pid[0]>0 && pid[1]>0 && pid[2]==0) // sender
			{
				//7 PPC
				// close read end
				close(fd[0]);
				
				send = Summation(Number[6]);
				// send result over pipe to parent process
				// send result over pipe to parent process
				// This is a critical section
				sem_wait(&mutex);

					write(fd[1],&send,sizeof(send));
					printf("child 1 sum = %Lf\n", send);

				sem_post(&mutex);

			}
			else if(pid[0]>0 && pid[1]>0 && pid[2]>0) // receiver
			{
				//8 PPP

				// close write end
				close(fd[1]);
					

				Sum_Of_N = Summation(Number[7]);

				for (int i=0; i <NUM_TASKS-1; i++)
				{
					waitpid(pid[i],&status,0);
					// receive data from child process
					read(fd[0],&receive,sizeof(receive));
					Sum_Of_N = Sum_Of_N + receive;	
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

			}
			else 
			{

			}

			break;
		}

		case 4:
		{
			if(pid[0]==0 && pid[1]==0)
			{
				//1 CC
				// close read end
				close(fd[0]);
				send = Summation(Number[3]);
				// send result over pipe to parent process
				// This is a critical section
				sem_wait(&mutex);

					write(fd[1],&send,sizeof(send));
					printf("child 3 sum = %Lf\n", send);

				sem_post(&mutex);

			}
			else if(pid[0]==0 && pid[1]>0)
			{
				//2 CP
				// close read end
				close(fd[0]);
				send = Summation(Number[2]);
				// send result over pipe to parent process
				// This is a critical section
				sem_wait(&mutex);

					write(fd[1],&send,sizeof(send));
					printf("child 2 sum = %Lf\n", send);

				sem_post(&mutex);

			}
			else if(pid[0]>0 && pid[1]==0)
			{
				//3 PC
				// close read end
				close(fd[0]);
				send = Summation(Number[1]);
				// send result over pipe to parent process
				// This is a critical section
				sem_wait(&mutex);

					write(fd[1],&send,sizeof(send));
					printf("child 1 sum = %Lf\n", send);

				sem_post(&mutex);
			}
			else if(pid[0]>0 && pid[1]>0)
			{
				//4 PP
				// close write end
				close(fd[1]);
					
				Sum_Of_N = Summation(Number[0]);
				printf("parent sum = %Lf\n", Sum_Of_N);

				// receive result over pipe to parent process
	
				for (int i=0; i <NUM_TASKS-1  ; i++)
				{
					waitpid(pid[i],&status,0);

					// receive data from child process
					read(fd[0],&receive,sizeof(receive));

					printf("received = %Lf\n", receive);
					Sum_Of_N = Sum_Of_N + receive;	
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
			}
			else 
			{

			}

			break;
		}

		case 2:
		{
			if(pid[0]==0)
			{
				//1 C
				//1 CCC
				// close read end
				close(fd[0]);
				send = Summation(Number[0]);
				// send result over pipe to parent process
				// This is a critical section
				sem_wait(&mutex);

					write(fd[1],&send,sizeof(send));

				sem_post(&mutex);

			}
			else if(pid[0]>0)
			{
				//2 P
				// close write end
				close(fd[1]);
					

				Sum_Of_N = Summation(Number[1]);

				printf("Num_task = %d\n", NUM_TASKS);
				printf("parent sum = %Lf\n", Sum_Of_N);


				// receive result over pipe to parent process
	
				for (int i=0; i <NUM_TASKS - 1; i++)
				{
					waitpid(pid[i],&status,0);

					// receive data from child process
					read(fd[0],&receive,sizeof(receive));

					printf("received = %Lf\n", receive);

					Sum_Of_N = Sum_Of_N + receive;	
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

			}
			else 
			{

			}
			break;
		}

		default:
			break;
		
	}




	return 0;
}