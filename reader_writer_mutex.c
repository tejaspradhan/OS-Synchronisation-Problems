#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>

pthread_t Reader[101], Writer[101];
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t reader_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t writer_cond = PTHREAD_COND_INITIALIZER;
int data = 0;
int rc = 0;

void *read()
{

    pthread_mutex_lock(&mutex);
    rc++;
    if (rc == 1)
        pthread_cond_wait(&writer_cond, &mutex);
    pthread_mutex_unlock(&mutex);
    printf("Reader no. %d reading data %d\n", rc, data);
    pthread_mutex_lock(&mutex);
    rc--;
    if (rc == 0)
        pthread_cond_signal(&writer_cond);
    pthread_mutex_unlock(&mutex);
}

void *write()
{
    pthread_mutex_lock(&mutex);
    data++;
    printf("Writer Changed data to : %d\n", data);
    pthread_mutex_unlock(&mutex);
}

int main()
{
    int n = 5; // number of readers and writers
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