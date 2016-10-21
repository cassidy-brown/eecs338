/* Cassidy Brown - cmb195
 * OS - HW 5
 * Problem 3: Race conditions via normal threads
 */

#include <pthread.h>
#include <stdio.h>
#include <math.h>

int g_count = 0; /* this data is shared by the thread(s) */

void *runner(void *param); /* the thread */

int main(int argc, char *argv[])
{
    pthread_t tid; /* the thread identifier */
    pthread_attr_t attr; /* set of attributes for the thread */

    int count = 0;

    /* get the default attributes */
    pthread_attr_init(&attr);

    /* create the thread */
    pthread_create(&tid,&attr,runner,argv[1]);

    // Do the counter things in the parent thread
    for(count = 0; count < 1000000000; count++){
      g_count++;
    }

    // Print parent results
    printf("Parent thread count: %d\n", count);

    /* now wait for the thread to exit */
    pthread_join(tid,NULL);

    // Print global results
    printf("Global counter: %d\n", g_count);
}

/**
 * The thread will begin control in this function
 */
void *runner(void *param) 
{
    int count = 0;

    // Do the counter things in the child thread
    for(count = 0; count > -1000000000; count--){
      g_count--;
    }

    // Print child results
    printf("Child thread count: %d\n", count);

  pthread_exit(0);
}
