#include <stdio.h> 
#include <sys/types.h> 
#include <unistd.h>  
#include <stdlib.h>  
#include <sys/wait.h> 
#include <string.h> 
#include <time.h> 
#include <signal.h>
#include <sys/time.h>

/************************************************************************************************ 
        These DEFINE statements represent the workload size of each task and 
        the time quantum values for Round Robin scheduling for each task.
*************************************************************************************************/

#define WORKLOAD1 100000
#define WORKLOAD2 100000
#define WORKLOAD3 100000
#define WORKLOAD4 100000

#define QUANTUM1 1000
#define QUANTUM2 1000
#define QUANTUM3 1000
#define QUANTUM4 1000

#define ITTERATION 10
#define BILLION  1000000000L;

double time_difference(struct timespec start_time, struct timespec end_time)
{
   
    double diff;
    diff = end_time.tv_sec - start_time.tv_sec + (double)( end_time.tv_nsec - start_time.tv_nsec ) / (double)BILLION;
    return diff;
}

/************************************************************************************************ 
        Lookoing at WORKLOADS we can see that for shortest job first implementation
        static priorities of tasks would be W4 > W3 > W2 > W1
*************************************************************************************************/


/************************************************************************************************ 
                    DO NOT CHANGE THE FUNCTION IMPLEMENTATION
*************************************************************************************************/
void myfunction(int param){

    int i = 2;
    int j, k;

    while(i < param){
        k = i; 
        for (j = 2; j <= k; j++)
        {
            if (k % j == 0){
                k = k/j;
                j--;
                if (k == 1){
                    break;
                }
            }
        }
        i++;
    }
}
/************************************************************************************************/

int main(int argc, char const *argv[])
{
    pid_t pid1, pid2, pid3, pid4;
    int running1, running2, running3, running4;

    // start time and end time of a task
    struct timespec t_start[4],t_end[4];

    // wait time and execution time of a task
    double t_wait[4], t_exec[4] = {0};;

    // execution/run time of a task
    struct timespec t_exec_start[4], t_exec_end[4]; 

    double sum_avg_t_wait = 0;
    double avg_t_wait[ITTERATION] = {0};

    for(int iter = 1 ; iter <= ITTERATION; iter++)
    {    
        // this is to create the 4 workload child processes used for implementing Scheduling

        pid1 = fork();

        if (pid1 == 0){

            myfunction(WORKLOAD1);

            exit(0);
        }
        kill(pid1, SIGSTOP);

        pid2 = fork();

        if (pid2 == 0){

            myfunction(WORKLOAD2);

            exit(0);
        }
        kill(pid2, SIGSTOP);

        pid3 = fork();

        if (pid3 == 0){

            myfunction(WORKLOAD3);

            exit(0);
        }
        kill(pid3, SIGSTOP);

        pid4 = fork();

        if (pid4 == 0){

            myfunction(WORKLOAD4);

            exit(0);
        }
        kill(pid4, SIGSTOP);

        /************************************************************************************************ 
            At this point, all  newly-created child processes are stopped, and ready for scheduling.
        *************************************************************************************************/



        /************************************************************************************************
            - Scheduling code starts here
            - Below is a sample schedule. (which scheduling algorithm is this?)
            - For the assignment purposes, you have to replace this part with the other scheduling methods 
            to be implemented.
        ************************************************************************************************/

        running1 = 1;
        running2 = 1;
        running3 = 1;
        running4 = 1;

        for(int t = 0; t<4; t++)
            clock_gettime( CLOCK_REALTIME, &t_start[t]);

        //while any of the 4 processes is running
        while (running1 > 0 || running2 > 0 || running3 > 0 || running4 > 0)
        {
            // run W4
             if (running4 > 0){
                clock_gettime( CLOCK_REALTIME, &t_exec_start[3]);
                
                kill(pid4, SIGCONT);
                waitpid(pid4 , &running4, 0);

                clock_gettime( CLOCK_REALTIME, &t_exec_end[3]);

                t_exec[3] = time_difference(t_exec_start[3], t_exec_end[3]);

                //printf("exec time of p4 = %f \n", t_exec[3]);
                t_wait[3] = time_difference(t_start[3], t_exec_end[3]) - t_exec[3];
                //printf("wait time of p4 = %0.6f \n",t_wait[3]);

            }

            // run W3
             if (running3 > 0){
                clock_gettime( CLOCK_REALTIME, &t_exec_start[2]);

                kill(pid3, SIGCONT);
                waitpid(pid3 , &running3, 0);

                clock_gettime( CLOCK_REALTIME, &t_exec_end[2]);

                t_exec[2] = time_difference(t_exec_start[2], t_exec_end[2]);

                //printf("exec time of p3= %f \n", t_exec[2]);
                t_wait[2] = time_difference(t_start[2], t_exec_end[2]) - t_exec[2];
                //printf("wait time of p3 = %0.6f \n",t_wait[2]);
            }

            // run W2
             if (running2 > 0){
                clock_gettime( CLOCK_REALTIME, &t_exec_start[1]);

                kill(pid2, SIGCONT);
                waitpid(pid2 , &running2, 0);

                clock_gettime( CLOCK_REALTIME, &t_exec_end[1]);

                t_exec[1] = time_difference(t_exec_start[1], t_exec_end[1]);

                //printf("exec time of p2= %f \n", t_exec[1]);
                t_wait[1] = time_difference(t_start[1], t_exec_end[1]) - t_exec[1];
                //printf("wait time of p2 = %0.6f \n",t_wait[1]);
            }

            // run W1
             if (running1 > 0){
                clock_gettime( CLOCK_REALTIME, &t_exec_start[0]);

                kill(pid1, SIGCONT);
                waitpid(pid1 , &running1, 0);

                clock_gettime( CLOCK_REALTIME, &t_exec_end[0]);

                t_exec[0] = time_difference(t_exec_start[0], t_exec_end[0]);

                //printf("exec time of p2= %f \n", t_exec[0]);
                t_wait[0] = time_difference(t_start[0], t_exec_end[0]) - t_exec[0];
                //printf("wait time of p2 = %0.6f \n",t_wait[0]);
            }


        }

        for(int t = 0; t<4; t++)
           clock_gettime( CLOCK_REALTIME, &t_end[t]);

        /************************************************************************************************
            - Scheduling code ends here
        ************************************************************************************************/
        avg_t_wait[iter] = ((double)(t_wait[0]+t_wait[1]+t_wait[2]+t_wait[3])/4.0);

        sum_avg_t_wait = sum_avg_t_wait + avg_t_wait[iter];

        //printf("T: iteration %d : Average wait Time taken to finish all the tasks is %f seconds \n", iter,
        //   ((double)(t_wait[0]+t_wait[1]+t_wait[2]+t_wait[3])/4.0));

    }    

     double mean_of_avg_t_wait = (double)(sum_avg_t_wait/ITTERATION);

     printf("T: Total iterations %d : mean wait Time taken to finish all the tasks is %f seconds \n", ITTERATION, mean_of_avg_t_wait);

    return 0;
}