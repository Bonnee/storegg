#include <sys/wait.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "msg_queue.h"
#include "logic.h"

#define N 8

int msgid;

//deletes and clears the messaque queue when exiting (ctrl+C)
void sighandle_int(int sig)
{
	printf("Exiting...");
	if (clear_queue(msgid) == -1)
		exit(EXIT_FAILURE);
	exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[])
{
	//replaces the standard sigint handler
	signal(SIGINT, sighandle_int);

	//controls if the n° of parameters is correct
    if(argc != 2){
        printf("Invalid number of parameters");
        return 1;
    }

    for (int i = 0; i < 2; i++)
	{
		//fork the input and output handlers
		pid_t pid = fork();
		if (pid < 0) 
		{
			printf("Failed to fork the processes");
			return -1;
		}
		if (pid == 0)
		{
			//if it is a child process, then it calls the two executables (one process each)
			char *args[] = {i ? "./out_handle" : "./in_handle", argv[1], NULL};
			execvp(args[0], args);
		}
	}

	//creates the queue to communicate with the two handlers
	msgid = create_id(2);

	while (1)
	{
		swbuffer values;

		//receives the input array from the in_handle
		receive(msgid, &values, sizeof(values), 1);

		swbuffer sendvalues;
		//type=2 to be received only by out_handle
		sendvalues.type = 2;

		//stampa inutile che dobbiamo togliere
		/*printf("handler: ");
		for (int i = 0; i < N; i++)
		{
			printf("%d ", values.state[i]);
		}
		printf("\n");*/

		//calculates the output array
		calc_output(values.state,sendvalues.state);

		printf("output array: ");
		// bisogna cambiare il valore di N e prenderlo dai parametri
		for (int i = 0; i < N; i++)
		{
			printf("%d ", sendvalues.state[i]);
		}
		printf("\n");

		// send the output array to the out_handle
		send(msgid, &sendvalues, sizeof(sendvalues));
	}

	//the parent process waits till the child processes are done
	wait(NULL);
}