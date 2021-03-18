//Benjamin Malz - hasher.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/types.h>
#include "sha-256.h"

int thePipe; //writing end of the pipe has to be global to be terminated by signal handler

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

void sigHandler(int theSig){
	close(thePipe); //closes the writing end of the pipe
	printf("This hasher was terminated by signal %d.\n", theSig);
	exit(0); //closes program
}

int countZeros(char* hashStr){ //function to count the leading zeros in a hashed string
	int i, leadingZ;
	i = 0;
	leadingZ = 0;
	while(hashStr[i] == '0'){ //continues to move through the array while there are more zeros
		leadingZ += 4;
		i++;
	}
	if (hashStr[i] == '1'){
		leadingZ += 3;
	}
	else if (hashStr[i] == '2' || hashStr[i] == '3'){
		leadingZ += 2;
	}
	else if (hashStr[i] == '4' || hashStr[i] == '5' || hashStr[i] == '6' || hashStr[i] == '7'){
		leadingZ += 1;
	}
	else {
		leadingZ += 0;
	}
	return leadingZ;
}

int main(int args, char* arg[]){
	signal(SIGINT, sigHandler); //this will be called if a hasher successfully found a hash
	thePipe = atoi(arg[2]); //assign the pipe to the global variable
	char *strbuffer = malloc(1000012 * sizeof(char));
	char *buffer = malloc(1000000 * sizeof(char));
	char hashed[65];
	int time, genNum, leadingZero, neededZero, found, theI;
	found = 0;
	neededZero = atoi(arg[3]);
	struct timeval seed; //struct to get time
	readFile(arg[1], buffer);
	while(found == 0){
			gettimeofday(&seed, NULL);
			time = (seed.tv_sec * 1000000 + seed.tv_usec);
			srand(time);
			genNum = rand() % 1000000;
			for (int i = 0; i < 1000000; i++){
				sprintf(strbuffer, "%s%06d%06d", buffer, genNum, i); //generate next string to hash
				sha_256_string(hashed, strbuffer, strlen(strbuffer));
				leadingZero = countZeros(hashed);
				if (leadingZero >= neededZero){
					theI = i;
					i = 1000000;
					found = 1;
			}
		}
	}
	int hashNums[2]; //array to be sent through the pipe
	hashNums[0] = genNum;
	hashNums[1] = theI;
	write(thePipe, hashNums, 2 * sizeof(int)); //send array through the pipe
	close(thePipe); //close pipe
	free(strbuffer);
	free(buffer);
	return 0;
}