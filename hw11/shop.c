#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "customer.h"


int main()
{
    pthread_t threads[3];
    pthread_mutex_init(&mutex, NULL);

    for (long i = 0; i < 3; i++)
    {
        pthread_create(&threads[i], NULL, customer, (void *)i);
    }

    for (int i = 0; i < 3; i++)
    {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    return 0;
}
