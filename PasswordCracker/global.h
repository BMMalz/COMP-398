#ifndef HEADER
#define HEADER

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>

struct GlobalBuffer
{
	char** buffer; //Global buffer
	int nextIn; //Location to be filled by producer
	int nextOut; //Location to be read by the consumer
	int occu; //If current position occupied
	int found; //For if answer found
	int eof; //Variable for if EOF reached
	//Locking statements
	pthread_mutex_t m;
	pthread_cond_t more; //more
	pthread_cond_t less; //less
	//Files name
	char* dictFile;
	char* outputFile;
	char* hash;
};


#endif