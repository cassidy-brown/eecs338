/* Cassidy Brown - cmb195
 * OS - HW 6
 * Problem 2: Peterson's solution with fork
 */

#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/wait.h>

#define N 10000
#define PARENT 0
#define CHILD 1


int main()
{
	const int SIZE = 1; // Use an appropriate size for the array
	const int SIZE2 = 2;
	const char *name = "cmb";
	const char *name1 = "cmb1";
	const char *name2 = "cmb2";
	int shm_fd, shm_fd_turn, shm_fd_flag;
	int *g_count;
	int *turn;
	int *flag;
	int count;

	// Allocate shared memory for the global count variable
	shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
	ftruncate(shm_fd,SIZE);
	g_count = mmap(0,SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
	if (g_count == MAP_FAILED) {
		printf("Map failed\n");
		return -1;
	}
	// Allocate shared memory for Peterson's turn variable
	shm_fd_turn = shm_open(name1, O_CREAT | O_RDWR, 0667);
	ftruncate(shm_fd_turn,SIZE);
	turn = mmap(0,SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd_turn, 0);
	if (turn == MAP_FAILED) {
		printf("Map failed\n");
		return -1;
	}
	// Allocate shared memory for Peterson's flag variable
	shm_fd_flag = shm_open(name2, O_CREAT | O_RDWR, 0670);
	ftruncate(shm_fd_flag,SIZE2);
	flag = mmap(0,SIZE2, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd_flag, 0);
	if (flag == MAP_FAILED) {
		printf("Map failed\n");
		return -1;
	}

	pid_t pid;
	pid = fork();
	if (!pid) {		// Child

		// Do decrementing counter things
		int count = 0;
	    while(count > -100000000){
	    	//entry
	    	flag[CHILD] = true;
	    	*turn = PARENT;	
	    	while(flag[PARENT] && *turn == PARENT) ; 	//wait
	    	//critical section
	    	g_count[0] = g_count[0] - 1;
	    	// exit
	    	flag[CHILD] = false;
	    	//remainder section
	    	count--;
	    }

		// Print process's counter result
    	printf("Child process count: %d\n", count);

    	return 0;
    }
	else {		// Parent
		
		// Do incrementing counter things
	    count = 1;
	    while(count < 100000000){
	    	//entry
	    	flag[PARENT] = true;
	    	*turn = CHILD;
	    	while(flag[CHILD] && *turn == CHILD) ; 	//wait
	    	//critical section
	    	g_count[0] = g_count[0] + 1;
	    	// exit
	    	flag[PARENT] = false;
	    	//remainder section
	    	count++;
	    }

		// Print process's counter result
    	printf("Parent process count: %d\n", count);

		// Make sure the child has finished
		wait(NULL);

		//Unlink all shared memory
		if (shm_unlink(name) == -1) {
			printf("Error removing %s\n",name);
			exit(0);
		}
		if (shm_unlink(name1) == -1) {
			printf("Error removing %s\n",name1);
			exit(0);
		}
		if (shm_unlink(name2) == -1) {
			printf("Error removing %s\n",name2);
			exit(0);
		}

		// Print net counter result
		printf("Global counter: %d\n", g_count[0]);	
	}

	return 0;
}
