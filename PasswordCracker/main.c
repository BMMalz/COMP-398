//Benjamin Malz, Brandon Witter, Ian Patrick Morris
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <pthread.h>
#include "global.h"
#include "consumer.h"
#include "producer.h"
#include "sha-256.h"

int main(int argc, char* argv[])
{
	if(argc != 5)
	{
		printf("Not Enough Arguments, Terminating Program\n\n Have a Nice Day ;)");
		exit (1);
	}	

	char* dict; //Arg 1
	char* input; //Arg 2
	char* output; //Arg 3
	int numberOfThreads; //Arg 4

	dict = argv[1];
	input = argv[2];
	output = argv[3];
	numberOfThreads = atoi(argv[4]);

	FILE *fptr;
	if ((fptr = fopen(input,"r")) == NULL) //Opening file
   	{
       		printf("Error! opening file\n");
			exit(1);
   	}

	char hashed[65];
	fscanf(fptr, "%s", hashed);
	fclose(fptr); //closing input file
	printf("Hash to find: %s\n", hashed);

	struct GlobalBuffer globStruct; //initialize variables for global buffer
	globStruct.nextIn = 0;
	globStruct.nextOut = 0;
	globStruct.occu = 0;
	globStruct.found = 0;
	globStruct.eof = 0;
	pthread_mutex_init(&(globStruct.m), NULL);
	pthread_cond_init(&(globStruct.more),NULL);
	pthread_cond_init(&(globStruct.less), NULL);
	globStruct.dictFile = dict;
	globStruct.outputFile = output;
	globStruct.hash = hashed;
	globStruct.buffer = malloc(10000 * (sizeof(char*)));

	for(int i = 0; i < 10000; i++) //set buffer to be 100 char strings
	{
		globStruct.buffer[i] = malloc(100 * (sizeof(char)));
	}

   	pthread_t *tid = malloc((numberOfThreads + 1) * sizeof(pthread_t)); //create thread array
    void* infoForThreads = &globStruct;

    pthread_create((&tid[0]), NULL, writeToBuffer, infoForThreads);

   	for(int i = 1; i <= numberOfThreads; i++)
   	{
   		pthread_create((&tid[i]), NULL, getWord, infoForThreads);
   	}


   	pthread_join(tid[0], NULL);

	for(int i = 1; i <= numberOfThreads; i++)
	{
		pthread_join(tid[i], NULL);
	}

   	for(int i = 0; i < 10000; i++)
   	{
   		free(globStruct.buffer[i]);
   	}

	pthread_mutex_destroy(&globStruct.m);
	pthread_cond_destroy(&globStruct.more);
	pthread_cond_destroy(&globStruct.less);
	free(tid);
	free(globStruct.buffer);
   	return 0;
}
