#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>

pthread_t Reader[101], Writer[101];
sem_t mutex, db;
int data = 0;
int rc = 0;

void *read()
{
    sem_wait(&mutex);
    rc++;
    if (rc == 1)
        sem_wait(&db);
    sem_post(&mutex);
    printf("Reader no. %d reading data %d\n", rc, data);
    sem_wait(&mutex);
    rc--;
    if (rc == 0)
        sem_post(&db);
    sem_post(&mutex);
}

void *write()
{
    sem_wait(&db);
    data++;
    printf("Writer Changed data to : %d\n", data);
    sem_post(&db);
}

int main()
{
    int n = 5;
    sem_init(&mutex, 0, 1);
    sem_init(&db, 0, 1);
    for (int i = 0; i < n; i++)
    {
        pthread_create(&Writer[i], NULL, write, NULL);
        pthread_create(&Reader[i], NULL, read, NULL);
    }

    for (int i = 0; i < n; i++)
    {
        pthread_join(Writer[i], NULL);
        pthread_join(Reader[i], NULL);
    }
    return 0;
}