/* The scheduling method in use determines how tasks are prioritized.
 * The prioritization is implemented through how processes are sorted in the heap.
 * Therefore, each scheduling method boils down to a comparison function for the heaps.
 * And here we are.
 */

#include "process.c"

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
 * Same as Shortest Job First, but with preempting, which is handled elsewhere
 */
int compareSRTF(process* a, process* b)
{
	return a->bursts[a->nextBurst] - b->bursts[b->nextBurst];
}

/* Round Robin
 * Also involves preempting
 * It also may not involve a heap
 */
int compareRR(process* a, process* b)
{
	return 0;
}


CompFcn getComparisonFunction(int schedule){
	switch(schedule){
		case 1:
			return compareFCFS;
		case 2:
			return compareSJF;
		case 3:
			return compareSRTF;
		case 4:
			return compareRR;
	}

}