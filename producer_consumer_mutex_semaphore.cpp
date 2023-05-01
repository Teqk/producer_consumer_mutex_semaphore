// Operating Systems - CS444/544
// Spring 2023
// Your Name
// Assignment Info: Producer-Consumer Problem using mutexes and semaphores
// Program Description: This program solves the producer-consumer problem using mutexes to protect the shared buffer and semaphores to synchronize the access to the buffer.
// Last Modified: [date]
// YouTube Code Review & Demo link: [insert link]

#define _XOPEN_SOURCE 600 // for POSIX semaphores
#define _POSIX_C_SOURCE 200809L // for usleep

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define BUFFER_SIZE 10

int buffer[BUFFER_SIZE];    // shared buffer
int buffer_index = 0;       // shared buffer index

pthread_mutex_t mutex;      // mutex lock to protect the shared buffer
sem_t full;                 // semaphore to count the number of full slots in the buffer
sem_t empty;                // semaphore to count the number of empty slots in the buffer

void* producer(void* arg) {
    int item = 0;
    while (true) {
        // produce an item
        item++;
        
        // wait for an empty slot in the buffer
        sem_wait(&empty);
        
        // acquire the mutex lock to access the shared buffer
        pthread_mutex_lock(&mutex);
        
        // insert the item into the buffer
        buffer[buffer_index] = item;
        buffer_index++;
        
        // release the mutex lock and signal that the buffer is now not empty
        pthread_mutex_unlock(&mutex);
        sem_post(&full);
        
        // simulate some processing time
        usleep(1000);
    }
    return NULL;
}

void* consumer(void* arg) {
    int item = 0;
    while (true) {
        // wait for a full slot in the buffer
        sem_wait(&full);
        
        // acquire the mutex lock to access the shared buffer
        pthread_mutex_lock(&mutex);
        
        // remove an item from the buffer
        buffer_index--;
        item = buffer[buffer_index];
        buffer[buffer_index] = 0;
        
        // release the mutex lock and signal that the buffer is now not full
        pthread_mutex_unlock(&mutex);
        sem_post(&empty);
        
        // consume the item
        printf("Consumed item: %d\n", item);
        
        // simulate some processing time
        usleep(1000);
    }
    return NULL;
}

int main() {
    // initialize the mutex lock and semaphores
    pthread_mutex_init(&mutex, NULL);
    sem_init(&full, 0, 0);
sem_init(&empty, 0, BUFFER_SIZE);

// create the producer and consumer threads
pthread_t producer_thread, consumer_thread;
pthread_create(&producer_thread, NULL, producer, NULL);
pthread_create(&consumer_thread, NULL, consumer, NULL);

// wait for the threads to finish
pthread_join(producer_thread, NULL);
pthread_join(consumer_thread, NULL);

// destroy the mutex lock and semaphores
pthread_mutex_destroy(&mutex);
sem_destroy(&full);
sem_destroy(&empty);

return 0;
}
