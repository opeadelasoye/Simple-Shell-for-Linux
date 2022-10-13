#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_LINE 80

int main(void){
	char *args[MAX_LINE/2 + 1];
	int should_run = 1;
	
	while(should_run){
		printf("CSCI3120>");
		fflush(stdout);
	}
	
	return 0;
}
