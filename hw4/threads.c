/* Cassidy Brown - cmb195
 * OS HW4 - 10/3/16
 * Multithreading method
 * Adapted from Gagne, Galvin, Silberschatz example
 */

#include <pthread.h>
#include <stdio.h>
#include <math.h>

double child_min; /* this data is shared by the thread(s) */

void *runner(void *param); /* the thread */
double computations(double x1Start, double x1End);
double shubert(double x1, double x2);

int main(int argc, char *argv[])
{
    pthread_t tid; /* the thread identifier */
    pthread_attr_t attr; /* set of attributes for the thread */

    double min;

    /* get the default attributes */
    pthread_attr_init(&attr);

    /* create the thread */
    pthread_create(&tid,&attr,runner,argv[1]);

    /* now wait for the thread to exit */
    pthread_join(tid,NULL);

    /* Do half the calculations */
    min = computations(-2.0, 0.0);

    /* Compare the mins from the two threads */
    if(child_min < min){
        min = child_min;
    }

    /* Print the net minimum */
    printf("min = %.2f\n", min);
}

/**
 * The thread will begin control in this function
 */
void *runner(void *param) 
{
    /* Do half the calculations */
    child_min = computations(0.5, 2.0);

	pthread_exit(0);
}

/* The double loop gets its own function since it's used twice */
double computations(double x1Start, double x1End){
	double x1, x2, y, min = 0;

	for (x1 = x1Start; x1 <= x1End; x1 += 0.5) {
        for (x2 = -2; x2 <= 2; x2 += 0.5) {
            y = shubert(x1, x2);
            printf("%.2f ", y);
            if (y < min)
                min = y;
        }
        printf("\n");
    }
    return min;
}

/* Shubert function, as provided */
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
