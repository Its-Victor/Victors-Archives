#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <semaphore.h>

typedef struct
{
    sem_t sem;
    char message[100];
} SharedMemory;

int main()
{
    key_t key = ftok("shmfile", 65);

    int shmid = shmget(key, sizeof(SharedMemory), 0666 | IPC_CREAT);

    if(shmid == -1)
    {
        perror("shmget");
        exit(1);
    }

    SharedMemory *shared = (SharedMemory *)shmat(shmid, NULL, 0);

    if(shared == (void *)-1)
    {
        perror("shmat");
        exit(1);
    }

    // Initialize semaphore for inter-process sharing
    sem_init(&shared->sem, 1, 0);

    pid_t pid = fork();

    if(pid < 0)
    {
        perror("fork");
        exit(1);
    }

    if(pid == 0)
    {
        // Child Process

        printf("Child: Waiting for parent...\n");

        sem_wait(&shared->sem);

        printf("Child: Message received:\n");
        printf("%s\n", shared->message);

        shmdt(shared);

        exit(0);
    }
    else
    {
        // Parent Process

        sleep(2);

        strcpy(shared->message,
        "Hello Child!\n"
        "This message was sent through Shared Memory.\n"
        "Synchronization achieved using POSIX Semaphore.");

        printf("Parent: Message written to shared memory.\n");

        sem_post(&shared->sem);

        wait(NULL);

        sem_destroy(&shared->sem);

        shmdt(shared);

        shmctl(shmid, IPC_RMID, NULL);

        printf("Shared memory cleaned up successfully.\n");
    }

    return 0;
}