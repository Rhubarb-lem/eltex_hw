#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    pid_t pid = fork();
    int status;
    if (pid == 0){
        printf("Child pid: %d\n", getpid());
        printf("Child ppid: %d\n", getppid());
        exit(0);
    }
    else{
        printf("Parent pid: %d\n", getpid());
        printf("Parent ppid: %d\n", getppid());
        wait(&status);
    }
   return 0;
}