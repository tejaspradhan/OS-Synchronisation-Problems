#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>

#define SIZE 5

int buffer[SIZE];
int count = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
sem_t empty, full;
void *produce()
{
    for (int i = 0; i < 10; i++)
    {
        sem_wait(&empty);
        pthread_mutex_lock(&mutex);
        buffer[count] = count;
        count++;
        printf("Producer produced item  : %d \n", count);
        pthread_mutex_unlock(&mutex);
        sem_post(&full);
    }
}

void *consume()
{
    int item;
    for (int i = 0; i < 10; i++)
    {
        sem_wait(&full);
        pthread_mutex_lock(&mutex);
        printf("Consumer consumed item : %d \n", count);
        count--;
        pthread_mutex_unlock(&mutex);
        sem_post(&empty);
    }
}

int main()
{
    pthread_t producer, consumer;
    sem_init(&empty, 0, SIZE); // initially everything is empty
    sem_init(&full, 0, 0);
    pthread_create(&producer, NULL, produce, NULL);
    pthread_create(&consumer, NULL, consume, NULL);
    pthread_join(producer, NULL);
    pthread_join(consumer, NULL);
    pthread_mutex_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&full);
    return 0;
}