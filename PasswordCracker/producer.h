#ifndef PRODUCER
#define PRODUCER
	
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>

		void* writeToBuffer(void*);

#endif
