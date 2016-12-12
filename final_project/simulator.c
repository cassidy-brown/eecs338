/* Cassidy Brown - cmb195
 * OS Final Project - 12/12/16
 *
 * CPU simulator
 * See report for more information
 */

#include "expgen.c"
#include "heap.c"
#include "process.c"
#include "schedules.c"

process* allTheProcesses[NUM_PROCESSES];
int nextToInit;

heap *cpuHeap;
heap *ioHeap;
process *inCPU;
int cpuRunning; //boolean
int endInCPU;
int addedCPU; //boolean
process *inIO;
int ioRunning; //boolean
int endInIO;

process* procPtr;

void initialize(int seedSeed);
void cleanUp();
int runSimulation(int schedule);
void populateInitQueue();
void makeNewProcess(process *p, int seed, int seed2);
void prettyPrintProcess(process *ptr)


/* Main method
 * Runs each scheduling method 10 times, writing the results to files corresponding to the method name
 */
int main()
{
	int i, s;
	char* name;
	for(s = 1; s<=4; s++)
	{
		switch(s){
			case 1:
				name = "fcfs.txt";
				break;
			case 2:
				name = "sjf.txt";
				break;
			case 3:
				name = "srtf.txt";
				break;
			case 4:
				name = "rr.txt";
		}
		for(i = 0; i < 10; i++)
		{
			initialize(i);
			
			runSimulation(s);
			writeResults(name);

			cleanUp();
		}
	}

	return 0;
}

/* Memory allocation and stuff */
void initialize(int seedSeed)
{
	int i;
	for(i = 0; i < NUM_PROCESSES; i++){
		allTheProcesses[i] = malloc(sizeof(process));
	}
	nextToInit = 0;

	cpuHeap = malloc(sizeof(heap));
	ioHeap = malloc(sizeof(heap));
	inCPU = malloc(sizeof(process));
	inIO = malloc(sizeof(process));

	cpuRunning = 0;
	endInCPU = 0;
	addedCPU = 0; //boolean
	ioRunning = 0;
	endInIO = 0;

	procPtr = (process*)malloc(sizeof(process));

	//Initialize heaps 
	heap_init(cpuHeap);
	heap_init(ioHeap);
	printf("passed heap init\n");
	// initQ: Queue all "processes" that will be run
	int seed = (seedSeed * 13) + 18; 
	populateInitQueue(seed);

}

/* Memory deallocation */
void cleanUp()
{	
	int i;
	for(i = 0; i < NUM_PROCESSES; i++){
		free(allTheProcesses[i]);
	}

	free(cpuHeap);
	free(ioHeap);
}

/* The interesting thing in this file
 * This function simulates a CPU executing the processes in allTheProcesses
 * based on the specified scheduling method
 */
int runSimulation(int schedule){
	printf("~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~\n");
	printf("Running simulation with scheduling method %d", schedule);

	CompFcn comparison_function = getComparisonFunction(schedule);
	int time = 0;


	char* z = (char*)malloc(sizeof(char));
	scanf("%c",z);

	// RUN SIMULATOR
	// Inside big ole while loop with a counter (counter = clock):
	while(TRUE){
		printf("----------start of loop, time = %d----------\n", time);
		addedCPU = FALSE;

		// add items to cpuHeap from initQ
		if(nextToInit != NUM_PROCESSES)
		{
			procPtr = allTheProcesses[nextToInit];

			if(procPtr->initTime == time)
			{
				printf("Initialize process to cpuHeap: %d\n", procPtr->bursts[0]);
				heap_insert(cpuHeap, comparison_function, procPtr);
				nextToInit++;
				addedCPU = TRUE;
			}
		}

		// Check current cpu and io executions
		if (time == endInCPU)
		{
			if(cpuRunning)
			{
				// inCPU added to endQ since all bursts complete
				if(inCPU->nextBurst == BURSTS_PER_PROCESS - 1)
				{
					printf("!!!!!!!!!!PROCESS COMPLETE: %d\n", inCPU->initTime);
					inCPU->finishTime = time;
					// enqueue(endQ, inCPU);
				} 
				// inCPU added to ioHeap for next burst
				else 
				{	printf("Send to ioHeap: %d(%d: %d)\n", inCPU->initTime, inCPU->nextBurst, inCPU->bursts[inCPU->nextBurst]);
					printf("        %p\n", inCPU);
					inCPU->nextBurst++;
					heap_insert(ioHeap, comparison_function, inCPU);
				}
			}
			// get next CPU process from heap
			if(cpuHeap->size)
			{
				inCPU = heap_extract_max(cpuHeap, comparison_function);
				printf("Set new inCPU: %d(%d: %d)\n", inCPU->initTime, inCPU->nextBurst, inCPU->bursts[inCPU->nextBurst]);
				printf("    %p\n", inCPU);
				// Update process trackers
				if(!inCPU->nextBurst)	//if this is the first burst, record response time
				{
					inCPU->responseTime = time - inCPU->initTime;
				}
				inCPU->netWaitTime += time - inCPU->startWaitTime;
				// set new end time
				endInCPU = time + inCPU->bursts[inCPU->nextBurst];
				cpuRunning = TRUE;
			} else {
				printf("    cpu heap is currently empty\n");
				cpuRunning = FALSE;
				endInCPU++;
			}
		}
		if(time == endInIO)
		{
			if(ioRunning)
			{
				ioRunning = FALSE;
				// inIO added to cpuHeap (will never go straight to endQ)
				inIO->startWaitTime = time;
				inIO->nextBurst++;	
				heap_insert(cpuHeap, comparison_function, inIO);
				addedCPU = TRUE;
				printf("Send to cpuHeap: %d(%d: %d)\n", inIO->initTime, inIO->nextBurst, inIO->bursts[inIO->nextBurst]);
				printf("    %p\n", inIO);
			}
			// get next IO process from heap
			if(ioHeap->size)
			{
				inIO = heap_extract_max(ioHeap, comparison_function);
				printf("Set new inIO  %d(%d: %d)\n", inIO->initTime, inIO->nextBurst, inIO->bursts[inIO->nextBurst]);
				printf("    %p\n", inIO);
				endInIO = time + inIO->bursts[inIO->nextBurst];
				ioRunning = TRUE;
			}
			else
			{
				printf("    ioHeap is currently empty\n");
				ioRunning = FALSE;
				endInIO++;
			}
		}
		// If we're using a preemptive algorithm, check that things are groovy
		if(schedule == SRTF)
		{
			printf("SRTF preempt\n");
			if(addedCPU)
			{	
				printf(": addedCPU, %p(%d)", inCPU, endInCPU-time);
				//put the current inCPU back into the heap
				inCPU->startWaitTime = time;
				inCPU->bursts[inCPU->nextBurst] = endInCPU - time;
				heap_insert(cpuHeap, comparison_function, inCPU);

				// Extract the current smallest from the heap
				inCPU = heap_extract_max(cpuHeap, comparison_function);
				// Update process trackers
				inCPU->netWaitTime += time - inCPU->startWaitTime;
				// set new end time
				endInCPU = time + inCPU->bursts[inCPU->nextBurst];
				printf("->%p(%d)", inCPU, inCPU->bursts[inCPU->nextBurst]);

			}
			// scanf("%c",z);
		}
		if(schedule == RR)
		{
			if(!time%4 && cpuRunning)		//arbitrarily picked 4-tick round robin cycles
			{
				//put the current inCPU back into the heap
				inCPU->startWaitTime = time;
				inCPU->bursts[inCPU->nextBurst] = endInCPU - time;
				heap_insert(cpuHeap, comparison_function, inCPU);

				// Extract the current smallest from the heap
				inCPU = heap_extract_max(cpuHeap, comparison_function);
				// Update process trackers
				inCPU->netWaitTime += time - inCPU->startWaitTime;
				// set new end time
				endInCPU = time + inCPU->bursts[inCPU->nextBurst];

			}
		}
		// exit condition: when all processes are done. initQ and the heaps are empty, and inCPU and inIO are null
		if(!cpuRunning && !cpuHeap->size && !ioRunning && !ioHeap->size && nextToInit==NUM_PROCESSES)
		{
			printf("<*><*><*><*><*>We're done<*><*><*><*><*>\n");
			break;
		}
		else
		{
			printf("            --------------------\n    cpuRunning = %d; cpuHeap->size = %d;\n    ioRunning = %d; ioHeap->size = %d;\n    nextToInit = %d;\n",
				cpuRunning, cpuHeap->size, ioRunning, ioHeap->size, nextToInit);
		}

		// if(!(time % 20)) scanf("%c", z);
		time++;

	}

	return 0;
}

/* Writes information from allTheProcesses to the specified file
 * Information is appended, so mind you don't run main repeatedly, or you'll get duplicate information
 */
int writeResults(char* filename){
	FILE *filePtr = fopen(filename,"a");

	int i;
	process* p;
	char* str;
	for(i = 0; i < NUM_PROCESSES; i++){
		p = allTheProcesses[i];
		fprintf(filePtr, "%d\t%d\t%d\t%d\t%d\n", (p->finishTime - p->initTime), p->responseTime, p->netWaitTime, p->netCpuTime, p->netIoTime);
	}

	fclose(filePtr);
	return 0;
}

/* Adds process information to allTheProcesses */
void populateInitQueue(int seed){
	int i;
	for(i=1; i<=NUM_PROCESSES; i++)
	{	
		makeNewProcess(allTheProcesses[i - 1], i, seed);
	}
	printf("------------------------------------------------------------------------\n");
}

/* Sets up a process
 * Using seeds, generates pseudo-random numbers to populate burst array and sets other fields to 0 
 */
void makeNewProcess(process *p, int seed, int seed2){
	int i;
	int initTime;
	rand_val(seed * seed2);
	rand_val(0);
	initTime = (int)((rand_val(0) + seed - 1) * 20);
	p->initTime = initTime;

	p->finishTime = 0;

	int length = BURSTS_PER_PROCESS;
	int burstArr[length];
	populateArrayWithExpDist(&burstArr, seed, 1, length);
	for(i=0; i < BURSTS_PER_PROCESS; i++)
	{
		p->bursts[i] = burstArr[i];
	}
	p->nextBurst = 0;

	p->responseTime = 0;
	p->startWaitTime = 0;
	p->netWaitTime = 0;

	int ioTime = 0, cpuTime = 0;
	for(i = 0; i < length; i++)
	{
		if(i % 2)	//i % 2 == 1 --> odd --> io 
			ioTime += burstArr[i];
		else		
			cpuTime += burstArr[i];	
	}

	p->netCpuTime = cpuTime;
	p->netIoTime = ioTime;	
}

/* Prints information about a process to the terminal */
void prettyPrintProcess(process *ptr){
	int i;
	process p = *ptr;
	printf("----------Printing Process----------\n");
	printf("initTime = %d\nendTime = %d\nresponseTime = %d\nnetWaitTime = %d\nnetCpuTime = %d\nnetIoTime = %d\nbursts = ",
		p.initTime, p.finishTime, p.responseTime, p.netWaitTime, p.netCpuTime, p.netIoTime);

	for(i = 0; i < BURSTS_PER_PROCESS; i++)
	{
		printf("%d, ", p.bursts[i]);
	}

	printf("\n");
}
