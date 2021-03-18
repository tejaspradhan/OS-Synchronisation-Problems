#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>

#define SIZE 5

int buffer[SIZE];
int count = 0;
// pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
sem_t empty, full, mutex;
void *produce()
{
    for (int i = 0; i < 10; i++)
    {
        sem_wait(&empty);
        sem_wait(&mutex);
        buffer[count] = count;
        count++;
        printf("Producer produced item  : %d \n", count);
        sem_post(&mutex);
        sem_post(&full);
    }
}

void *consume()
{
    int item;
    for (int i = 0; i < 10; i++)
    {
        sem_wait(&full);
        sem_wait(&mutex);
        printf("Consumer consumed item : %d \n", count);
        count--;
        sem_post(&mutex);
        sem_post(&empty);
    }
}

int main()
{
    pthread_t producer, consumer;
    sem_init(&empty, 0, SIZE);
    sem_init(&full, 0, 0);
    sem_init(&mutex, 0, 1);
    pthread_create(&producer, NULL, produce, NULL);
    pthread_create(&consumer, NULL, consume, NULL);
    pthread_join(producer, NULL);
    pthread_join(consumer, NULL);
    // pthread_mutex_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&full);
    return 0;
}