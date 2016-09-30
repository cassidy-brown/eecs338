// Chris Fietkiewicz (cxf47)
#include <stdio.h>
#include <stdlib.h> // Required for system()
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define MAX_LINE 80 /* 80 chars per line, per command */

char *read_line(void); // Prototype tells compiler how it will be used

int main(void)
{
    int should_run = 1; // Controls main loop
	pid_t pid; // Process ID
	char line[50]; // Buffer for storing command line
	char *args[MAX_LINE/2 + 1];	/* command line (of 80) has max of 40 arguments */
	int n_args = 0; // Number of arguments
		
    while (should_run){   
        printf("chris> ");
        fflush(stdout);
		gets(line);
		if (!strcmp(line, "exit")) // Returns difference in lexicographical order (0 means equal)
			should_run = 0;
		else {
			args[0] = strtok(line, " ");
			n_args = 0;
			while (args[n_args] != NULL)
				args[++n_args] = strtok(NULL, " ");
			pid = fork();
			if (pid == 0) // Child
				execvp(args[0], args);
			else if (pid > 0) // Parent
				if (!(strcmp(args[n_args-1], "&") == 0)) // If & is not present at the end...
					wait(NULL);            // ... wait for child to finish
		}
    }
	return 0;
}
