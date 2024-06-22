#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pthread.h>
#include <ncurses.h>

#define SHM_NAME "/chat_shm"
#define MAX_SIZE 1000
#define SHM_SIZE 4096
#define MSG_STOP "exit"
#define USER_JOINED "User joined: "
#define USER_LEFT "User left: "
#define MAX_NAME 100

pthread_mutex_t user_list_mutex = PTHREAD_MUTEX_INITIALIZER;
char user_list[MAX_SIZE][MAX_NAME];
int user_count = 0;

WINDOW *msg_win, *input_win, *user_win;
char username[MAX_NAME];

void init_ui() {
    initscr();
    cbreak();
    curs_set(FALSE);
    refresh();
    int height, width;
    getmaxyx(stdscr, height, width);

    user_win = newwin(height, width / 4, 0, width - width / 4);
    msg_win = newwin(height - 3, width - width / 4, 0, 0);
    input_win = newwin(3, width - width / 4, height - 3, 0);

    scrollok(msg_win, TRUE);
    box(user_win, 0, 0);
    box(input_win, 0, 0);
}

void close_ui() {
    delwin(msg_win);
    delwin(input_win);
    delwin(user_win);
    endwin();
}

void update_user_list() {
    werase(user_win);
    box(user_win, 0, 0);
    mvwprintw(user_win, 1, 1, "Users:");
    pthread_mutex_lock(&user_list_mutex);
    for (int i = 0; i < user_count; ++i) {
        mvwprintw(user_win, i + 2, 1, user_list[i]);
    }
    pthread_mutex_unlock(&user_list_mutex);
    wrefresh(user_win);
}

void display_message(const char *message) {
    wprintw(msg_win, "%s\n", message);
    wrefresh(msg_win);
}

void *receive_messages(void *arg) {
    char *shm_ptr = (char *)arg;
    char buffer[MAX_SIZE];

    while (1) {
      
        if (strcmp(shm_ptr, "") != 0) {
            strncpy(buffer, shm_ptr, MAX_SIZE);
            shm_ptr[0] = '\0';

            if (strncmp(buffer, MSG_STOP, strlen(MSG_STOP)) == 0) {
                break;
            }

            if (strncmp(buffer, USER_JOINED, strlen(USER_JOINED)) == 0) {
                pthread_mutex_lock(&user_list_mutex);
                strncpy(user_list[user_count], buffer + strlen(USER_JOINED), MAX_NAME - 1);
                user_list[user_count][MAX_NAME - 1] = '\0';
                user_count++;
                pthread_mutex_unlock(&user_list_mutex);
                update_user_list();
                display_message(buffer);
            } else if (strncmp(buffer, USER_LEFT, strlen(USER_LEFT)) == 0) {
                pthread_mutex_lock(&user_list_mutex);
                for (int i = 0; i < user_count; ++i) {
                    if (strcmp(user_list[i], buffer + strlen(USER_LEFT)) == 0) {
                        for (int j = i; j < user_count - 1; ++j) {
                            strncpy(user_list[j], user_list[j + 1], MAX_NAME);
                        }
                        user_count--;
                        break;
                    }
                }
                pthread_mutex_unlock(&user_list_mutex);
                update_user_list();
                display_message(buffer);
            } else {
                display_message(buffer);
            }
        }

        usleep(100000);  
    }

    return NULL;
}

void *send_messages(void *arg) {
    char *shm_ptr = (char *)arg;
    char buffer[MAX_SIZE];
    char message[MAX_SIZE + MAX_NAME];

    while (1) {
        werase(input_win);
        box(input_win, 0, 0);
        mvwgetnstr(input_win, 1, 1, buffer, MAX_SIZE - 2);
        wrefresh(input_win);

        snprintf(message, sizeof(message) + 1, "%s: %s", username, buffer);
        strncpy(shm_ptr, message, SHM_SIZE);

        if (strncmp(buffer, MSG_STOP, strlen(MSG_STOP)) == 0) {
            break;
        }

        usleep(100000);  
    }

    return NULL;
}

int main() {
    printf("Enter your username: ");
    fgets(username, MAX_NAME, stdin);
    username[strcspn(username, "\n")] = '\0';

    init_ui();

    int shm_fd = shm_open(SHM_NAME, O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("Error opening shm");
        close_ui();
        exit(1);
    }

    char *shm_ptr = mmap(0, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shm_ptr == MAP_FAILED) {
        perror("Error maping");
        close_ui();
        exit(1);
    }

    char join_message[MAX_SIZE];
    snprintf(join_message, sizeof(join_message), "%s%s", USER_JOINED, username);
    strncpy(shm_ptr, join_message, SHM_SIZE);

    pthread_t receive_thread, send_thread;
    pthread_create(&receive_thread, NULL, receive_messages, (void *)shm_ptr);
    pthread_create(&send_thread, NULL, send_messages, (void *)shm_ptr);

    pthread_join(send_thread, NULL);

    char leave_message[MAX_SIZE];
    snprintf(leave_message, sizeof(leave_message), "%s%s", USER_LEFT, username);
    strncpy(shm_ptr, leave_message, SHM_SIZE);

    pthread_cancel(receive_thread);
    pthread_join(receive_thread, NULL);

    if (munmap(shm_ptr, SHM_SIZE) == -1) {
        perror("Error unmaping");
    }

    close(shm_fd);
    close_ui();

    return 0;
}
