#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define N 5
#define THINKING 2
#define HUNGRY 1
#define EATING 0

pthread_mutex_t mutex;
pthread_cond_t bowl_cond;

int state[N]; // Added state array
pthread_mutex_t bowl_locks[N]; // Added locks for each bowl

struct Philosopher
{
    int id;
    pthread_t thread_id;
    pthread_cond_t cond;
    struct Philosopher *next;
    struct Philosopher *prev; // Added prev pointer for doubly linked list
};

struct Philosopher *philosophers;

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

void test(struct Philosopher *phil)
{
    if (state[phil->id] == HUNGRY &&
        state[phil->next->id] != EATING &&
        state[phil->prev->id] != EATING)
    {
        // state that eating
        state[phil->id] = EATING;

        pthread_cond_signal(&phil->cond);
    }
}

void take_fork(struct Philosopher *phil)
{
    pthread_mutex_lock(&mutex);

    // state that hungry
    state[phil->id] = HUNGRY;

    // Pick up both forks
    test(phil);

    while (state[phil->id] != EATING)
        pthread_cond_wait(&phil->cond, &mutex);

    pthread_mutex_unlock(&mutex);
}

void put_fork(struct Philosopher *phil)
{
    pthread_mutex_lock(&mutex);

    // idhar thinking aaega
    state[phil->id] = THINKING;

    test(phil->next);
    test(phil->prev); // Update to check the previous philosopher to avoid circular waiting

    pthread_mutex_unlock(&mutex);
}

void *philosopher(void *philosopher)
{
    struct Philosopher *phil = (struct Philosopher *)philosopher;

    while (1)
    {
        thinking(phil->id);

        take_fork(phil);

        // Now acquire the bowl lock
        pthread_mutex_lock(&bowl_locks[phil->id]);

        eating(phil->id);

        // Release the bowl lock
        pthread_mutex_unlock(&bowl_locks[phil->id]);

        put_fork(phil);

        // Signal that a bowl is available
        pthread_mutex_lock(&mutex);
        pthread_cond_signal(&bowl_cond);
        pthread_mutex_unlock(&mutex);
    }
}

void initialize_philosophers()
{
    philosophers = NULL;

    for (int i = 0; i < N; i++)
    {
        pthread_mutex_init(&bowl_locks[i], NULL);
    }

    for (int i = N - 1; i >= 0; i--)
    {
        struct Philosopher *new_philosopher = malloc(sizeof(struct Philosopher));
        new_philosopher->id = i;
        pthread_cond_init(&new_philosopher->cond, NULL);
        new_philosopher->next = philosophers;
        new_philosopher->prev = NULL;

        if (philosophers != NULL)
        {
            philosophers->prev = new_philosopher;
        }

        pthread_create(&new_philosopher->thread_id, NULL, philosopher, new_philosopher);

        philosophers = new_philosopher;
    }

    // Update the prev pointer of the last philosopher to complete the doubly linked circular list
    struct Philosopher *temp = philosophers;
    while (temp->next != NULL)
    {
        temp = temp->next;
    }
    temp->next = philosophers;
    philosophers->prev = temp;
}

int main()
{
    pthread_mutex_init(&mutex, NULL); // Initialize the global mutex
    pthread_cond_init(&bowl_cond, NULL); // Initialize the bowl condition variable

    initialize_philosophers();

    // Join all threads in the circular linked list
    struct Philosopher *temp = philosophers;
    do
    {
        pthread_join(temp->thread_id, NULL);
        temp = temp->next;
    } while (temp != philosophers);

    // destroy mutex, conditions, and locks
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&bowl_cond);
    for (int i = 0; i < N; i++)
    {
        pthread_mutex_destroy(&bowl_locks[i]);
    }

    // Clean up the philosophers
    temp = philosophers;
    do
    {
        struct Philosopher *next_philosopher = temp->next;
        free(temp);
        temp = next_philosopher;
    } while (temp != philosophers);

    return 0;
}
