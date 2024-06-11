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
    struct mq_attr attr;
    char buffer[MAX_SIZE];

    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = MAX_SIZE;
    attr.mq_curmsgs = 0;

    // Открываем очередь сообщений для отправки клиенту
    server_mq = mq_open(SERVER_QUEUE_NAME, O_CREAT | O_WRONLY, 0644, &attr);
    if (server_mq == -1) {
        perror("error opening queue");
        exit(1);
    }

    // Отправляем сообщение "Hi!" клиенту
    strcpy(buffer, "Hi!");
    if (mq_send(server_mq, buffer, strlen(buffer) + 1, 0) == -1) {
        perror("error send message");
        exit(1);
    }

    // Закрываем очередь для отправки
    mq_close(server_mq);

    // Открываем очередь для получения ответа от клиента
    client_mq = mq_open(CLIENT_QUEUE_NAME, O_CREAT | O_RDONLY, 0644, &attr);
    if (client_mq == -1) {
        perror("error opening queue");
        exit(1);
    }

    // Ждем ответ от клиента
    if (mq_receive(client_mq, buffer, MAX_SIZE, NULL) == -1) {
        perror("error receive message");
        exit(1);
    }

    // Выводим ответ клиента
    printf("Received message: %s\n", buffer);

    // Закрываем и удаляем очереди
    mq_close(client_mq);
    mq_unlink(SERVER_QUEUE_NAME);
    mq_unlink(CLIENT_QUEUE_NAME);

    return 0;
}
