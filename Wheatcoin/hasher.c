//Benjamin Malz - hasher.c
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <sys/time.h>
#include "sha-256.h"


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
	char strbuffer[1000012];
	char buffer[1000000];
	char hashed[65];
	int read, looper, time, genNum, leadingZero, neededZero, found;
	neededZero = atoi(arg[3]);
	looper = 0;
	struct timeval seed; //struct to get time
	FILE* reader;
	if ((reader = fopen(arg[1], "r"))){
		read = fgetc(reader);
		while (read != EOF){ 
			buffer[looper] = read;
			looper++;
			read = fgetc(reader);
		}
		fclose(reader);
		}
	else{
		printf("File does not exist. Check your input.\n");
		return 0;
	}
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
					i = 1000000;
					found = 1;
			}
		}
	}
	FILE* writer;
	writer = fopen(arg[2], "w");
	fprintf(writer, "%s\n", strbuffer);
	fclose(writer);
	return 0;
}