/* Cassidy Brown - cmb195
 * OS - HW 5
 * Problem 2: Shubert via OpenMP, without reductions
 */

#include <stdio.h>
#include <omp.h> 
#include <math.h>
#define MAX_THREADS 4

double shubert(double x1, double x2);

int main () {
	int tid;
	double x1, x2, y, net_min = 0;
	// Array to store minimum values calculated on each thread
	double min[MAX_THREADS] = {0, 0, 0, 0};
	int index1 = 0;
	#pragma omp parallel for private(y)
    for (index1 = 0; index1 <= 8; index1++) {
    	// Get the thread number
    	tid = omp_get_thread_num();
    	// Translate index (required by omp to be int) to double for Shubert calculations 
    	x1 = (0.5 * index1) - 2;
    	// Inner loop, calling shubert and storing a minimum value
        for (x2 = -2; x2 <= 2; x2 += 0.5) {
            y = shubert(x1, x2);
            printf("%.2f ", y);
            if (y < min[tid])
                min[tid] = y;
        }
        printf("\n");
    }

    /* Examine the stored mins to find the net lowest calculated value */
    net_min = min[0];
    printf("0: %.2f\n", min[0]);
    for(index1 = 1; index1 < MAX_THREADS; index1++){
    	printf("%d: %.2f\n", index1, min[index1]);
    	if(min[index1] < net_min){
    		net_min = min[index1];
    	}
    }
    printf("min = %.2f\n", net_min);	
	return 0;
} 

double shubert(double x1, double x2) {
	double sum1 = 0; 
	double sum2 = 0;
	int i;
	for (i = 1; i <= 5; i++) {
		sum1 += i * cos((i + 1) * x1 + i);
		sum2 += i * cos((i + 1) * x2 + i);
	}
	return sum1 * sum2;
}