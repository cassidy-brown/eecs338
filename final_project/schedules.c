/* Cassidy Brown - cmb195
 * OS Final Project - 12/12/16
 * 
 *The scheduling method in use determines how tasks are prioritized.
 * The prioritization is implemented through how processes are sorted in the heap.
 * Therefore, each scheduling method boils down to a comparison function for the heaps.
 * And here we are.
 *
 * Comparison function: + -> a is "larger", has higher priority
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
	return b->startWaitTime - a->startWaitTime;
}

/* Shortest Job First 
 * the process with the shorter next burst takes priority 
 */
int compareSJF(process* a, process* b)
{
	return b->bursts[b->nextBurst] - a->bursts[a->nextBurst];
}	

/* Shortest Remaining Time First
 * Same as Shortest Job First, but with preempting, which is handled elsewhere
 */
int compareSRTF(process* a, process* b)
{
	return b->bursts[b->nextBurst] - a->bursts[a->nextBurst];
}

/* Round Robin
 * Preemptive (handled elsewhere). We want to go in the same order each time, like first come, first serve
 */
int compareRR(process* a, process* b)
{
	return b->startWaitTime - a->startWaitTime;
}

// for heap, via runSimulation
CompFcn getComparisonFunction(int schedule){
	switch(schedule){
		case FCFS:
			return compareFCFS;
		case SJF:
			return compareSJF;
		case SRTF:
			return compareSRTF;
		case RR:
			return compareRR;
	}

}