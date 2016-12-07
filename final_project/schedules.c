/* The scheduling method in use determines how tasks are prioritized.
 * The prioritization is implemented through how processes are sorted in the heap.
 * Therefore, each scheduling method boils down to a comparison function for the heaps.
 * And here we are.
 */

#include <schedule.c>	//we need the process struct, but this seems like a bad, cyclic idea

#define FCFS 1		// First Come, First Served
#define SJF  2		// Shortest Job First
#define SRTF 3		// Shortest Remaining Time First
#define RR   4		// Round Robin


/* First Come, First Serve
 * the process with the earlier startWaitTime has priority
 */
int compareFCFS(process* a, process* b)
{
	return a->startWaitTime - b->startWaitTime;
}

/* Shortest Job First 
 * the process with the shorter next burst takes priority 
 */
int compareSJF(process* a, process* b)
{
	return a->bursts[a->nextBurst] - b->bursts[b->nextBurst];
}	

/* Shortest Remaining Time First
 * This one involves preempting, which I haven't considered yet... */
int compareSRTF(process* a, process* b)
{
	return 0;
}

/* Round Robin
 * Also involves preempting
 */
int compareRR(process* a, process* b)
{
	return 0;
}




typedef struct{
	int initTime;						// when process will be added to the CPU queue
	int finishTime;						// when the process is done executing
	int bursts[BURSTS_PER_PROCESS];		// array representing burst lengths. Even indices (starting at 0) are CPU bursts, odd are IO
	int nextBurst;						// next number to access in the array
	int startWaitTime;					// when the process most recently entered the cpuHeap
	int netWaitTime;					// how long the process spent in cpuHeap total
	int netCpuTime						// sum of bursts' even indices 		
} process;