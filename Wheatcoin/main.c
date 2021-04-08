//Benjamin Malz - main.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>

int main(int args, char* arg[]){
	FILE* reader;
	if ((reader = fopen(arg[1], "r"))){ //check file exists
		fclose(reader);
	}
	else{
		printf("Invalid file. Terminating.\n");
		return 0;
	}
	char* hasher = "./hasher"; //used to open hasher
	char output[10]; //maximum of 1000 processes for file output
	int processes;
	processes = atoi(arg[3]); //number of processes to run
	char** argv = malloc(5 * sizeof(char*)); //allocate array for arguments for hasher
	pid_t ret;
	pid_t* procIDS = malloc(processes * sizeof(pid_t*)); //array of process IDs
	argv[0] = hasher;
	argv[1] = arg[1];
	argv[3] = arg[2];
	argv[4] = NULL;
	for (int i = 0; i < processes; i++){
		sprintf(output, "out%d.txt", i); //increment the output file's number
		ret = fork();
		if (ret != 0)
		{
			procIDS[i] = ret; //gathers the process IDs to terminate later
		}
		else{
			argv[2] = output;
			execvp(hasher, argv); //runs hasher
		}
}
int status;
for (int i = 0; i < processes; i++){
	pid_t foundProc;
	foundProc = wait(&status); //first process to return is the first one to have found the proper number of zeros
	if (WIFEXITED(status)) {
		for (int j = 0; j < processes; j++){
			if (procIDS[j] != foundProc){
				kill(procIDS[j], SIGKILL); //kill processes that are still running
			}
			else{
				printf("Success! Output can be found in the file out%d.txt.\n", j);
				kill(procIDS[j], SIGKILL);
			}
			}
  		}
	}
	free(procIDS);
	free(argv);
	return 0;
}