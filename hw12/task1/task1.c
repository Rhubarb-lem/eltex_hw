#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#define MSG_SIZE 4

int main()
{
    int fd[2];
    if (pipe(fd) == -1)
    {
        perror("Pipe create error");
        exit(1);
    }
    pid_t pid = fork();
    if (pid < 0)
    {
        perror("Fork create error");
        exit(1);
    }
    int status;
    if (pid != 0)
    {
        close(fd[0]);
        char smsg[MSG_SIZE] = "Hi!";
        write(fd[1],smsg, sizeof(smsg));
        close(fd[1]);
        wait(&status);
    }
    else
    {
        close(fd[1]);
        char rmsg[MSG_SIZE];
        read(fd[0], rmsg, MSG_SIZE);
        printf("Received message: %s\n", rmsg);
        close(fd[0]);
        exit(0);
    }
}