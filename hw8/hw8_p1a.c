/* Cassidy Brown - cmb195
 * OS - HW 8
 * Problem 1a: Testing global counters with OpenMP's critical pragma
 */

#include <stdio.h>
#include <omp.h> 
#define MAX_THREADS 8

int main () { 
	int test; 
	for (test = 1; test <= 100; test++) {
		int i, count = 0;
		#pragma omp parallel for
		for (i = 1; i <= 10000; i++) {
			if(omp_get_thread_num() % 2){ //odd -- i % 2 == 1
				#pragma omp critical
				count++;
			} else {	//even
				#pragma omp critical
				count--;
			}
		} /* All threads join master thread and terminate */ 

		// Print this run's count and a new line, if appropriate
		printf("%d ", count);
		if(!(test % 10))
			printf("\n");
	}
	return 0;
} 