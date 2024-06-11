#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mqueue.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#define QUEUE_NAME "/test_queue2"
#define MAX_SIZE 1024

int main()
{
    mqd_t mq;
    struct mq_attr attr;
    char buffer[MAX_SIZE];
    char *queue_path = "/test_queue2";
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = MAX_SIZE;
    attr.mq_curmsgs = 0;

    mq = mq_open(queue_path, O_CREAT | O_WRONLY, 0644, &attr);
    if (mq == -1)
    {
        perror("error opening queue");
        exit(1);
    }

    strcpy(buffer, "Hi!");
    if (mq_send(mq, buffer, strlen(buffer) + 1, 0) == -1)
    {
        perror("error send message");
        exit(1);
    }

    mq_close(mq);
    sleep(10);

    mq = mq_open(queue_path, O_RDONLY);
    if (mq == -1)
    {
        perror("error opening queue");
        exit(1);
    }

    if (mq_receive(mq, buffer, MAX_SIZE, NULL) == -1)
    {
        perror("error receive message");
        exit(1);
    }

    printf("Received message: %s\n", buffer);

    mq_close(mq);
    mq_unlink(queue_path);

    return 0;
}
