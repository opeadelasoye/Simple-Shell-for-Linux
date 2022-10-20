#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>

#define MAX_LINE 80

int main(void){
	char *args[MAX_LINE/2 + 1];
	char cmdLineInput[MAX_LINE];
	int wait_status;
	int should_run = 1;
	int i;
	
	while(should_run == 1){
		memset(args, 0, MAX_LINE/2 + 1);
		i = 0;
		printf("CSCI3120>");
		fflush(stdout);

		fgets(cmdLineInput, MAX_LINE, stdin);
		char *splitInput = strtok(cmdLineInput, " ");
		while(splitInput != NULL){
			args[i] = splitInput;
			splitInput = strtok(NULL, " \n");
			i++;
		}
		if(i == 1){
			args[i-1][strlen(args[i-1])-1]= '\0'; //remove new line
		}
		args[i] = NULL;
		if(strcmp(args[0], "exit") == 0){
				should_run = 0;
				break;
		}
		
		pid_t pid;
		pid = fork(); //fork a child process
		
		if(pid < 0){
			printf("Error: Process failed\n");
			return 1;
		}else if(pid == 0){ //child process
			int execvp_status = execvp(args[0], args);
			if(execvp_status == -1){
				printf("Error: Invalid command\n");
				exit(1);
			}
		}else{ //parent process
			wait(&wait_status);
		}
	}
	return 0;
}
