#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mqueue.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <pthread.h>
#include <unistd.h>
#include <ncurses.h>

#define QUEUE_NAME "/chat_queue"
#define MAX_SIZE 1000
#define MAX_NAME 100
#define MSG_STOP "exit"
#define USER_JOINED "User joined: "
#define USER_LEFT "User left: "

mqd_t mq;
char username[MAX_NAME];
pthread_mutex_t user_list_mutex = PTHREAD_MUTEX_INITIALIZER;
char user_list[MAX_SIZE][MAX_NAME];
int user_count = 0;

WINDOW *msg_win, *input_win, *user_win;

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
    char buffer[MAX_SIZE];
    mqd_t receive_mq = mq_open(QUEUE_NAME, O_RDONLY);
    if (receive_mq == -1) {
        perror("Error opening queue");
        exit(1);
    }

    while (1) {
        ssize_t bytes_read = mq_receive(receive_mq, buffer, MAX_SIZE, NULL);
        if (bytes_read < 0) {
            perror("Error receiveng message");
            exit(1);
        }
        buffer[bytes_read] = '\0';

        if (strncmp(buffer, MSG_STOP, strlen(MSG_STOP)) == 0) {
            break;
        }

        if (strncmp(buffer, USER_JOINED, strlen(USER_JOINED)) == 0) {
             pthread_mutex_lock(&user_list_mutex);
            strncpy(user_list[user_count], buffer + strlen(USER_JOINED), MAX_SIZE - 1);
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
                        strncpy(user_list[j], user_list[j + 1], MAX_SIZE);
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

    mq_close(receive_mq);
    return NULL;
}

void *send_messages(void *arg) {
    char buffer[MAX_SIZE];
    char message[MAX_SIZE + 100]; 
    mqd_t send_mq = mq_open(QUEUE_NAME, O_WRONLY);
    if (send_mq == -1) {
        perror("Error opening queue");
        exit(1);
    }

    while (1) {
        werase(input_win);
        box(input_win, 0, 0);
        mvwgetnstr(input_win, 1, 1, buffer, MAX_SIZE - 2);
        wrefresh(input_win);

        snprintf(message, sizeof(message)+1, "%s: %s", username, buffer);
        if (mq_send(send_mq, message, strlen(message) + 1, 0) == -1) {
            perror("Error sending message");
            exit(1);
        }

        if (strncmp(buffer, MSG_STOP, strlen(MSG_STOP)) == 0) {
            break;
        }
    }

    mq_close(send_mq);
    return NULL;
}

int main() {
    printf("Enter your username: ");
    fgets(username, MAX_NAME, stdin);
    username[strcspn(username, "\n")] = '\0'; 

    init_ui();

    mq = mq_open(QUEUE_NAME, O_WRONLY);
    if (mq == -1) {
        perror("Client: mq_open");
        close_ui();
        exit(1);
    }

    char join_message[MAX_SIZE];
    snprintf(join_message, sizeof(join_message), "%s%s", USER_JOINED, username);
    if (mq_send(mq, join_message, strlen(join_message) + 1, 0) == -1) {
        perror("Client: mq_send join_message");
        close_ui();
        exit(1);
    }

    pthread_t receive_thread, send_thread;
    pthread_create(&receive_thread, NULL, receive_messages, NULL);
    pthread_create(&send_thread, NULL, send_messages, NULL);

    pthread_join(send_thread, NULL);

    char leave_message[MAX_SIZE];
    snprintf(leave_message, sizeof(leave_message), "%s %s", USER_LEFT, username);
    if (mq_send(mq, leave_message, strlen(leave_message) + 1, 0) == -1) {
        perror("Client: mq_send leave_message");
    }

    pthread_cancel(receive_thread);
    pthread_join(receive_thread, NULL);

    mq_close(mq);
    close_ui();

    return 0;
}