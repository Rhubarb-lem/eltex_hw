#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mqueue.h>
#include <fcntl.h>
#include <sys/stat.h>

#define SERVER_QUEUE_NAME "/server_queue"
#define CLIENT_QUEUE_NAME "/client_queue"
#define MAX_SIZE 1024

int main() {
    mqd_t server_mq, client_mq;
    char buffer[MAX_SIZE];

    server_mq = mq_open(SERVER_QUEUE_NAME, O_RDONLY);
    if (server_mq == -1) {
        perror("error opening queue");
        exit(1);
    }


    if (mq_receive(server_mq, buffer, MAX_SIZE, NULL) == -1) {
        perror("error receive message");
        exit(1);
    }

    printf("Received message: %s\n", buffer);

    mq_close(server_mq);


    client_mq = mq_open(CLIENT_QUEUE_NAME, O_WRONLY);
    if (client_mq == -1) {
        perror("error opening queue");
        exit(1);
    }

    strcpy(buffer, "Hello!");
    if (mq_send(client_mq, buffer, strlen(buffer) + 1, 0) == -1) {
        perror("error send message");
        exit(1);
    }

    mq_close(client_mq);

    return 0;
}
