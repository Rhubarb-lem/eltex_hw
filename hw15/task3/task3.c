#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int main()
{
    sigset_t set;
    int sig;

    sigemptyset(&set);
    sigaddset(&set, SIGUSR1);
    if (sigprocmask(SIG_BLOCK, &set, NULL) == -1) {
        perror("sigprocmask");
        exit(EXIT_FAILURE);
    }

    while (1)
    {
        // if (sigwait(&set, &sig) == 0)
        // {
        //     if (sig == SIGUSR1)
        //     {
        //         printf("Received SIGUSR1\n");
        //     }
        // }
        sigwait(&set, &sig);
        printf("Received SIGUSR1\n");
    }

    return 0;
}
