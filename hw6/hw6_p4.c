/* Cassidy Brown - cmb195
 * OS - HW 6
 * Problem 4: Semaphores with fork
 */

#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <semaphore.h>

int main()
{
	const int SIZE = 1; // Use an appropriate size for the array
	const char *name = "cmb";
	const char *name_sem = "cmb_sem";
	int shm_fd, shm_fd_sem;
	int *g_count;
	sem_t *sem;
	int count;

	/* Allocate shared memory for global counter variable */
	shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
	ftruncate(shm_fd,SIZE);
	g_count = mmap(0,SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
	if (g_count == MAP_FAILED) {
		printf("Map failed\n");
		return -1;
	}

	/* Allocate shared memory for semaphore */
	shm_fd_sem = shm_open(name_sem, O_CREAT | O_RDWR, 0667);
	ftruncate(shm_fd_sem,SIZE);
	sem = mmap(0,SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd_sem, 0);
	if (sem == MAP_FAILED) {
		printf("Map failed\n");
		return -1;
	}

	/* Initialize semaphore */
	if(sem_init(sem, 1, 1) < 0) { // 1 = multiprocess
		fprintf(stderr, "ERROR: could not initialize semaphore.\n");
		exit(0);
	}

	pid_t pid;
	pid = fork();
	if (!pid) {		// Child

		// Do decrementing counter things
		count = 0;
		while(count > -100000000){
			sem_wait(sem);
			g_count[0] = g_count[0] - 1;
			sem_post(sem);
			count--;
		}

		// Print process's counter result
    	printf("Child process count: %d\n", count);

    	return 0;
    }
	else {		// Parent
		
		// Do incrementing counter things
		count = 0;
		while(count < 100000000){
			sem_wait(sem);
			g_count[0] = g_count[0] + 1;
			sem_post(sem);
			count++;
		}

		// Print process's counter result
    	printf("Parent process count: %d\n", count);

		// Make sure the child has finished
		wait(NULL);

		/* Clean up: Destroy semaphore and unlink shared memory */
		sem_destroy(sem);
		if (shm_unlink(name) == -1) {
			printf("Error removing %s\n",name);
			exit(0);
		}
		if (shm_unlink(name_sem) == -1) {
			printf("Error removing %s\n",name_sem);
			exit(0);
		}

		// Print net counter result
		printf("Global counter: %d\n", g_count[0]);	
	}

	return 0;
}
