#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define MAX_PASSENGERS 100

int total_passengers;
int car_capacity;
int passengers_on_board = 0;

sem_t mutex, boarding_queue, unboarding_queue, ride_start, ride_end;

void load() {
    printf("Car is loading passengers.\n");
    sleep(1);
}

void unload() {
    printf("Car is unloading passengers.\n");
    sleep(1);
}

void board(int passenger_id) {
    printf("Passenger %d is boarding.\n", passenger_id);
    sleep(1);
}

void offboard(int passenger_id) {
    printf("Passenger %d is getting off.\n", passenger_id);
    sleep(1);
}

void* car(void* args) {
    while (1) {
        load();
        sem_wait(&boarding_queue);  // Wait for passengers to board
        sem_post(&ride_start);      // Signal to passengers that the ride has started
        sem_wait(&unboarding_queue);  // Wait for passengers to get off
        unload();
        passengers_on_board = 0;    // Reset passenger count
        sem_post(&ride_end);        // Signal to passengers that the ride has ended
    }
    pthread_exit(NULL);
}

void* passenger(void* args) {
    int passenger_id = *((int*)args);

    while (1) {
        sem_wait(&mutex);  // Enter critical section
        if (passengers_on_board < car_capacity) {
            passengers_on_board++;
            board(passenger_id);
            if (passengers_on_board == car_capacity) {
                sem_post(&boarding_queue);  // Signal to car that it can start
            }
            sem_post(&mutex);  // Exit critical section
            sem_wait(&ride_start);  // Wait for the ride to start
            offboard(passenger_id);
            sem_post(&unboarding_queue);  // Signal to car that passenger has disembarked
            sem_wait(&ride_end);  // Wait for the ride to end
        } else {
            sem_post(&mutex);  // If car is full, release mutex and try again
        }
        sleep(1);  // Simulate time between attempts
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t car_thread, passenger_threads[MAX_PASSENGERS];
    int passenger_ids[MAX_PASSENGERS];

    // Input total number of passengers and car capacity
    printf("Enter total number of passengers: ");
    scanf("%d", &total_passengers);
    printf("Enter car capacity: ");
    scanf("%d", &car_capacity);

    if (total_passengers <= 0 || car_capacity <= 0) {
        printf("Invalid input.\n");
        return 1;
    }

    // Initialize semaphores
    sem_init(&mutex, 0, 1);
    sem_init(&boarding_queue, 0, 0);
    sem_init(&unboarding_queue, 0, 0);
    sem_init(&ride_start, 0, 0);
    sem_init(&ride_end, 0, 0);

    // Create car thread
    pthread_create(&car_thread, NULL, car, NULL);

    // Create passenger threads
    for (int i = 0; i < total_passengers; i++) {
        passenger_ids[i] = i + 1;
        pthread_create(&passenger_threads[i], NULL, passenger, &passenger_ids[i]);
    }

    // Join car thread
    pthread_join(car_thread, NULL);

    // Join passenger threads
    for (int i = 0; i < total_passengers; i++) {
        pthread_join(passenger_threads[i], NULL);
    }

    // Destroy semaphores
    sem_destroy(&mutex);
    sem_destroy(&boarding_queue);
    sem_destroy(&unboarding_queue);
    sem_destroy(&ride_start);
    sem_destroy(&ride_end);

    return 0;
}
