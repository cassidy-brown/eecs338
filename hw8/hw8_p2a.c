/* Cassidy Brown - cmb195
 * OS - HW 8
 * Problem 2a: Readers/Writers with no end in sight...
 */

#include <pthread.h>
#include <stdio.h>

#define N 10
#define BUFFER_SIZE 5 

int g_buffer[BUFFER_SIZE] = {0, 0, 0, 0, 0};

void *reader(void *param); /* the thread */

int main(int argc, char *argv[])
{
    pthread_t tid1, tid2; /* the thread identifier */
    pthread_attr_t attr; /* set of attributes for the thread */

    int data[N] = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
    int write_index;

    /* get the default attributes */
    pthread_attr_init(&attr);

    /* create the thread */
    pthread_create(&tid1,&attr,reader,argv[1]);
    pthread_create(&tid2,&attr,reader,argv[1]);

    // Write the values to the buffer
    int i = 0;
    while(i < N){
        write_index = i % BUFFER_SIZE;
        g_buffer[write_index] = data[i];
        i++;
    }

    /* now wait for the thread to exit */
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
}

/**
 * The thread will begin control in this function
 */
void *reader(void *param) 
{
    int i = 0;
    int read_index;

    // Read N values from the buffer
    while(i < N){
        read_index = i % BUFFER_SIZE;
        printf("%d\n", g_buffer[read_index]);
        i++;
    }


  pthread_exit(0);
}
