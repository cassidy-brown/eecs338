/* Cassidy Brown - cmb195
 * OS - HW 5
 * Problem 1: Shubert via OpenMP and reductions
 */

#include <stdio.h>
#include <omp.h> 
#include <math.h>
#define MAX_THREADS 8

double shubert(double x1, double x2);

int main () {
	double x1, x2, y, min = 0;
	int index1 = 0;
	#pragma omp parallel for private(y) reduction(min: min)
    for (index1 = 0; index1 <= 8; index1++) {
    	// omp for must increment with an int, 
    	//so translate it to the corresponding double for the shubert function
    	x1 = (0.5 * index1) - 2;
    	// Run inner loop, calling shubert() and storing the minimum of the loop
        for (x2 = -2; x2 <= 2; x2 += 0.5) {
            y = shubert(x1, x2);
            printf("%.2f ", y);
            if (y < min)
                min = y;
        }
        printf("\n");
    }
    printf("min = %.2f\n", min);	
	return 0;
} 

/* Shubert function, as provided in HW4 */
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