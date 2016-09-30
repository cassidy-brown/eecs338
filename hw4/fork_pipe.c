/* Cassidy Brown - cmb195
 * OS HW4 - 10/3/16
 *
 * Fork and pipe method
 */

# include <stdlib.h>
# include <stdio.h>
# include <math.h>
# include <string.h>

#define BUFFER_SIZE 10
#define READ_END    0
#define WRITE_END 1

double shubert(double x1, double x2);

int main (){
    pid_t pid;
    char write_msg[BUFFER_SIZE];
    char read_msg[BUFFER_SIZE];
    int fd[2];
    double x1, x2, y, min = 0;
    double x1Start, x1Max = 0;
    double childMin;


    /* create the pipe */
    if (pipe(fd) == -1) {
        fprintf(stderr,"Pipe failed");
        return 1;
    }

    /* Fork the process */
    pid = fork();

    /* Set starting values--each does half the x1 values */
    if(!pid){ // Child
        x1Start = 0.5;
        x1Max = 2.0;

    } else { // Parent
        x1Start = -2.0;
        x1Max = 0.0;
    }

    // Both processes execute the computation
    for (x1 = x1Start; x1 <= x1Max; x1 += 0.5) {
        for (x2 = -2; x2 <= 2; x2 += 0.5) {
            y = shubert(x1, x2);
            printf("%.2f ", y);
            if (y < min)
                min = y;
        }
        printf("\n");
    }

    /* Deal with pipe communication once mins are found */
    if(!pid){ // child
        /* close the unused end of the pipe */
        close(fd[READ_END]);

        /* convert the min to a string for ease of piping */
        sprintf(write_msg, "%f", min);

        /* write to the pipe */
        write(fd[WRITE_END], write_msg, strlen(write_msg)+1); 

        /* close the write end of the pipe */
        close(fd[WRITE_END]);

    } else { // Parent
        /* Wait for the child to finish doing its thing */
        wait(NULL);

        /* close the unused end of the pipe */
        close(fd[WRITE_END]);

        /* read from the pipe */
        read(fd[READ_END], read_msg, BUFFER_SIZE);

        /* convert the value back to a double */
        childMin = atof(read_msg);

        /* close the write end of the pipe */
        close(fd[READ_END]);

        if(childMin < min){
            // set the minimum to be that found by the child, if appropriate
            min = childMin;
        }

        /* print the net minimum */
        printf("min = %.2f\n", min);
    }

    return 0;
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