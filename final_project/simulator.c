#include "expgen.c"
#include "heap.c"
#include "queue.c"
#include "process.c"
#include "schedules.c"


queue *initQ;
queue *endQ;

heap *cpuHeap;
heap *ioHeap;
process *inCPU;
int endInCPU;
int addedCPU; //boolean
process *inIO;
int endInIO;

int runSimulation(int schedule);
void populateInitQueue(queue *q);
void makeNewProcess(process *p, int seed);
void printProcess(process* ptr);

int main(){

	initQ = malloc(sizeof(queue));
	endQ = malloc(sizeof(queue));
	cpuHeap = malloc(sizeof(heap));
	ioHeap = malloc(sizeof(heap));
	inCPU = malloc(sizeof(process));
	inIO = malloc(sizeof(process));
	
	runSimulation(FCFS);
	writeResults();

	clearQueue(initQ);
	free(initQ);
	clearQueue(endQ);
	free(endQ);
	free(cpuHeap);
	free(ioHeap);
	free(inCPU);
	free(inIO);

	return 0;
}

int runSimulation(int schedule){
	process* procPtr;
	procPtr = (process*)malloc(sizeof(process));
	process p;

	CompFcn comparison_function = getComparisonFunction(schedule);
	printf("passed getComparisonFunction\n");
	int time = 0;

	//Initialize queues
	initQueue(initQ);
	initQueue(endQ);
	printf("passed q init\n");
	//Initialize heaps 
	heap_init(cpuHeap);
	heap_init(ioHeap);
	printf("passed heap init\n");
	// initQ: Queue all "processes" that will be run
	populateInitQueue(initQ);
	printf("passed initQ population\n");
	return;

	peek(initQ, procPtr);
	printProcess(procPtr);
	int* z = (int*)malloc(sizeof(int));
	scanf("%d",z);

	// RUN SIMULATOR
	// Inside big ole while loop with a counter (counter = clock):
	while(TRUE){
		printf("start of loop, time = %d\n", time);
		scanf("%d", z);
		addedCPU = FALSE;

		// add items to cpuHeap from initQ
		if(!isEmpty(initQ))
		{
			printf("inside initQ not empty\n");
			peek(initQ, procPtr);
			if(time == 32 || time == 1){
				printf("queue head = %d", initQ->head);
				printProcess(procPtr);
			}

			if(procPtr->initTime == time)
			{
				printf("initTime matches\n");
				scanf("%d",z);
				fflush(stdout);
				dequeue(initQ, procPtr);
				heap_insert(cpuHeap, comparison_function, procPtr);
				addedCPU = TRUE;
			}
			printf("after it all\n");
			fflush(stdout);
		}

		printf("after add to cpuHeap\n");
		fflush(stdout);
		// Check current cpu and io executions
		if (time == endInCPU)
		{
			if(inCPU)
			{
				// inCPU added to endQ since all bursts complete
				if(inCPU->nextBurst == BURSTS_PER_PROCESS)
				{
					inCPU->finishTime = time;
					enqueue(endQ, inCPU);
				} 
				// inCPU added to ioHeap for next burst
				else 
				{
					inCPU->nextBurst++;
					heap_insert(ioHeap, comparison_function, inCPU);
				}
			}
			// get next CPU process from heap
			if(cpuHeap->size)
			{
				inCPU = heap_extract_max(cpuHeap, comparison_function);
				// Update process trackers
				inCPU->netWaitTime += time - inCPU->startWaitTime;
				// set new end time
				endInCPU = time + inCPU->bursts[inCPU->nextBurst];
			} else {
				inCPU = 0;
				endInCPU++;
			}
		}
		printf("after change out cpu\n");
		if(time == endInIO)
		{
			if(inIO)
			{
				// inIO added to cpuHeap (will never go straight to endQ)
				inIO->startWaitTime = time;
				heap_insert(cpuHeap, comparison_function, inIO);
				addedCPU = TRUE;
			}
			// get next IO process from heap
			if(ioHeap->size)
			{
				inIO = heap_extract_max(ioHeap, comparison_function);
				endInIO = time + inIO->bursts[inIO->nextBurst];
				inIO->nextBurst++;		//ahhh pointers are okay with this, right?
			}
			else
			{
				inIO = 0;
				endInIO++;
			}
		}
		printf("after change out io\n");
		// If we're using a preemptive algorithm, check that things are groovy
		if(schedule == SRTF)
		{
			if(addedCPU)
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
		printf("after preemptive check\n");
		// exit condition: when all processes are done. initQ and the heaps are empty, and inCPU and inIO are null
		if(!inCPU && !cpuHeap->size && !inIO && !ioHeap->size && isEmpty(initQ))
		{
			break;
		}

		printf("after end condition check\n");
		time++;

	}

	return 0;
}

int writeResults(){
	int i;
	for(i = 0; i < NUM_PROCESSES; i++){
		printf("Process %d\n", i);
		process *p = malloc(sizeof(process));
		dequeue(initQ, p);
		printProcess(p);
		printf("\n");
		free(p);
	}

	return 0;
}


void populateInitQueue(queue *q){
	int i;
	for(i=1; i<=NUM_PROCESSES; i++)
	{	// we shouldn't let the rand seed be 0
		process* p = malloc(sizeof(process));
		makeNewProcess(p, i);
		enqueue(q, p);
	}
	printf("flush\n");
	fflush(stdout);
	writeResults();
	printf("------------------------------------------------------------------------\n");
}

void makeNewProcess(process *p, int seed){
	int i;
	int initTime;
	rand_val(seed * 200);
	rand_val(0);
	initTime = (int)((rand_val(0) + seed - 1) * 20);
	p->initTime = initTime;
	printf("seed= %d; initTime = %d\n", seed, p->initTime);

	p->finishTime = 0;

	int length = BURSTS_PER_PROCESS;
	int burstArr[length];
	populateArrayWithExpDist(&burstArr, seed, 1, length);
	for(i=0; i < BURSTS_PER_PROCESS; i++)
	{
		p->bursts[i] = burstArr[i];
	}
	p->nextBurst = 0;

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

void printProcess(process* ptr){
	int i;
	process p = *ptr;
	printf("----------Printing Process----------\n");
	printf("initTime = %d\nendTime = %d\nnextBurst = %d\nstartWaitTime = %d\nnetWaitTime = %d\nnetCpuTime = %d\nnetIoTime = %d\nbursts = ",
		p.initTime, p.finishTime, p.nextBurst, p.startWaitTime, p.netWaitTime, p.netCpuTime, p.netIoTime);

	for(i = 0; i < BURSTS_PER_PROCESS; i++)
	{
		printf("%d, ", p.bursts[i]);
	}
}