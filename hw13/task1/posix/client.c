#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mqueue.h>
#include <fcntl.h>
#include <sys/stat.h>


#define MAX_SIZE 1024

int main() {
    mqd_t mq;
    char buffer[MAX_SIZE];
    char *queue_path = "/test_queue2";

    mq = mq_open(queue_path, O_RDONLY);
    if (mq == -1) {
        perror("mq_open");
        exit(1);
    }


    if (mq_receive(mq, buffer, MAX_SIZE, NULL) == -1) {
        perror("mq_receive");
        exit(1);
    }


    printf("Received message: %s\n", buffer);

    mq_close(mq);

    mq = mq_open(queue_path, O_WRONLY);
    if (mq == -1) {
        perror("mq_open");
        exit(1);
    }

    strcpy(buffer, "Hello!");
    if (mq_send(mq, buffer, strlen(buffer) + 1, 0) == -1) {
        perror("mq_send");
        exit(1);
    }

    mq_close(mq);

    return 0;
}
