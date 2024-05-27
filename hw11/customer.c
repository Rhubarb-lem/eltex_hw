#include"customer.h"
int shop[] = {1000, 1000, 1000, 1000, 1000};
void *customer(void *arg)
{
    int money = 10000;
    long id = (long)arg;

    while (money > 0)
    {

        for (int i = 0; i < 5; i++)
        {

            if (money >= shop[i])
            {
                pthread_mutex_lock(&mutex);
                money -= shop[i];
                printf("Customer %ld: Spent %d, remaining money %d\n", id, shop[i], money);
                pthread_mutex_unlock(&mutex);
                sleep(2);
            }
            else
            {
                printf("Customer %ld: Not enough money to spend on item %d\n", id, i);
                break;
            }
        }
    }

    printf("Customer %ld: Money has run out!\n", id);
    return NULL;
}