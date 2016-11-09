/* Cassidy Brown - cmb195
 * OS - HW 8
 * Problem 1b: Testing global counters without OpenMP's critical pragma
 */

#include <stdio.h>
#include <omp.h> 
#define MAX_THREADS 8

int main () { 
	int trial; 
	for (trial = 1; trial <= 100; trial++) {
		int i, count = 0;
		#pragma omp parallel for
		for (i = 1; i <= 10000; i++) {
			if(omp_get_thread_num() % 2){ //odd -- i % 2 == 1
				count++;
			} else {	//even
				count--;
			}
		} /* All threads join master thread and terminate */ 
		printf("%d ", count);
		if(!(trial % 10))
			printf("\n");
	}
	printf("\n");
	return 0;
} 