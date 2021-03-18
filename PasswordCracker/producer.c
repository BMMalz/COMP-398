//Benjamin Malz, Brandon Witter, Ian Patrick Morris
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#include <pthread.h>
#include "producer.h"
#include "global.h"

void* writeToBuffer(void* package){
	struct GlobalBuffer *buffBoy = (struct GlobalBuffer *)package; //creates local variable for global buffer
	char* fileBuff[100];
	FILE* reader;
	for (int i = 0; i < 100; i++){
		fileBuff[i] = (char*)malloc(50 * sizeof(char)); //creates local buffer of strings
	}
	int loop = 0;
	if ((reader = fopen(buffBoy->dictFile, "r"))){
		while (fscanf(reader, "%s", fileBuff[loop]) != EOF && buffBoy->found == 0){  //reads file
			if (loop >= 99){ //resets loop
    			if (buffBoy->occu >= 9900){ //waits if global buffer is at capacity
    				pthread_mutex_lock(&buffBoy->m);
       				pthread_cond_wait(&buffBoy->less, &buffBoy->m);
       				pthread_mutex_unlock(&buffBoy->m);
    			}
       			for (int i = 0; i < 100; i++){ //increases nextin, prevents it from going over 100, sends local strings
			    	strcpy(buffBoy->buffer[buffBoy->nextIn], fileBuff[i]);
			    	buffBoy->nextIn++;
			    	buffBoy->nextIn %= 10000;
			    	buffBoy->occu++;
				}


    			pthread_cond_broadcast(&buffBoy->more); //signal that there's more

				loop = 0;
			}
			else{
				loop++;
			}
		}

		fclose(reader);
		buffBoy->eof = 1; //signal that EOF
	}
	else{
		printf("File does not exist. Check your input. \n");
		exit(0);
		}

	for (int i = 0; i < 100; i++){
		free(fileBuff[i]);
	}

	pthread_exit(0);
}