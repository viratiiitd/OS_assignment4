#include<stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

int total_passengers, car_capacity;
int passenger_id = 0;
sem_t mutex,passengers;
bool boardingAllowed = false;
bool deBoardingAllowed = false;

void load() {
    sleep(1);
    printf("Loading completed\n");
    //fflush(stdout);
    boardingAllowed = true;
} 

void passing() {
    boardingAllowed = false;
    printf("Car is running\n");
    fflush(stdout);
    sleep(1);
}

void unload() {
    sleep(1);
    printf("Unloading completed\n");
    fflush(stdout);
    deBoardingAllowed = true;
    sem_post(&mutex);
    int curr_passengers;
    sem_getvalue(&passengers,&curr_passengers);
    while (curr_passengers != car_capacity) {
        sem_getvalue(&passengers,&curr_passengers);
    }
    deBoardingAllowed = false;
}    

void car() {
    while (true) {
        sem_wait(&mutex);
        load();
        sem_post(&mutex);
        int current_passengers;
        sem_getvalue(&passengers,&current_passengers); 
        while (current_passengers != 0) {
            sem_wait(&mutex);
            sem_getvalue(&passengers,&current_passengers);
            sem_post(&mutex);
        }
        boardingAllowed = false;
        sem_wait(&mutex);
        passing();
        unload();
        
    }
}                 

void board(int curr) {
    //sem_wait(&mutex);
    sem_wait(&passengers);
    printf("Passenger %d boarding \n",curr);
    fflush(stdout);
    fflush(stdout);
    sleep(1);
    //sem_post(&mutex);
}                 

void offboard(int curr) {
    //sem_wait(&mutex);
    sem_post(&passengers);
    printf("Passenger %d deboarding \n",curr);
    fflush(stdout);
    fflush(stdout);
    sleep(1);
    //sem_post(&mutex);
}   

void passenger() {
    int curr_passenger_id = passenger_id++;
    while (true) {
        if (boardingAllowed) {
            board(curr_passenger_id);
            while (!deBoardingAllowed) {
                continue;
            }
            offboard(curr_passenger_id);
        }
        //board(curr_passenger_id);
    }
}   

        

int main() {
    pthread_t car_thread;
    printf("Enter total number of passengers: ");
    scanf("%d", &total_passengers);
    printf("Enter car capacity: ");
    scanf("%d", &car_capacity);

    if (total_passengers <= 0 || car_capacity <= 0) {
        printf("Invalid input.\n");
        return 1;
    }
    sem_init(&mutex,0,1);
    sem_init(&passengers,0,car_capacity);
    //sem_init(&load,0,1);

    pthread_t passenger_threads[total_passengers];

    pthread_create(&car_thread, NULL,(void*)&car,NULL);

    for (int i=0;i<total_passengers;i++) {
        pthread_create(&passenger_threads[i],NULL,(void*)&passenger,NULL);
    }

    pthread_join(car_thread, NULL);
    for (int i = 0; i < total_passengers; i++) {
        pthread_join(passenger_threads[i], NULL);
    }

    sem_destroy(&mutex);
    sem_destroy(&passengers);



    return 0;
}
