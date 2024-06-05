#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#define MSG_SIZE 4

int main()
{

    char *fifo_path = "./taskfifo";
    if (mkfifo(fifo_path,0644) == -1)//Write only for the creator. In this case, you need to start the client using sudo
    {
        perror("Pipe file create error");
        exit(1);
    }
    int fd = open(fifo_path,  O_WRONLY);
    if (fd == -1) {
        perror("open");
        exit(1);
    }
    char msg[MSG_SIZE] = "Hi!";
    write(fd, msg, sizeof(msg));
    printf("Message sended to pipe\n");
    close(fd);
    if (unlink(fifo_path) == -1) {
        perror("Unlink file error");
        exit(1);
    }
    
    return 0;
}
