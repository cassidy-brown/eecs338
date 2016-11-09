/* Cassidy Brown - cmb195
 * OS - HW 7
 * Problem 2: Bounded buffer with synchronization
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

#define N 10
#define BUFFER_SIZE 5 

int g_buffer[BUFFER_SIZE] = {0, 0, 0, 0, 0};
sem_t sem_mutex, sem_empty, sem_full;

void *consumer(void *param); /* the thread */

int main(int argc, char *argv[])
{

    if(sem_init(&sem_mutex, 0, 1) < 0) { // 0 = multithreaded
        fprintf(stderr, "ERROR: could not initialize &semaphore.\n");
        exit(0);
    }
    if(sem_init(&sem_full, 0, 0) < 0) { // 0 = multithreaded
        fprintf(stderr, "ERROR: could not initialize &semaphore.\n");
        exit(0);
    }
    if(sem_init(&sem_empty, 0, BUFFER_SIZE) < 0) { // 0 = multithreaded
        fprintf(stderr, "ERROR: could not initialize &semaphore.\n");
        exit(0);
    }

    pthread_t tid; /* the thread identifier */
    pthread_attr_t attr; /* set of attributes for the thread */

    int data[N] = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
    int write_index;

    /* get the default attributes */
    pthread_attr_init(&attr);

    /* create the thread */
    pthread_create(&tid,&attr,consumer,argv[1]);

    // Producer
    // Write the values to the buffeR
    int i = 0;
    while(i < N){
        write_index = i % BUFFER_SIZE;

        sem_wait(&sem_empty);
        sem_wait(&sem_mutex);

        g_buffer[write_index] = data[i];

        sem_post(&sem_mutex);
        sem_post(&sem_full);
        i++;
    }

    /* now wait for the consumer thread to exit */
    pthread_join(tid,NULL);
}

/**
 * The thread will begin control in this function
 */
void *consumer(void *param) 
{
    int i = 0;
    int read_index;

    // Consumer
    // Read N values from the buffer
    while(i < N){
        read_index = i % BUFFER_SIZE;

        sem_wait(&sem_full);
        sem_wait(&sem_mutex);

        int item = g_buffer[read_index];

        sem_post(&sem_mutex);
        sem_post(&sem_empty);

        printf("%d\n", item);
        i++;
    }


  pthread_exit(0);
}
