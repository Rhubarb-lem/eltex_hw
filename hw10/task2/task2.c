#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    pid_t pid1 = fork();
    int status;
    if (pid1 == 0)
    {
        pid_t pid3 = fork();
        if (pid3 == 0)
        {
            printf("Process 3 pid: %d\n", getpid());
            printf("Process 3 ppid: %d\n", getppid());
            exit(5);
        }
        else
        {
            pid_t pid4 = fork();
            if (pid4 == 0)
            {
                printf("Process 4 pid: %d\n", getpid());
                printf("Process 4 ppid: %d\n", getppid());
                exit(5);
            }
            else
            {
                printf("Process 1 pid: %d\n", getpid());
                printf("Process 1 ppid: %d\n", getppid());
                wait(&status);
                exit(5);
            }
        }
    }
    else
    {
        pid_t pid2 = fork();
        if (pid2 == 0)
        {
            pid_t pid5 = fork();
            if(pid5 == 0){
                printf("Process 5 pid: %d\n", getpid());
                printf("Process 5 ppid: %d\n", getppid());
                exit(5);
            }
            else{
                printf("Process 2 pid: %d\n", getpid());
                printf("Process 2 ppid: %d\n", getppid());
                wait(&status);
                exit(5);
            }

        }
        else
        {
            printf("Root 2 pid: %d\n", getpid());
            printf("Root 2 ppid: %d\n", getppid());
            wait(&status);
        }
    }
    return 0;
}