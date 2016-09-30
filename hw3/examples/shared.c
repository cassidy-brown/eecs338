// Chris Fietkiewicz (chris.fietkiewicz@case.edu)
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

int main()
{
	const int SIZE = 4096;
	const char *name = "ChrisF";
	int shm_fd;
	void *ptr;

	/* create the shared memory segment */
	shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);

	/* configure the size of the shared memory segment */
	ftruncate(shm_fd,SIZE);

	/* now map the shared memory segment in the address space of the process */
	ptr = mmap(0,SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
	if (ptr == MAP_FAILED) {
		printf("Map failed\n");
		return -1;
	}

	pid_t pid; // Process ID
	pid = fork();
	if (!pid)
		sprintf(ptr,"%s", "Hello from your child!\n");
	else if (pid > 0) {
		wait(NULL);
		printf("%s", ptr);
		/* remove the shared memory segment */
		if (shm_unlink(name) == -1) {
			printf("Error removing %s\n",name);
			exit(-1);
		}
	}
	return 0;
}
