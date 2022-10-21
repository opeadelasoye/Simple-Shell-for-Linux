#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>

#define MAX_LINE 80
#define MAX_HISTORY 10

int main(){
	char *args[MAX_LINE/2 + 1];
	char cmdLineInput[MAX_LINE];
	int execvp_status;
	int wait_status;
	int should_run = 1;
	int i;
	
	int historyTracker = 0;
	int historyCalled;
	int historyID[MAX_HISTORY];
	int historyPID[MAX_HISTORY];
	char historyCommand[MAX_HISTORY][MAX_LINE];
	
	while(should_run == 1){
		memset(args, 0, sizeof(args));
		memset(cmdLineInput, 0, sizeof(cmdLineInput));
		i = 0;
		printf("CSCI3120>");
		fflush(stdout);

		fgets(cmdLineInput, MAX_LINE, stdin);
		char currentInput[MAX_LINE];
		int x;
		for(x = 0; x < sizeof(cmdLineInput); x++){
			currentInput[x] = cmdLineInput[x];
		}
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
		
		currentInput[strlen(currentInput)-1]= '\0';
		
		if(strcmp(args[0], "exit") == 0){
			should_run = 0;
			break;
		}else if(strcmp(args[0], "history") == 0){
			historyCalled = 0;
			int j;
			if(historyTracker > 0){
				printf("ID\tPID\tCommand\n");
				for(j = 0; j < historyTracker; j++){
					printf("%d\t%d\t%s\n", historyID[j], historyPID[j], historyCommand[j]);
				}
			}else{
				printf("No commands recorded yet\n");
			}
		}else{
			historyCalled = 1;
		}
		
		pid_t pid;
		pid = fork(); //fork a child process
		
		if(historyTracker < 10 && historyCalled == 1){
			historyID[historyTracker] = historyTracker+1;
			historyPID[historyTracker] = pid;
			strcpy(historyCommand[historyTracker], currentInput);
			historyTracker++;
		}else if(historyCalled == 1){
			int l;
			for(l = 0; l < 9; l++){
				historyPID[l] = historyPID[l + 1];
				strcpy(historyCommand[l], historyCommand[l+1]);
			}
			historyID[l] = 10;
			historyPID[l] = pid;
			strcpy(historyCommand[l], currentInput);				
		}		
		
		if(pid < 0){
			printf("Error: Process failed\n");
			return 1;
		}else if(pid == 0){ //child process
			execvp_status = execvp(args[0], args);
			if(execvp_status == -1 && ((strcmp(args[0], "history") > 0) || (strcmp(args[0], "history") < 0))){
				printf("Error: Invalid command\n");
				exit(1);
			}
		}else{ //parent process
			wait(&wait_status);
		}
	}
	exit(0);
}
