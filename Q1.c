#include <stdio.h>
#include <pthread.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;



void *philosopher(void *arg){
    int i,shared_variable = 0 ;
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
    pthread_t thread0, thread1, thread2, thread3, thread4;

    // Create two threads
    for(int i = 0 ; i < 5 ; i++){
        pthread_create(&thread0, NULL, philosopher, (void*)i);
        pthread_create(&thread1, NULL, philosopher, (void*)i);
        pthread_create(&thread2, NULL, philosopher, (void*)i);
        pthread_create(&thread3, NULL, philosopher, (void*)i);
        pthread_create(&thread4, NULL, philosopher, (void*)i);
    }


    // Wait for threads to finish
    pthread_join(thread0, NULL);
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    pthread_join(thread3, NULL);
    pthread_join(thread4, NULL);

    return 0;
}
