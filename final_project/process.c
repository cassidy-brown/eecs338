#ifndef PROCESS_FLAG
#define PROCESS_FLAG

// Define TRUE and FALSE if they have not already been defined
#ifndef FALSE
#define FALSE (0)
#endif
#ifndef TRUE
#define TRUE (!FALSE)
#endif

#define BURSTS_PER_PROCESS 11		// For the sake of simplicity, we'll make them all the same. For now...
									// I don't want to deal with full-on pointers, so I'm going to use arrays as a crutch
#define NUM_PROCESSES 10

/* This struct represents a process that needs to be executed
 * From the processing perspective, the most important part is the array of burst times
 * the rest of the information helps track it through all the data structures */
// typedef struct proc_def process;

typedef struct {
	int initTime;						// when process will be added to the CPU queue
	int finishTime;						// when the process is done executing
	int bursts[BURSTS_PER_PROCESS];		// array representing burst lengths. Even indices (starting at 0) are CPU bursts, odd are IO
	int nextBurst;						// next number to access in the array
	int startWaitTime;					// when the process most recently entered the cpuHeap
	int netWaitTime;					// how long the process spent in cpuHeap total
	int netCpuTime;						// sum of bursts' even indices 		
	int netIoTime;						// sun of bursts' odd indices
} process;

typedef int (*CompFcn)(process*, process*);	

#endif