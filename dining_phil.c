#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <semaphore.h>

#define N 5 // Number of philosophers
#define LEFT (i + N - 1) % N
#define RIGHT (i + 1) % N
#define THINKING 0
#define HUNGRY 1
#define EATING 2

pthread_t phil_thread[N];
sem_t s[N], mutex;
int state[N];
int philosophers[N];
void test(int i)
{
    if (state[i] == HUNGRY && state[LEFT] != EATING && state[RIGHT] != EATING)
    {
        printf("Philosopher %d is eating\n", i);
        state[i] = EATING;
        sem_post(&s[i]);
    }
}
void take_forks(int i)
{
    sem_wait(&mutex);
    printf("Philosopher %d is hungry\n", i);
    state[i] = HUNGRY;
    test(i);
    sem_post(&mutex);
    sem_wait(&s[i]);
}

void put_forks(int i)
{
    sem_wait(&mutex);
    state[i] = THINKING;
    test(LEFT);
    test(RIGHT);
    sem_post(&mutex);
}
void *philosopher(void *index)
{
    while (1)
    {
        int *i = index;
        printf("Philosopher %d is thinking\n", *i);
        take_forks(*i);
        put_forks(*i);
    }
}
int main()
{
    sem_init(&mutex, 0, 1);
    for (int i = 0; i < N; i++)
    {
        sem_init(&s[i], 0, 0);
        philosophers[i] = i;
    }
    for (int i = 0; i < N; i++)
    {
        pthread_create(&phil_thread[i], NULL, philosopher, &philosophers[i]);
    }
    for (int i = 0; i < N; i++)
    {
        pthread_join(phil_thread[i], NULL);
    }
    return 0;
}