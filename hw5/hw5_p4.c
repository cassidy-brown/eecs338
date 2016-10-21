/* Cassidy Brown - cmb195
 * OS - HW 5
 * Problem 3: Race conditions with fork
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

#define N 10000

int main()
{
	const int SIZE = 1; // Use an appropriate size for the array
	const char *name = "cmb";
	int shm_fd;
	int *g_count;
	int count;

	shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
	ftruncate(shm_fd,SIZE);
	g_count = mmap(0,SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
	if (g_count == MAP_FAILED) {
		printf("Map failed\n");
		return -1;
	}

	pid_t pid;
	pid = fork();
	if (!pid) {		// Child

		// Do decrementing counter things
		for(count = 0; count > -1000000000; count--){
			g_count[0] = g_count[0] - 1;
		}

		// Print process's counter result
    	printf("Child thread count: %d\n", count);

    	return 0;
    }
	else {		// Parent
		
		// Do incrementing counter things
		for(count = 0; count < 1000000000; count++){
			g_count[0] = g_count[0] + 1;
		}

		// Print process's counter result
    	printf("Parent thread count: %d\n", count);

		// Make sure the child has finished
		wait(NULL);

		// Print net counter result
		printf("Global counter: %d\n", g_count[0]);	
	}

	return 0;
}
