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
#include <sys/time.h>

#define N 1000000000

int main()
{
	const int SIZE = 2;
	const char *name = "ChrisF";
	int shm_fd;
	long long int *ptr;
	long int i;
	long long int x = 0;
	struct timeval start_time, stop_time, elapsed_time;  // timers

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

	gettimeofday(&start_time,NULL); // Unix timer
	pid_t pid1, pid2; // Process IDs
	pid1 = fork();
	if (!pid1) {
		for (i = 0; i < N; i++) {
			x = x + 1;
		}
		ptr[0] = x;
	}
	else if (pid1 > 0) {
		pid2 = fork();
		if (!pid2) {
			for (i = 0; i < N; i++) {
				x = x + 1;
			}
			ptr[1] = x;
		}
		else if (pid2 > 0) {
			int status1, status2;
			waitpid(pid1, &status1, 0); // Wait for process #1
			waitpid(pid2, &status2, 0); // Wait for process #2
			gettimeofday(&stop_time,NULL);
			timersub(&stop_time, &start_time, &elapsed_time); // Unix time subtract routine
			long long int sum = ptr[0] + ptr[1];
			printf("Total time was %f seconds. sum = %lld.\n", elapsed_time.tv_sec+elapsed_time.tv_usec/1000000.0, sum);
			/* remove the shared memory segment */
			if (shm_unlink(name) == -1) {
				printf("Error removing %s\n",name);
				exit(-1);
			}
		}
	}
	return 0;
}
