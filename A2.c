#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <ctype.h>

#define MAX_LINE 80
#define MAX_HISTORY 10

//Sources used
//https://www.programiz.com/c-programming/library-function/string.h/strcpy - strcpy
//https://www.programiz.com/c-programming/library-function/ctype.h/isdigit#:~:text=The%20isdigit()%20function%20checks,0%2D9)%20or%20not. - isdigit
//https://www.tutorialspoint.com/c_standard_library/c_function_memset.htm - memset
//https://www.geeksforgeeks.org/multidimensional-arrays-c-cpp/ - 2d array

int main(){
	char *args[MAX_LINE/2 + 1];
	char cmdLineInput[MAX_LINE];
	int execvp_status, wait_status;
	int should_run = 1;
	int i;
	
	int historyTracker = 0;
	int historyID[MAX_HISTORY];
	int historyPID[MAX_HISTORY];
	char historyCommand[MAX_HISTORY][MAX_LINE];
	
	while(should_run == 1){
		memset(args, 0, sizeof(args));
		memset(cmdLineInput, 0, sizeof(cmdLineInput));
		int historyCalled = 1, exitCalled = 1, recentCalled = 1;
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
			exitCalled = 0;
		}else if(strcmp(args[0], "history") == 0){
			historyCalled = 0;
			int j;
			if(historyTracker > 0){
				printf("ID\tPID\tCommand\n");
				for(j = 0; j < historyTracker; j++){
					printf("%d\t%d\t%s\n", historyID[j], historyPID[j], historyCommand[j]);
				}
			}else{
				printf("No command in history!\n");
			}
		}else if(strcmp(args[0], "!!") == 0){
			recentCalled = 0;
			if(historyTracker == 0){
				printf("No command in history!\n");
				historyCalled = 0;
			}else{
				printf("%s\n", historyCommand[historyTracker - 1]);
				i = 0;
				strcpy(cmdLineInput, historyCommand[historyTracker - 1]);
				char *recentInput = strtok(cmdLineInput, " ");
				while(recentInput != NULL){
					args[i] = recentInput;
					recentInput = strtok(NULL, " \n");
					i++;
				}
				args[i] = NULL;	
			}
		}else if(isdigit(args[0][1]) > 0){
			recentCalled = 0;
			if(args[0][1] - 48 <= 0 || args[0][1] - 48 > 10){
				printf("Such a command is not in history 1");	
			}else{
				if(historyID[args[0][1]] - 48 > historyTracker){
					printf("Such a command is not in history 2");
				}else{
					printf("%s\n", historyCommand[args[0][1] - 48 - 1]);
					i = 0;
					strcpy(cmdLineInput, historyCommand[args[0][1] - 48 - 1]);
					char *recentInput = strtok(cmdLineInput, " ");
					while(recentInput != NULL){
						args[i] = recentInput;
						recentInput = strtok(NULL, " \n");
						i++;
					}
					args[i] = NULL;	
				}
			}
			
		}
		
		pid_t pid;
		if(historyCalled == 1 && exitCalled == 1){
			pid = fork();
		}
		
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
			if(historyCalled == 0 || exitCalled == 0 || recentCalled == 0){
			}else{
				printf("Error: Process failed\n");
				return 1;
			}
		}else if(pid == 0){ //child process
			execvp_status = execvp(args[0], args);
			if(execvp_status == -1 && ((strcmp(args[0], "history") < 0))){
				printf("Invalid command!\n");
				exit(1);
			}
		}else{ //parent process
			wait(&wait_status);
		}
	}
	return 0;
}
