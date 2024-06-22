#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void signal_handler(int sig)
{
    if (sig == SIGINT)
    {
        printf("Received SIGINT\n");
    }
}

int main()
{
    struct sigaction sa;
    sigset_t new_set;

    sigemptyset(&new_set);
    sigaddset(&new_set, SIGINT);

    sigprocmask(SIG_BLOCK, &new_set, NULL);

    while (1)
    {
    }

    return 0;
}
