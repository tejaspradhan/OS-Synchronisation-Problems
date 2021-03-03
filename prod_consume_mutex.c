#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define SIZE 5

int buffer[SIZE];
int count = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t producer_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t consumer_cond = PTHREAD_COND_INITIALIZER;

void *produce()
{
    for (int i = 0; i < 10; i++)
    {
        pthread_mutex_lock(&mutex);
        if (count == SIZE)
        {
            pthread_cond_wait(&producer_cond, &mutex);
        }
        buffer[count] = count;
        count++;
        printf("Producer produced item  : %d \n", count);
        pthread_mutex_unlock(&mutex);
        pthread_cond_signal(&consumer_cond);
    }
}

void *consume()
{
    int item;
    for (int i = 0; i < 10; i++)
    {
        pthread_mutex_lock(&mutex);
        if (count <= -1)
        {
            pthread_cond_wait(&consumer_cond, &mutex);
        }
        printf("Consumer consumed item : %d \n", count);
        count--;
        pthread_mutex_unlock(&mutex);
        pthread_cond_signal(&producer_cond);
    }
}

int main()
{
    pthread_t producer, consumer;
    pthread_create(&producer, NULL, produce, NULL);
    pthread_create(&consumer, NULL, consume, NULL);
    pthread_join(producer, NULL);
    pthread_join(consumer, NULL);
    pthread_mutex_destroy(&mutex);
    return 0;
}