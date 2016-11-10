/* Cassidy Brown - cmb195
 * OS - HW 8
 * Problem 2: Readers/Writers Problem
 */

#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>
#include <unistd.h>  

#define BUFFER_SIZE 25 

char* g_buffer;
int read_count, version = 0;
sem_t rw_mutex, r_mutex;

void *reader(void *param); /* the thread */

int main(int argc, char *argv[])
{
    if(sem_init(&rw_mutex, 0, 1) < 0) { // 0 = multithreaded
        fprintf(stderr, "ERROR: could not initialize &semaphore.\n");
        exit(0);
    }
    if(sem_init(&r_mutex, 0, 1) < 0) { // 0 = multithreaded
        fprintf(stderr, "ERROR: could not initialize &semaphore.\n");
        exit(0);
    }

    pthread_t tid1, tid2; /* the thread identifier */
    pthread_attr_t attr;  //set of attributes for the thread 

    /* get the default attributes */
    pthread_attr_init(&attr);

    /* create the thread */
    pthread_create(&tid1,&attr,reader,argv[1]);
   pthread_create(&tid2,&attr,reader,argv[1]);

    //I don't like this code repetition thing, but I'm also too lazy to deal with mallocing. or callocing. or whatever I'm supposed to do
    // Each String is written to the buffer, then the writer sleeps for 1 second
    sem_wait(&rw_mutex);
    g_buffer = "I solemnly swear\n";
    version++;
    sem_post(&rw_mutex);
    sleep(1);
    sem_wait(&rw_mutex);
    g_buffer = "to tell the truth\n";
    version++;
    sem_post(&rw_mutex);
    sleep(1);
    sem_wait(&rw_mutex);
    g_buffer = "the whole truth\n";
    version++;
    sem_post(&rw_mutex);
    sleep(1);
    sem_wait(&rw_mutex);
    g_buffer = "and nothing but the truth\n";
    version++;
    sem_post(&rw_mutex);
    sleep(1);
    sem_wait(&rw_mutex);
    g_buffer = "so help me god\n";
    version++;
    sem_post(&rw_mutex);

    /* now wait for the thread to exit */
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
}

/**
 * The thread will begin control in this function
 */
void *reader(void *param) 
{
    // local variable storing last recieved buffer version
    int current_version = 0;
    
    //Storing thread id as an int because I can't seem to print pthread_t
    int tid = (int)pthread_self();

    // We expect five Strings to be written to the buffer. 
    //Once they have all been written, we can exit
    while(current_version < 5){
        //check if version has changed
        if(current_version != version){
            //Update the local version variable
            current_version = version;
            
            sem_wait(&r_mutex);
            read_count++;
            if(read_count == 1)
                sem_wait(&rw_mutex);
            sem_post(&r_mutex);
            
            // read
            printf("%d: %s", tid, g_buffer);

            sem_wait(&r_mutex);
            read_count--;
            if(read_count == 0)
                sem_post(&rw_mutex);
            sem_post(&r_mutex);
        }
    }



  pthread_exit(0);
}
