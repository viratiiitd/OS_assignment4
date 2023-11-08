#include <stdio.h>
#include <pthread.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *philosopher(void *arg){
    int i;
    for (i = 0; i < 10; i++) {
        // Acquire the lock
        pthread_mutex_lock(&mutex);

        // Access the shared resource
        shared_variable++;
        printf("Thread %ld: shared_variable = %d\n", (long)arg, shared_variable);

        // Release the lock
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main() {
    pthread_t thread1, thread2;

    // Create two threads
    pthread_create(&thread1, NULL, philosopher, (void*)1);
    pthread_create(&thread2, NULL, philosopher, (void*)2);

    // Wait for threads to finish
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    return 0;
}
