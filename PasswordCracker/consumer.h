#ifndef CONSUMER
#define CONSUMER

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include "global.h"
#include "sha-256.h"

		void* getWord(void*);
		void passwordGen(char*, char*, struct GlobalBuffer*);

#endif