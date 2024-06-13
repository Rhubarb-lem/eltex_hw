#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mqueue.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <pthread.h>
#include <unistd.h>

#define QUEUE_NAME "/chat_queue"
#define MAX_SIZE 1024
#define MSG_STOP "exit"
#define USER_JOINED "User joined: "
#define USER_LEFT "User left: "

mqd_t mq;
char users[MAX_SIZE][MAX_SIZE];
int user_count = 0;
pthread_mutex_t user_mutex = PTHREAD_MUTEX_INITIALIZER;

void notify_all_clients(const char *message) {
    mqd_t mq_notify = mq_open(QUEUE_NAME, O_WRONLY);
    if (mq_notify == -1) {
        perror("Error openin queue");
        exit(1);
    }

    if (mq_send(mq_notify, message, strlen(message) + 1, 0) == -1) {
        perror("Error sending message");
        exit(1);
    }

    mq_close(mq_notify);
}

void *handle_client(void *arg) {
    char buffer[MAX_SIZE];
    mqd_t client_mq = *(mqd_t *)arg;

    while (1) {
        ssize_t bytes_read = mq_receive(client_mq, buffer, MAX_SIZE, NULL);
        if (bytes_read < 0) {
            perror("Error receiving message");
            pthread_exit(NULL);
        }
        buffer[bytes_read] = '\0';

        if (strncmp(buffer, MSG_STOP, strlen(MSG_STOP)) == 0) {
            break;
        }

        notify_all_clients(buffer);
    }

    mq_close(client_mq);
    pthread_exit(NULL);
}

int main() {
    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = MAX_SIZE;
    attr.mq_curmsgs = 0;

    mq = mq_open(QUEUE_NAME, O_CREAT | O_RDWR, 0644, &attr);
    if (mq == -1) {
        perror("Error opening queue");
        exit(1);
    }

    while (1) {
        char buffer[MAX_SIZE];
        ssize_t bytes_read = mq_receive(mq, buffer, MAX_SIZE, NULL);
        if (bytes_read < 0) {
            perror("Server: mq_receive");
            exit(1);
        }
        buffer[bytes_read] = '\0';

        if (strncmp(buffer, USER_JOINED, strlen(USER_JOINED)) == 0) {
            pthread_mutex_lock(&user_mutex);
            strncpy(users[user_count], buffer + strlen(USER_JOINED), MAX_SIZE - 1);
            users[user_count][MAX_SIZE - 1] = '\0';
            user_count++;
            pthread_mutex_unlock(&user_mutex);
            notify_all_clients(buffer);
        } else if (strncmp(buffer, USER_LEFT, strlen(USER_LEFT)) == 0) {
            pthread_mutex_lock(&user_mutex);
            for (int i = 0; i < user_count; ++i) {
                if (strcmp(users[i], buffer + strlen(USER_LEFT)) == 0) {
                    for (int j = i; j < user_count - 1; ++j) {
                        strncpy(users[j], users[j + 1], MAX_SIZE);
                    }
                    user_count--;
                    break;
                }
            }
            pthread_mutex_unlock(&user_mutex);
            notify_all_clients(buffer);
        } else {
            notify_all_clients(buffer);
        }
    }

    mq_close(mq);
    mq_unlink(QUEUE_NAME);

    return 0;
}