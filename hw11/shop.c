#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>

pthread_mutex_t mutex;
int shop[] = {1000, 1000, 1000, 1000, 1000};

void *customer(void *arg)
{

    int money = 10000;

    // int *shop = *((int *)arg);

    while (money > 0)
    {

        pthread_mutex_lock(&mutex);

        for (int i = 0; i < 5; i++)
        {

            if ((money - shop[i]) > 0)
            {
                // printf("Money has run out!");
                money -= shop[i];
            }
            else
            {
                // printf("Money has run out!");
                break;
            }
        }
        //printf("Money has run out!");
        pthread_mutex_unlock(&mutex);
        sleep(2);
        //printf("Money has run out!");
        
    }
    printf("Money has run out!");
    return NULL;
}

int main()
{
   
    pthread_t threads[3];
    pthread_mutex_init(&mutex, NULL);
    for (int i = 0; i < 3; i++)
    {
        pthread_create(&threads[i], NULL, customer, NULL);
    }
    // while(1){
        
    // }
    //pthread_mutex_destroy(&mutex);
    return 0;
}