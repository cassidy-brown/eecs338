/* Cassidy Brown - cmb195
 * OS - HW 6
 * Problem 3: Semaphores with multithreading
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

int g_count = 0; /* this data is shared by the thread(s) */
sem_t sem;

void *child(void *param); /* the thread's function */

int main(int argc, char *argv[])
{
	if(sem_init(&sem, 0, 1) < 0) { // 0 = multithreaded
		fprintf(stderr, "ERROR: could not initialize &semaphore.\n");
		exit(0);
	}

    pthread_t tid; /* the thread identifier */
    pthread_attr_t attr; /* set of attributes for the thread */

    /* get the default attributes */
    pthread_attr_init(&attr);

    /* create the thread */
    pthread_create(&tid,&attr,child,argv[1]);

    // Do the counter things in the parent thread
    int count = 0;
    while(count < 100000000){
    	sem_wait(&sem);
      	g_count++;
    	sem_post(&sem);
    	count++;
    }

    // Print parent results
    printf("Parent thread count: %d\n", count);

    /* now wait for the thread to exit */
    pthread_join(tid,NULL);

    // destory semaphore
    sem_destroy(&sem);

    // Print global results
    printf("Global counter: %d\n", g_count);
}

/**
 * The thread will begin control in this function
 */
void *child(void *param) 
{
    int count = 0;

    // Do the counter things in the child thread
    while(count > -100000000){
    	sem_wait(&sem);
      	g_count--;
    	sem_post(&sem);
    	count--;
    }

    // Print child results
    printf("Child thread count: %d\n", count);

  pthread_exit(0);
}
