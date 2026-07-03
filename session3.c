#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define THREADS 6

sem_t semaphore;

void *worker(void *arg)
{
    int id = *(int *)arg;

    printf("Thread %d is waiting to enter...\n", id);

    sem_wait(&semaphore);

    printf(">>> Thread %d ENTERED the critical section.\n", id);

    // Simulate some work
    sleep(3);

    printf("<<< Thread %d EXITING the critical section.\n", id);

    sem_post(&semaphore);

    return NULL;
}

int main()
{
    pthread_t threads[THREADS];
    int id[THREADS];

    // Counting semaphore allowing 3 threads at once
    sem_init(&semaphore, 0, 3);

    for(int i = 0; i < THREADS; i++)
    {
        id[i] = i + 1;
        pthread_create(&threads[i], NULL, worker, &id[i]);
    }

    for(int i = 0; i < THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }

    sem_destroy(&semaphore);

    printf("\nAll threads have completed.\n");

    return 0;
}