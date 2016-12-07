// Chris Fietkiewicz
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <omp.h> 
#include <sys/time.h>

double shubert(double x1, double x2);

int main() {
  double x_start = -2.0, x_stop = 2.0, y, min = 0;
  int i1 = 0, i2 = 0; // Iterators
  int n_steps = 5000;
  double step = (x_stop - x_start) / n_steps;
  struct timeval start_time, stop_time, elapsed_time;  // timers
  gettimeofday(&start_time,NULL); // Unix timer

  #pragma omp parallel for reduction(min:min)
  for (i1 = 0; i1 < n_steps; i1++) {
	double x1 = (double) i1 * step + x_start;
	for (i2 = 0; i2 < n_steps; i2++) {
	  double x2 = (double) i2 * step + x_start;
      y = shubert(x1, x2);
      if (y < min)
        min = y;
    }
  }
  gettimeofday(&stop_time,NULL);
  timersub(&stop_time, &start_time, &elapsed_time); // Unix time subtract routine
  printf("Total time was %f seconds.\n", elapsed_time.tv_sec+elapsed_time.tv_usec/1000000.0);
  printf("min = %.2f\n", min);
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