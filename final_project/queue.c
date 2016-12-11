/* A queue implementation from Rick Coleman at University of Alabama in Huntsville
 * http://www.cs.uah.edu/~rcoleman/Common/CodeVault/Code/Code130_Queue.html */
//---------------------------------------------------------------
// File:queue.c
// Purpose: Implementation file for a demonstration of a queue  
//		implemented as an array.
// Programming Language: C
// Author: Dr. Rick Coleman
// Date: February 11, 2002
// Adapted to store process structs
//---------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include "process.c"

#define MAX_SIZE    11        // Define maximum length of the queue

typedef struct {
	int head;
	int tail;
	process **theQueue;
} queue;

// List Function Prototypes
void initQueue(queue *q);             // Initialize the queue
void clearQueue(queue *q);            // Remove all items from the queue
int enqueue(queue *q, process* p);    // Enter an item in the queue
void dequeue(queue *q, process* p);   // Remove an item from the queue
void peek(queue *q, process* p);      // View the next item in the queue
int isEmpty(queue *q);                // Return true if queue is empty
int isFull(queue *q);                 // Return true if queue is full


// Declare these as static so no code outside of this source
// can access them.

//--------------------------------------------
// Function: InitQueue()
// Purpose: Initialize queue to empty.
// Returns: void
//--------------------------------------------
void initQueue(queue* q)
{
	q->head = q->tail = -1;
	q->theQueue = (process**)malloc(sizeof(process*) * MAX_SIZE);
	int i;
	for(i = 0; i<MAX_SIZE; i++){
		(q->theQueue)[i] = (process*)malloc(sizeof(process));
	}
}

//--------------------------------------------
// Function: ClearQueue()
// Purpose: Remove all items from the queue
// Returns: void
//--------------------------------------------
void clearQueue(queue *q)
{
	q->head = q->tail = -1; // Reset indices to start over
	int i;
	for(i = 0; i<MAX_SIZE; i++){
		free((q->theQueue)[i]);
	}
	free(q->theQueue);
}

//--------------------------------------------
// Function: Enqueue()
// Purpose: Enqueue an item into the queue.
// Returns: TRUE if enqueue was successful
//		or FALSE if the enqueue failed.
// Note: We let head and tail continuing 
//		increasing and use [head % MAX_SIZE] 
//		and [tail % MAX_SIZE] to get the real
//		indices.  This automatically handles
//		wrap-around when the end of the array
//		is reached.
//--------------------------------------------
int enqueue(queue *q, process* p)
{
	// Check to see if the Queue is full
	if(isFull(q)) return FALSE;

	// Increment tail index
	q->tail++;
	// Add the item to the Queue
	q->theQueue[q->tail % MAX_SIZE] = p;
	return TRUE;
}

//--------------------------------------------
// Function: Dequeue()
// Purpose: Dequeue an item from the Queue.
// Returns: The process at head, or NULL if
//			the queue is empty
//--------------------------------------------
void dequeue(queue *q, process* p)
{

	// Check for empty Queue
	if(isEmpty(q)) p = 0;  // Return null if queue is empty
	else
	{
		p = q->theQueue[q->head % MAX_SIZE];		// Get process to return
		q->head = q->head + 1;
	}
}

//--------------------------------------------
// Function: peek()
// Purpose: Look at the next item in the queue
// Returns: The process at head, or NULL if
//			the queue is empty
//--------------------------------------------
void peek(queue *q, process *p)
{
	// Check for empty Queue
	if(isEmpty(q)){
		p =0;  // Return null if queue is empty
	}
	else
	{
		p = q->theQueue[q->head % MAX_SIZE];		// Get process to return
	}
}

//--------------------------------------------
// Function: isEmpty()
// Purpose: Return true if the queue is empty
// Returns: TRUE if empty, otherwise FALSE
// Note: C has no boolean data type so we use
//	the defined int values for TRUE and FALSE
//	instead.
//--------------------------------------------
int isEmpty(queue *q)
{
	fflush(stdout);
	return (q->head == q->tail);
}

//--------------------------------------------
// Function: isFull()
// Purpose: Return true if the queue is full.
// Returns: TRUE if full, otherwise FALSE
// Note: C has no boolean data type so we use
//	the defined int values for TRUE and FALSE
//	instead.
//--------------------------------------------
int isFull(queue *q)
{
	// Queue is full if tail has wrapped around
	//	to location of the head.  See note in
	//	Enqueue() function.
	return ((q->tail - MAX_SIZE) == q->head);
}
