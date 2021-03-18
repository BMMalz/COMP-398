//Benjamin Malz, Brandon Witter, Ian Patrick Morris
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#include <pthread.h>
#include "consumer.h"

void passwordGen(char* word, char* hash, struct GlobalBuffer *Global){
	int wordLength = strlen(word);		// int that stores the length of the word
	char hashed[65];
	char* word_with_ex = malloc((wordLength + 1) * (sizeof(char)));		// array to store word modified with i -> ! (+1 for null terminator)
	strcpy(word_with_ex, word);		// copy word
	char* word_with_one = malloc((wordLength + 1) * (sizeof(char)));		// array to store word modified with l -> 1 (+1 for null terminator)
	strcpy(word_with_one, word);		// copy word
	char* word_with_zero = malloc((wordLength + 1) * (sizeof(char)));	// array to store word modified with o -> 0 (+1 for null terminator)
	strcpy(word_with_zero, word);		// copy word
	char* word_with_ex_one = malloc((wordLength + 1) * (sizeof(char)));	// array to store word modified with i -> !, l -> 1 (+1 for null terminator)
	strcpy(word_with_ex_one, word);		// copy word
	char* word_with_ex_zero = malloc((wordLength + 1) * (sizeof(char)));	// array to store word modified with i -> !, o -> 0 (+1 for null terminator)
	strcpy(word_with_ex_zero, word);		// copy word
	char* word_with_one_zero = malloc((wordLength + 1) * (sizeof(char)));	// array to store word modified with l -> 1, o -> 0 (+1 for null terminator)
	strcpy(word_with_one_zero, word);		// copy word
	char* word_with_all = malloc((wordLength + 1) * (sizeof(char)));		// array to store word modified with i -> !, l -> 1, o -> 0 (+1 for null terminator)
	strcpy(word_with_all, word);		// copy word

	for(int i = 0; i < wordLength; i++){
		if (word[i] == 'i'){	// replace all i's with !'s
			word_with_ex[i] = '!';
			word_with_ex_one[i] = '!';
			word_with_ex_zero[i] = '!';
			word_with_all[i] = '!';
		}
		if (word[i] == 'l'){	// replace all l's with 1's
			word_with_one[i] = '1';
			word_with_ex_one[i] = '1';
			word_with_one_zero[i] = '1';
			word_with_all[i] = '1';
		}
		if (word[i] == 'o'){	// replace all o's with 0's
			word_with_zero[i] = '0';
			word_with_ex_zero[i] = '0';
			word_with_one_zero[i] = '0';
			word_with_all[i] = '0';
		}
	}

	char* password = malloc((wordLength + 2) * (sizeof(char)));	// stores the password + number
	char* passNoNum = malloc((wordLength + 1) * (sizeof(char)));		// stores the password without the appended number
	char strNum[2];		// stores an int as a string to concatenate with passwordNoNum

	for (int i = 0; i < 8; i++){	// goes through each possible password without any numbers
		if (i == 0)
			strcpy(passNoNum, word);
		if (i == 1){
			if (strcmp(word_with_ex, word) == 0)
				i++;
		
			else
				strcpy(passNoNum, word_with_ex);
		}
		if (i == 2){
			if (strcmp(word_with_one, word) == 0)
				i++;
		
			else
				strcpy(passNoNum, word_with_one);
		}
		if (i == 3)
		{
			if (strcmp(word_with_zero, word) == 0)
				i++;
	
			else
				strcpy(passNoNum, word_with_zero);
		}
		if (i == 4){
			if (strcmp(word_with_ex_one, word) == 0 || strcmp(word_with_ex_one, word_with_ex) == 0 || strcmp(word_with_ex_one, word_with_one) == 0)
				i++;
			else
				strcpy(passNoNum, word_with_ex_one);
		}
		if (i == 5){
			if (strcmp(word_with_ex_zero, word) == 0 || strcmp(word_with_ex_zero, word_with_ex) == 0 || strcmp(word_with_ex_zero, word_with_zero) == 0)
				i++;
			else
				strcpy(passNoNum, word_with_ex_zero);
		}
		if (i == 6){
			if (strcmp(word_with_one_zero, word) == 0 || strcmp(word_with_one_zero, word_with_one) == 0 || strcmp(word_with_one_zero, word_with_zero) == 0)
				i++;
			else
				strcpy(passNoNum, word_with_one_zero);
		}
		if (i == 7)
		{
			if (strcmp(word_with_all, word) == 0 || strcmp(word_with_all, word_with_ex) == 0 || strcmp(word_with_all, word_with_one) == 0
				|| strcmp(word_with_all, word_with_zero) == 0 || strcmp(word_with_all, word_with_ex_one) == 0 || strcmp(word_with_all, word_with_ex_zero) == 0 
				|| strcmp(word_with_all, word_with_one_zero) == 0)
				i++;
			else
				strcpy(passNoNum, word_with_all);
		}
		if (i != 8){	// as long as the modified word does not equal a password we have already done
			sha_256_string(hashed, passNoNum, strlen(passNoNum));
			if (strcmp(hash, hashed) == 0){
					Global->found = 1;
					pthread_cond_broadcast(&Global->more);
					pthread_cond_broadcast(&Global->less);
					printf("Found password. Outputting to %s.\n", Global->outputFile);
					FILE *fOut;
					fOut = fopen(Global->outputFile,"w");
				   	fprintf(fOut, "%s\n", passNoNum);
				   	fclose(fOut);
				}
			// ==========CHECK PASSWORD HERE========== checks all possible passwords without any numbers
			for (int j = 0; j < 10; j++){
				strcpy(password, passNoNum);	// set password equal to the password with no number
				sprintf(strNum, "%d", j);		// convert the number to a string
				strcat(password, strNum);		// add the digit to the end of the password
				sha_256_string(hashed, password, strlen(password));
				if (strcmp(hash, hashed) == 0){
					Global->found = 1;
					pthread_cond_broadcast(&Global->more);
					pthread_cond_broadcast(&Global->less);
					printf("Found password. Outputting to %s.\n", Global->outputFile);
					FILE *fOut;
					fOut = fopen(Global->outputFile,"w");
				   	fprintf(fOut, "%s\n", password);
				   	fclose(fOut);
				}
				// ==========CHECK PASSWORD HERE========== checks all possible passwords with numbers
			}
		}
	}
	// Free all the mallocs
	free(word_with_ex);
	free(word_with_one);
	free(word_with_zero);
	free(word_with_ex_one);
	free(word_with_ex_zero);
	free(word_with_one_zero);
	free(word_with_all);
	free(password);
	free(passNoNum);
}

void* getWord(void* package){
	char passBoy[50];
	struct GlobalBuffer *con = (struct GlobalBuffer *) package;
	struct GlobalBuffer *consumption = con;
	while (consumption->found == 0){
		pthread_mutex_lock(&consumption->m);
		strcpy(passBoy, consumption->buffer[consumption->nextOut]); //take string from global buffer
		consumption->nextOut++;
		pthread_mutex_unlock(&consumption->m);
	    
	    if(consumption->occu <= 0){ //wait for more input
	    	pthread_mutex_lock(&consumption->m);
	        pthread_cond_wait(&consumption->more, &consumption->m);
	        pthread_mutex_unlock(&consumption->m);
	    }

	    consumption->nextOut %= 10000;
	    consumption->occu--;

	    passwordGen(passBoy, consumption->hash, consumption);
	    if (consumption->occu < 9900){
	    pthread_cond_signal(&consumption->less); //signal that can take more strings to buffer
		}
	    if (consumption->eof == 1 && consumption->occu == 0){
			consumption->found = 2; //exit loop if no more strings
		}

	}
	if (consumption->found == 2){
		printf("Not found!\n");
	}
	
   	pthread_exit(0);
}