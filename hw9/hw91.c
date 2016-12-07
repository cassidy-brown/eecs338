// Chris Fietkiewicz (chris.fietkiewicz@case.edu)
// This is a revised version of HW #3, Problem #2.
#include <stdio.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h> // For atoi()

#define N 1000000000

int main() {
	int nProc = 12; // Number of processes for computation
	int iProc = 1; // Index of process
	int nextProc; // Iterator for process creation
	long int i; // Summation iterator
	long long int sum = 0;
	pid_t pid = 0;
	struct timeval start_time, stop_time, elapsed_time;  // timers
	gettimeofday(&start_time,NULL); // Unix timer
	for (nextProc = 2; nextProc <= nProc & !pid; nextProc++) {
		pid = fork();
		if (!pid)
			iProc = nextProc;
	}
	for (i = 0; i < N/nProc; i++) {
		sum = sum + 1;
	}
	printf("Process %d sum = %lld.\n", iProc, sum);
	// Wait for all children to finish
	for (nextProc = 1; nextProc < nProc; nextProc++)
		wait(NULL);
	// 1st process displays the total time
	if (iProc == 1) {
		gettimeofday(&stop_time,NULL);
		timersub(&stop_time, &start_time, &elapsed_time); // Unix time subtract routine
		printf("Total time was %f seconds.\n", elapsed_time.tv_sec+elapsed_time.tv_usec/1000000.0);
	}
}
