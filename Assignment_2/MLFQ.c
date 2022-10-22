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

#define QUANTUM1 1000000
#define QUANTUM2 1000000
#define QUANTUM3 1000000
#define QUANTUM4 1000000

#define ITTERATION 10
#define BILLION  1000000000L;

double time_difference(struct timespec start_time, struct timespec end_time)
{
   
    double diff;
    diff = end_time.tv_sec - start_time.tv_sec + (double)( end_time.tv_nsec - start_time.tv_nsec ) / (double)BILLION;
    return diff;
}

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

typedef struct 
{
    int task_queue[4];
    int num_tasks_in_queue;

}fcfs_q;

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

    double  sum_avg_t_wait = 0;
    double avg_t_wait[ITTERATION] = {0};

    fcfs_q queue;

   

    for(int iter = 1 ; iter <= ITTERATION; iter++)
    {   

        for(int t = 0; t<4; t++)
            t_exec[t] = 0;

        for(int t = 0; t<4; t++)
            t_wait[t] = 0;

        for(int t=0; t<4; t++)
        {
            queue.task_queue[t] = 0;
        }
        queue.num_tasks_in_queue = 0;

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

        // Q1 with Quantum defined 
   
        if (running1 > 0){
            clock_gettime( CLOCK_REALTIME, &t_exec_start[0]);

            kill(pid1, SIGCONT);
            usleep(QUANTUM1);
            kill(pid1, SIGSTOP);
            waitpid(pid1, &running1, WNOHANG);

            clock_gettime( CLOCK_REALTIME, &t_exec_end[0]);
            t_exec[0] = t_exec[0] +  time_difference(t_exec_start[0], t_exec_end[0]);

            if (running1 == 0)
            {    
                    //printf("exec time of p1-q1 = %0.6f \n", t_exec[0]);
                    t_wait[0] = time_difference(t_start[0], t_exec_end[0]) - t_exec[0];
                    //printf("wait time of p1-q1 = %0.6f \n",t_wait[0]);
            }  
            else
            {
                queue.task_queue[queue.num_tasks_in_queue]=1; 
                queue.num_tasks_in_queue++;
            }

                
        }
        if (running2 > 0){
            clock_gettime( CLOCK_REALTIME, &t_exec_start[1]);
                
            kill(pid2, SIGCONT);
            usleep(QUANTUM2);
            kill(pid2, SIGSTOP);

            clock_gettime( CLOCK_REALTIME, &t_exec_end[1]);

            t_exec[1] = t_exec[1] +  time_difference(t_exec_start[1], t_exec_end[1]);
                
            //printf("exec time of p2 = %0.6f \n", t_exec[1]);
            waitpid(pid2, &running2, WNOHANG);

            if (running2 == 0)
            {    
                //printf("exec time of p2-q1 = %0.6f \n", t_exec[1]);
                t_wait[1] = time_difference(t_start[1], t_exec_end[1]) - t_exec[1];
                //printf("wait time of p2-q1 = %0.6f \n",t_wait[1]);
            }  
            else
            {
                queue.task_queue[queue.num_tasks_in_queue]=2; 
                queue.num_tasks_in_queue++;
            }
               
        }
        if (running3 > 0){
            clock_gettime( CLOCK_REALTIME, &t_exec_start[2]);

            kill(pid3, SIGCONT);
            usleep(QUANTUM3);
            kill(pid3, SIGSTOP);
            
            clock_gettime( CLOCK_REALTIME, &t_exec_end[2]);

            t_exec[2] = t_exec[2] +  time_difference(t_exec_start[2], t_exec_end[2]);

            //printf("exec time of p3 = %0.6f \n", t_exec[2]);
            waitpid(pid3, &running3, WNOHANG);

            if (running3 == 0)
            {
                
                //printf("exec time of p3-q1 = %0.6f \n", t_exec[2]);
                t_wait[2] = time_difference(t_start[2], t_exec_end[2]) - t_exec[2];
                //printf("wait time of p3-q1 = %0.6f \n",t_wait[2]);
            }
            else
            {
                queue.task_queue[queue.num_tasks_in_queue]=3; 
                queue.num_tasks_in_queue++;
            }
                
        }
        if (running4 > 0){
            clock_gettime( CLOCK_REALTIME, &t_exec_start[3]);

            kill(pid4, SIGCONT);
            usleep(QUANTUM4);
            kill(pid4, SIGSTOP);

            clock_gettime( CLOCK_REALTIME, &t_exec_end[3]);

            t_exec[3] = t_exec[3] +  time_difference(t_exec_start[3], t_exec_end[3]);

            //printf("exec time of p3 = %0.6f \n", t_exec[3]);
            waitpid(pid4, &running4, WNOHANG);
            if (running4 == 0)
            {
                
                //printf("exec time of p4-q1 = %0.6f \n", t_exec[3]);
                t_wait[3] = time_difference(t_start[3], t_exec_end[3]) - t_exec[3];
                //printf("wait time of p4-q1 = %0.6f \n",t_wait[3]);
            }  
            else
            {
                queue.task_queue[queue.num_tasks_in_queue]=4; 
                queue.num_tasks_in_queue++;
            }
                

        }

        printf("number of tasks in queue 2 = %d \n\n",queue.num_tasks_in_queue);

        for(int q=0; q<=queue.num_tasks_in_queue; q++)
        {

            if(queue.task_queue[q]!=0)
            {
                switch(queue.task_queue[q])
                {

                    case 1:
                            // run Q2 as FCFS
                            if (running1 > 0){
                                clock_gettime( CLOCK_REALTIME, &t_exec_start[0]);
                                kill(pid1, SIGCONT);
                                waitpid(pid1 , &running1, 0);

                                clock_gettime( CLOCK_REALTIME, &t_exec_end[0]);
                                t_exec[0] = t_exec[0] +  time_difference(t_exec_start[0], t_exec_end[0]);

                                //printf("exec time of p1-q2 = %0.6f \n", t_exec[0]);
                                t_wait[0] = time_difference(t_start[0], t_exec_end[0]) - t_exec[0];
                                //printf("wait time of p1-q2 = %0.6f \n",t_wait[0]);
                            }

                        break;

                    case 2: 
                            // run W2
                            if (running2 > 0){
                                clock_gettime( CLOCK_REALTIME, &t_exec_start[1]);

                                
                                kill(pid2, SIGCONT);
                                waitpid(pid2 , &running2, 0);
                                clock_gettime( CLOCK_REALTIME, &t_exec_end[1]);

                                t_exec[1] = t_exec[1] +  time_difference(t_exec_start[1], t_exec_end[1]);

                                //printf("exec time of p2-q2 = %0.6f \n", t_exec[1]);
                                t_wait[1] = time_difference(t_start[1], t_exec_end[1]) - t_exec[1];
                                //printf("wait time of p2-q2 = %0.6f \n",t_wait[1]);
                            }


                        break;

                    case 3: 
                            // run W3
                            if (running3 > 0){
                                clock_gettime( CLOCK_REALTIME, &t_exec_start[2]);
                                
                                kill(pid3, SIGCONT);
                                waitpid(pid3 , &running3, 0);
                                clock_gettime( CLOCK_REALTIME, &t_exec_end[2]);

                                t_exec[2] = t_exec[2] +  time_difference(t_exec_start[2], t_exec_end[2]);

                               
                                //printf("exec time of p3-q2 = %0.6f \n", t_exec[2]);
                                t_wait[2] = time_difference(t_start[2], t_exec_end[2]) - t_exec[2];
                                //printf("wait time of p3-q2 = %0.6f \n",t_wait[2]);
                            }
                        break;

                    case 4:
                        // run W4
                        if (running4 > 0){
                            clock_gettime( CLOCK_REALTIME, &t_exec_start[3]);
                            
                            kill(pid4, SIGCONT);
                            waitpid(pid4 , &running4, 0);
                            clock_gettime( CLOCK_REALTIME, &t_exec_end[3]);

                            t_exec[3] = t_exec[3] +  time_difference(t_exec_start[3], t_exec_end[3]);

                            //printf("exec time of p4-q2 = %0.6f \n", t_exec[3]);
                            t_wait[3] = time_difference(t_start[3], t_exec_end[3]) - t_exec[3];
                            //printf("wait time of p4-q2 = %0.6f \n",t_wait[3]);

                        }
                        break;

                    default:
                        break;
                }

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