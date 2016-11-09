/* Cassidy Brown - cmb195
 * OS - HW 6
 * Problem 1: Peterson's solution with multithreading
 */

#include <pthread.h>
#include <stdio.h>
#include <stdbool.h>

#define PARENT 0
#define CHILD 1

int turn;
int flag[2] = {0, 0};
int g_count = 0; /* this data is shared by the thread(s) */

void *runner(void *param); /* the thread */

int main(int argc, char *argv[])
{
    pthread_t tid; /* the thread identifier */
    pthread_attr_t attr; /* set of attributes for the thread */


    /* get the default attributes */
    pthread_attr_init(&attr);

    /* create the thread */
    pthread_create(&tid,&attr,runner,argv[1]);

    // Do the counter things in the parent thread
    int count = 0;
    while(count < 100000000){
    	//entry
    	flag[PARENT] = true;
    	turn = CHILD;
    	while(flag[CHILD] && turn == CHILD) ; 	//wait
    	//critical section
    	g_count++;
    	// exit
    	flag[PARENT] = false;
    	//remainder section
    	count++;
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
    while(count > -100000000){
    	//entry
    	flag[CHILD] = true;
    	turn = PARENT;
    	while(flag[PARENT] && turn == PARENT) ; 	//wait
    	//critical section
    	g_count--;
    	// exit
    	flag[CHILD] = false;
    	//remainder section
    	count--;
    }

    // Print child results
    printf("Child thread count: %d\n", count);

  pthread_exit(0);
}
