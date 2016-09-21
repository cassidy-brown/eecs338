/**
 * Simple shell interface program.
 *
 * Operating System Concepts - Ninth Edition
 * Copyright John Wiley & Sons - 2013
 */

#include <stdio.h>
#include <unistd.h>
#include <string.h>



#define MAX_LINE		80 /* 80 chars per line, per command */

int main(void)
{
	char *args[MAX_LINE/2 + 1];	/* command line (of 80) has max of 40 arguments */
    int should_run = 1;
    char input[MAX_LINE];
	
	int i, upper;
    pid_t pid;
		
    while (should_run){   
        printf("cmb>");
        fflush(stdout);

        // Get user's input
        gets(input);

        // Split input and read it into args
        i = 0;
        args[0] = strtok(input, " ");
        while (args[i] != NULL){
            args[++i] = strtok(NULL, " ");
        }

        // check that something was input
        // if not, start over
        // this prevents us from trying to reach unaccessible data
        if(args[0]==NULL){
            continue;
        }
        // check for exit command
        if(!strcmp(args[0], "exit")){
            return 0;
        }

        //Fork the process
        pid = fork();
        if(pid == 0){
            // The child executes the command
            execvp(args[0], args);
            printf("done executing!");
        }
        else{
            //The parent (effectively, the prompt) waits for the child to finish
            // before letting the user call something else in the shell
            if(strcmp(args[i - 1], "&")){
                wait(NULL);
                printf("done waiting!");
            }

        }
        
        /**
         * After reading user input, the steps are:
         * (1) fork a child process
         * (2) the child process will invoke execvp()
         * (3) if command included &, parent will invoke wait()
         */
    }
    
	return 0;
}

