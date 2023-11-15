#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#define N 5
#define THINKING 2
#define HUNGRY 1
#define EATING 0

pthread_mutex_t mutex;
pthread_cond_t cond[N];
pthread_mutex_t bowl_mutex;
pthread_cond_t bowl_cond;
int state[N];
int phil[N] = {0, 1, 2, 3, 4};
int bowls_available = 2;

void eating(int phnum)
{
    printf("Philosopher %d is Eating\n", phnum + 1);
    sleep(2);
}

void thinking(int phnum)
{
    printf("Philosopher %d is Thinking\n", phnum + 1);
    sleep(1);
}

void test(int phnum)
{
    if (state[phnum] == HUNGRY && state[(phnum + 1) % N] != EATING && state[(phnum + 2) % N] != EATING && bowls_available > 0)
    {
        // state that eating
        state[phnum] = EATING;

        pthread_cond_signal(&cond[phnum]);
        bowls_available--;
    }
}

void take_fork(int phnum)
{
    pthread_mutex_lock(&mutex);

    // state that hungry
    state[phnum] = HUNGRY;

    printf("Philosopher %d is Hungry\n", phnum + 1);

    // eat if neighbors are not eating and there is a bowl available
    test(phnum);

    while (state[phnum] != EATING)
        pthread_cond_wait(&cond[phnum], &mutex);

    pthread_mutex_unlock(&mutex);
}

void put_fork(int phnum)
{
    pthread_mutex_lock(&mutex);

    // idhar thinking aaega
    state[phnum] = THINKING;

    
	printf("\n");

    test((phnum + 1) % N);
    test((phnum + 2) % N);

    pthread_mutex_unlock(&mutex);
}

void *philosopher(void *num)
{
    while (1)
    {
        int *i = num;

        thinking(*i);

        take_fork(*i);

        eating(*i);

        put_fork(*i);

        pthread_mutex_lock(&bowl_mutex);
        bowls_available++;
        pthread_cond_signal(&bowl_cond);
        pthread_mutex_unlock(&bowl_mutex);
    }
}

int main()
{
    pthread_t thread_id[N];

    // initialize the mutex and conditions
    pthread_mutex_init(&mutex, NULL);
    pthread_mutex_init(&bowl_mutex, NULL);
    pthread_cond_init(&bowl_cond, NULL);

    for (int i = 0; i < N; i++)
        pthread_cond_init(&cond[i], NULL);

    for (int i = 0; i < N; i++)
    {
        pthread_create(&thread_id[i], NULL, philosopher, &phil[i]);
        // printf("Philosopher %d is thinking\n", i + 1);
    }

    for (int i = 0; i < N; i++)
        pthread_join(thread_id[i], NULL);

    // destroy mutex and conditions
    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&bowl_mutex);
    pthread_cond_destroy(&bowl_cond);
    for (int i = 0; i < N; i++)
        pthread_cond_destroy(&cond[i]);

    return 0;
}
