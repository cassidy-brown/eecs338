/**
 * Cassidy Brown - cmb195
 * HW3, problem 3
 * 
 * Adapted code from Silberschatz, Galvin, and Gagne */
#include <unistd.h>
#include <sys/types.h>


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>

int main()
{
	const int SIZE = 4096;
	const char *name = "cmb";
	const char *message= "Sharing is caring\n";

	int shm_fd;
	void *ptr;

	pid_t pid;

	// Fork the process
	pid = fork();

	if(pid == 0){ // CHILD (writer)
		/* The child process creates the shared memory space 
		 * and writes the message to it */

		/* Create the shared memory segment */
		shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);

		/* Configure the size of the shared memory segment */
		ftruncate(shm_fd,SIZE);

		/* Map the shared memory segment in the address space of the child process */
		ptr = mmap(0,SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
		if (ptr == MAP_FAILED) {
			printf("Map failed\n");
			return -1;
		}

		/* Write to the shared memory */
		sprintf(ptr,"%s",message);

	} else { // PARENT (reader)
		/* The parent process waits for the child to write to memory
		 * then prints from the shared memory and unlinks it */

		/* Wait for child to finish executing */
		wait(NULL);

		/* Open the shared memory segment */
		shm_fd = shm_open(name, O_RDONLY, 0666);
		if (shm_fd == -1) {
			printf("shared memory failed\n");
			exit(-1);
		}

		/* Map the shared memory segment in the address space of the parent process */
		ptr = mmap(0,SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);
		if (ptr == MAP_FAILED) {
			printf("Map failed\n");
			exit(-1);
		}

		/* Read from the shared memory region */
		printf("%s",ptr);

		/* Remove the shared memory segment */
		if (shm_unlink(name) == -1) {
			printf("Error removing %s\n",name);
			exit(-1);
		}
	}

	return 0;
}