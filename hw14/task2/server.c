#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pthread.h>

#define SHM_NAME "/chat_shm"
#define MAX_SIZE 1000
#define SHM_SIZE 4096
#define MSG_STOP "exit"
#define USER_JOINED "User joined: "
#define USER_LEFT "User left: "

pthread_mutex_t user_mutex = PTHREAD_MUTEX_INITIALIZER;

void notify_all_clients(const char *message) {
    // Печать сообщения для всех клиентов
    printf("Server: %s\n", message);
}

void *handle_client(void *arg) {
    char *shm_ptr = (char *)arg;
    char buffer[MAX_SIZE];

    while (1) {
        
        if (strcmp(shm_ptr, "") != 0) {
            strncpy(buffer, shm_ptr, MAX_SIZE);
            shm_ptr[0] = '\0';

            if (strncmp(buffer, MSG_STOP, strlen(MSG_STOP)) == 0) {
                break;
            }

            notify_all_clients(buffer);
        }

        usleep(100000);  
    }

    pthread_exit(NULL);
}

int main() {
    int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("Error creating shm");
        exit(EXIT_FAILURE);
    }

    if (ftruncate(shm_fd, SHM_SIZE) == -1) {
        perror("Error truncate");
        exit(EXIT_FAILURE);
    }

    char *shm_ptr = mmap(0, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shm_ptr == MAP_FAILED) {
        perror("Error maping");
        exit(EXIT_FAILURE);
    }

    pthread_t client_thread;
    pthread_create(&client_thread, NULL, handle_client, (void *)shm_ptr);

    pthread_join(client_thread, NULL);

    if (munmap(shm_ptr, SHM_SIZE) == -1) {
        perror("Error unmaoing");
        exit(EXIT_FAILURE);
    }

    if (shm_unlink(SHM_NAME) == -1) {
        perror("Error unlink");
        exit(EXIT_FAILURE);
    }

    close(shm_fd);

    return 0;
}
