// Chris Fietkiewicz (chris.fietkiewicz@case.edu)
#include <stdio.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <unistd.h>

#define N 1000000000

int main() {
	int nProc; // Number of processes for computation
	printf("How many processes? ");
	scanf("%d", &nProc);

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
	gettimeofday(&stop_time,NULL);
	timersub(&stop_time, &start_time, &elapsed_time); // Unix time subtract routine
	wait(NULL);
	printf("Process %d total time was %f seconds. sum = %lld.\n", iProc, elapsed_time.tv_sec+elapsed_time.tv_usec/1000000.0, sum);
}
