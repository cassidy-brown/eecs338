#include <heap.c>
#include <simulator.c>

#define BURSTS_PER_PROCESS 11		// For the sake of simplicity, we'll make them all the same. For now...
									// I don't want to deal with full-on pointers, so I'm going to use arrays as a crutch

/* This struct represents a process that needs to be executed
 * From the processing perspective, the most important part is the array of burst times
 * the rest of the information helps track it through all the data structures */
typedef struct{
	int initTime;						// when process will be added to the CPU queue
	int finishTime;						// when the process is done executing
	int bursts[BURSTS_PER_PROCESS];		// array representing burst lengths. Even indices (starting at 0) are CPU bursts, odd are IO
	int nextBurst;						// next number to access in the array
	int startWaitTime;					// when the process most recently entered the cpuHeap
	int netWaitTime;					// how long the process spent in cpuHeap total
	int netCpuTime						// sum of bursts' even indices 		
} process;

process *initQ;			//queue, array, same difference
int currentProcess;
process *endQ;

heap *cpuHeap;
heap *ioHeap;
process *inCPU;
int endInCPU;
process *inIO;
int endInIO;


int main(){

	int time = 0;

	//Initialize heaps 
	heap_init(cpuHeap);
	heap_init(ioHeap);
	//get their comparison function from somewhere

	// initQ: Queue all "processes" that will be run

	// RUN SIMULATOR
	// Inside big ole while loop with a counter (counter = clock):
		// add items to cpuHeap from initQ

		// Check current cpu and io executions
		if (time == endInCPU)
		{
			// inCPU added to ioHeap (or endQ if all bursts complete)
			if(inCPU->nextBurst == BURSTS_PER_PROCESS)
			{
				inCPU->finishTime = time;
				//add to endQ
			} 
			else 
			{
				heap_insert(ioHeap)
			}
			// get next CPU process from heap
			inCPU = heap_extract_max(cpuHeap);
			// Update process trackers
			inCPU->netWaitTime += time - inIO->startWaitTime;
			inCPU->netCpuTime += inCPU->bursts[nextBurst];
			// set new end time
			endInCPU = time + inCPU->bursts[nextBurst];
			inCPU->nextBurst++;
		}

		if(time == endInIO)
		{
			// inIO added to cpuHeap (will never go straight to endQ)
			heap_insert(cpuHeap, COMPARISON_FUNCTION, inIO);
			// get next IO process from heap
			// TODO: what if the heap is empty?
			inIO = heap_extract_max(ioHeap);
			endInIO = time + inIO->bursts[inIO->nextBurst];
			inIO->nextBurst++;		//ahhh pointers are okay with this, right?
		}
		time++;
		// exit condition: when all processes are done. initQ and the heaps are empty, and inCPU and inIO are null

	// Outside the loop: Data organization
		// Figure out throughput, CPU utilization, turnaround, waiting time, response time
		// save to tab-separated file for outside analysis



	return 0;
}