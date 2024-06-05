#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#define MSG_SIZE 4

int main()
{
    char *fifo_path = "./taskfifo";
    int fd = open(fifo_path, O_RDONLY);
    if (fd == -1)
    {
        perror("open");
        exit(1);
    }
    char msg[MSG_SIZE];
    read(fd, msg, MSG_SIZE);
    printf("Received message: %s\n", msg);
    close(fd);
    return 0;
}