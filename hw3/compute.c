/**
 * Cassidy Brown - cmb195
 * HW3, problem 2
 * 
 * Adapted code from lecture example */
#include <stdio.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <unistd.h>

#define N 2000000000

void forked(int proc_num, int count){
	int i;
	long long int x = 0;
	struct timeval start_time, stop_time, elapsed_time;  // timers 		//what is this?

	gettimeofday(&start_time,NULL); // Unix timer
	for (i = 0; i < count; i++) {
		x = x + 1;
	}
	gettimeofday(&stop_time,NULL);
	timersub(&stop_time, &start_time, &elapsed_time); // Unix time subtract routine
	printf("Process %d: %f seconds. x = %lld.\n", proc_num, elapsed_time.tv_sec+elapsed_time.tv_usec/1000000.0, x);
}

int main() {
	int i, num, count;
	int parent_id = getpid();
	pid_t pid;
	struct timeval start_time, stop_time, elapsed_time;  // timers

	printf("Number of processes? ");
	scanf("%d", &num);

	count = N / num;

	for(i = 1; i <= num; i++){
		pid = fork();
		if(pid == 0){
			forked(i, count);
			return 0;
		}
	}
	wait(NULL);
	return 0;
}

