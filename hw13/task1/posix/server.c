#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mqueue.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#define QUEUE_NAME "/test_queue2"
#define MAX_SIZE 1024

int main() {
    mqd_t mq;
    struct mq_attr attr;
    char buffer[MAX_SIZE];
    char *queue_path = "/test_queue2";
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = MAX_SIZE;
    attr.mq_curmsgs = 0;

    // Открываем очередь сообщений для записи
    mq = mq_open(queue_path, O_CREAT | O_WRONLY, 0644, &attr);
    if (mq == -1) {
        perror("mq_open");
        exit(1);
    }

    // Отправляем сообщение "Hi!"
    strcpy(buffer, "Hi!");
    if (mq_send(mq, buffer, strlen(buffer) + 1, 0) == -1) {
        perror("mq_send");
        exit(1);
    }

    // Закрываем очередь для записи
    mq_close(mq);
    sleep(10);

    // Открываем очередь для чтения
    mq = mq_open(queue_path, O_RDONLY);
    if (mq == -1) {
        perror("mq_open");
        exit(1);
    }

    // Ждем ответ от клиента
    if (mq_receive(mq, buffer, MAX_SIZE, NULL) == -1) {
        perror("mq_receive");
        exit(1);
    }

    // Выводим ответ клиента
    printf("Received message: %s\n", buffer);

    // Закрываем и удаляем очередь
    mq_close(mq);
    mq_unlink(queue_path);

    return 0;
}
