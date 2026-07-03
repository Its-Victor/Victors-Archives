#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define THREADS 5
#define COUNT 100000

int counter = 0;
pthread_mutex_t mutex;

void *increment(void *arg)
{
    for(int i = 0; i < COUNT; i++)
    {
        pthread_mutex_lock(&mutex);

        counter++;

        pthread_mutex_unlock(&mutex);
    }

    return NULL;
}

int main()
{
    pthread_t tid[THREADS];

    pthread_mutex_init(&mutex, NULL);

    for(int i=0;i<THREADS;i++)
        pthread_create(&tid[i], NULL, increment, NULL);

    for(int i=0;i<THREADS;i++)
        pthread_join(tid[i], NULL);

    printf("Final Counter = %d\n", counter);

    pthread_mutex_destroy(&mutex);

    return 0;
}