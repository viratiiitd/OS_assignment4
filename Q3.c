#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

unsigned long leftCars, rightCars;
sem_t car_semaphore;
sem_t allSameCars;
int curr_direction = 0;
unsigned long carNumber = 1;

// direction = 0 for left and 1 for right

void passing(int direction, int currentCarNumber) {
    int number_of_cars;
    sem_getvalue(&car_semaphore, &number_of_cars);

    const char* direction_str = (direction == 0) ? "left" : "right";

    if (number_of_cars == 5) {
        //sem_post(&allSameCars);
        if (direction != curr_direction) {
            curr_direction = direction;
        }
    } 
    else if (number_of_cars != 5 && direction != curr_direction) {
        while (number_of_cars != 5) {
            sem_getvalue(&car_semaphore, &number_of_cars);
        }
        //sem_post(&allSameCars);
        if (direction != curr_direction) {
            curr_direction = direction;
        }
    }

    sem_wait(&car_semaphore);
    // Use mutex to protect printf
    printf("Car %d entering - %s\n", currentCarNumber, direction_str);
    fflush(stdout);

    sleep(1);

    printf("Car %d leaving - %s\n", currentCarNumber, direction_str);
    fflush(stdout);
    sem_post(&car_semaphore);
}


// Assuming one car takes 0.1 second to cross the bridge

// Assuming the first one will always be the left one
void left() {
    passing(0,carNumber++);
}

void right() {
    passing(1,carNumber++);
}

int min(int a, int b) {
    return (a < b) ? a : b;
}

int max(int a, int b) {
    return (a > b) ? a : b;
}

int main() {
    printf("Enter number of left cars: ");
    scanf("%ld", &leftCars);
    printf("Enter number of right cars: ");
    scanf("%ld", &rightCars);

    sem_init(&car_semaphore, 0, 5);
    sem_init(&allSameCars, 0, 1);

    // CarNumber
    pthread_t leftThreads[leftCars];
    pthread_t rightThreads[rightCars];
    int minimum = min(leftCars, rightCars);
    int maximum = max(leftCars, rightCars);
    for (unsigned long i = 0; i < minimum; i++) {
        pthread_create(&leftThreads[i], NULL, (void*)&left, NULL);
        pthread_create(&rightThreads[i], NULL, (void*)&right, NULL);
    }
    if (maximum == leftCars) {
        for (int i = rightCars; i < leftCars; i++) {
            pthread_create(&leftThreads[i], NULL, (void*)&left, NULL);
        }
    } else {
        for (int i = leftCars; i < rightCars; i++) {
            pthread_create(&rightThreads[i], NULL, (void*)&right, NULL);
        }
    }
    for (int i = 0; i < leftCars; ++i) {
        pthread_join(leftThreads[i], NULL);
    }

    for (int i = 0; i < rightCars; ++i) {
        pthread_join(rightThreads[i], NULL);
    }


    sem_destroy(&car_semaphore);
    sem_destroy(&allSameCars);

    pthread_exit(NULL);
}

