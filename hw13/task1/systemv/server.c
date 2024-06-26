#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define SERVER_QUEUE_KEY 1234
#define CLIENT_QUEUE_KEY 5678
#define MAX_SIZE 1024

struct message {
    long mtype;
    char mtext[MAX_SIZE];
};

int main() {
    int server_qid, client_qid;
    struct message msg;

    server_qid = msgget(SERVER_QUEUE_KEY, IPC_CREAT | 0666);
    if (server_qid == -1) {
        perror("error creating server queue");
        exit(1);
    }


    if (msgrcv(server_qid, &msg, sizeof(msg.mtext), 0, 0) == -1) {
        perror("error receiving message");
        exit(1);
    }

    printf("Received message: %s\n", msg.mtext);

    client_qid = msgget(CLIENT_QUEUE_KEY, IPC_CREAT | 0666);
    if (client_qid == -1) {
        perror("error creating client queue");
        exit(1);
    }

    msg.mtype = 1;
    strcpy(msg.mtext, "Hello!");
    if (msgsnd(client_qid, &msg, sizeof(msg.mtext), 0) == -1) {
        perror("error sending message");
        exit(1);
    }

    return 0;
}
