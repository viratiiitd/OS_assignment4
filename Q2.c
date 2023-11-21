#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define MAX_PASSENGERS 100

// Global variables
int total_passengers;
int car_capacity;
sem_t car_sem, board_sem, unboard_sem;

// Function prototypes
void* car(void* args);
void* passenger(void* args);
void load();
void unload();
void board(int passenger_id);
void offboard(int passenger_id);

int main() {
    // Input: Total passengers and car capacity
    printf("Enter the total number of passengers: ");
    scanf("%d", &total_passengers);
    
    printf("Enter the car capacity: ");
    scanf("%d", &car_capacity);

    // Validate input
    if (total_passengers <= 0 || car_capacity <= 0 || car_capacity > total_passengers) {
        printf("Invalid input. Exiting.\n");
        return 1;
    }

    // Initialize semaphores
    sem_init(&car_sem, 0, 0);
    sem_init(&board_sem, 0, 0);
    sem_init(&unboard_sem, 0, 0);

    // Create car thread
    pthread_t car_thread;
    pthread_create(&car_thread, NULL, car, NULL);

    // Create passenger threads
    pthread_t passenger_threads[MAX_PASSENGERS];
    for (int i = 0; i < total_passengers; ++i) {
        int* passenger_id = malloc(sizeof(int));
        *passenger_id = i;
        pthread_create(&passenger_threads[i], NULL, passenger, passenger_id);
    }

    // Join threads
    pthread_join(car_thread, NULL);
    for (int i = 0; i < total_passengers; ++i) {
        pthread_join(passenger_threads[i], NULL);
    }

    // Destroy semaphores
    sem_destroy(&car_sem);
    sem_destroy(&board_sem);
    sem_destroy(&unboard_sem);

    return 0;
}

void* car(void* args) {
    while (1) {
        load();
        sem_post(&car_sem); // Signal passengers to board
        printf("Car is fully loaded. Starting the ride!\n");
        sleep(2); // Simulate ride duration
        unload();
    }
}

void* passenger(void* args) {
    int passenger_id = *((int*)args);
    free(args);

    while (1) {
        sem_wait(&car_sem); // Wait for the car to be loaded
        board(passenger_id);
        sem_post(&board_sem); // Signal the car that passenger has boarded
        sem_wait(&unboard_sem); // Wait for the car to unload
        offboard(passenger_id);
    }
}

void load() {
    printf("Car is loading passengers...\n");
    sleep(1); // Simulate loading time
}

void unload() {
    printf("Car is unloading passengers...\n");
    sleep(1); // Simulate unloading time
}

void board(int passenger_id) {
    printf("Passenger %d is boarding the car.\n", passenger_id);
    sleep(1); // Simulate boarding time
}

void offboard(int passenger_id) {
    printf("Passenger %d is getting off the car.\n", passenger_id);
    sleep(1); // Simulate offboarding time
}
