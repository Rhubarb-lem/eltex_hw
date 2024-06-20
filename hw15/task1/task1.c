#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

// Обработчик сигнала
void signal_handler()
{

    printf("Received SIGUSR1\n");
    exit(0);
}

int main()
{
    struct sigaction sa;
    sa.sa_handler = signal_handler;
    sa.sa_flags = 0; 

    sigaction(SIGUSR1, &sa, NULL);

    while (1)
    {
        printf("Waiting...\n");
        //sleep(1);
    }

    return 0;
}
