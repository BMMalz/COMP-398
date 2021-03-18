//Benjamin Malz - main.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>

void readFile(char* filename, char* buffer){
	FILE* reader;
	int i, read;
	i = 0;
	if ((reader = fopen(filename, "r"))){
		read = fgetc(reader);
		while (read != EOF){ 
			buffer[i] = read;
			i++;
			read = fgetc(reader);
		}
		buffer[i] = '\0';
		fclose(reader);
		}
	else{
		printf("File does not exist. Check your input.\n");
		exit(0);
	}
}

int main(int args, char* arg[]){
	char *strbuffer = malloc(1000012 * sizeof(char));
	char *buffer = malloc(1000000 * sizeof(char));
	readFile(arg[1], buffer);
	char* hasher = "./hasher"; //used to open hasher
	int processes;
	processes = atoi(arg[4]); //number of processes to run
	char** argv = malloc(5 * sizeof(char*)); //allocate array for arguments for hasher
	pid_t ret;
	pid_t* procIDS = malloc(processes * sizeof(pid_t*)); //array of process IDs
	int** thePipes;
	thePipes = malloc(processes * sizeof(int*)); //creates an array of arrays
	for (int i = 0; i < processes; i++){
		thePipes[i] = malloc(2 * sizeof(int)); //each array is 2 ints, reading and writing end
		pipe(thePipes[i]); //turns the arrays into pipes
	}
	argv[0] = hasher;
	argv[1] = arg[1];
	char sender[20]; //this string will send the writing end of the pipe
	int receivedNums[2]; //this is where the two integers returned by the pipe will be stored
	argv[3] = arg[3];
	argv[4] = NULL;
	for (int i = 0; i < processes; i++){
		ret = fork();
		if (ret != 0)
		{
			procIDS[i] = ret; //gathers the process IDs to terminate later
		}
		else{
			sprintf(sender, "%d", thePipes[i][1]); //put the writing end of the pipe into the buffer
			argv[2] = sender; //writing end of the pipe becomes the second argument
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
				kill(procIDS[j], SIGINT); //kill processes that are still running
				close(thePipes[j][0]); //close the reading end of the pipe for unsuccessful processes
				}
			else{
				read(thePipes[j][0], receivedNums, 2 * sizeof(int)); //read the 2 integer array into the receiving array
				close(thePipes[j][0]); //close successful reading end of pipe
			}
			}
  		}
	}

	sprintf(strbuffer, "%s%06d%06d", buffer, receivedNums[0], receivedNums[1]); //format the string to have 6 digits with extra zeroes so it gives the correct hash
	FILE* writer;
	writer = fopen(arg[2], "w"); //write the unhashed string to the specified filename
	fprintf(writer, "%s\n", strbuffer);
	fclose(writer);
	for (int i = 0; i < processes; i++){ //free the pipe arrays
		free(thePipes[i]);
	}
	free(thePipes);
	free(argv);
	free(procIDS);
	free(buffer);
	free(strbuffer);
	return 0;
}