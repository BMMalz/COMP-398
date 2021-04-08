/*Benjamin Malz
Game of Life
COMP-398 Program Assignment #1*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <string.h>

//function to open and read the file and fill the variables as specified by the file
int **readFile(char* file, int ** newGrid, int* xaxis, int* yaxis, int* iterations){
	FILE* reader;
	int row, col;
	if (reader = fopen(file, "r")){
		fscanf(reader, "%i", xaxis);
		fscanf(reader, "%i", yaxis);
		fscanf(reader, "%i", iterations); //read the 3 initial variables outside of a loop to simplify taking input
		newGrid = malloc(*xaxis * sizeof(int*));
		for (int i = 0; i < *xaxis; i++){ //allocate the 2D array to the dimensions specified by the file
			newGrid[i] = malloc(*yaxis * sizeof(int));
		}
		for (int i = 0; i < *xaxis; i++){
			for (int j = 0; j < *yaxis; j++){
				newGrid[i][j] = 0; //fills the grid with dead cells prior to getting live cell coordinates
			}
		}
		while (fscanf(reader, "%d %d\n", &row, &col) != EOF){
			newGrid[row][col] = 1; //sets every coordinate given by the file to a live cell
		}
		return newGrid;
	}
	else{
		printf("Invalid file. Check your input.\n"); //if the file does not exist, exit the program
		exit(0);
	}
}
//this function will check the status of a cell's neighbors and determine the status of the cell in the next iteration of the game based on how many dead/alive neighbors it has.
void stayingAlive(int x, int y, int xaxis, int yaxis, int torus, int ** gridOne, int ** gridTwo){
	//topleft neighbor
		int x1, y1, lifeCounter, wraps;
		x1 = x - 1;
		y1 = y - 1;
		wraps = 0; //this will be set to 1 every time that there is wrapping, so that if the grid is not set to be a torus it will disregard wrapping neighbors
		lifeCounter = 0; //lifecounter will increase by one for every live neighboring cell, used to determine what will happen to the current cell
		if ((x - 1) < 0){
			x1 = xaxis - 1;
			wraps = 1;
		}
		if ((y - 1) < 0){
			y1 = yaxis - 1;
			wraps = 1;
		}
		if (gridOne[x1][y1] == 1){
			if (torus == 0 && wraps == 1){ //if the neighbor cannot be reached unless wrapping and the grid is not a torus, disregard this neighbor
				lifeCounter +=0;
			}
			else{
				lifeCounter++;
		}}
		//topmid neighbor
		wraps = 0;
		x1 = x;
		y1 = y - 1;
		if ((y - 1) < 0){
			y1 = yaxis - 1;
			wraps = 1;
		}
		if (gridOne[x1][y1] == 1){
			if (torus == 0 && wraps == 1){
				lifeCounter +=0;
			}
			else{
				lifeCounter++;
		}}
		//topright neighbor
		wraps = 0;
		x1 = x + 1;
		y1 = y - 1;
		if ((x + 1) >= xaxis){
			x1 = 0;
			wraps = 1;
		}
		if ((y - 1) < 0){
			y1 = yaxis - 1;
			wraps = 0;
		}
		if (gridOne[x1][y1] == 1){
			if (torus == 0 && wraps == 1){
				lifeCounter +=0;
			}
			else{
				lifeCounter++;
		}}
		//midleft neighbor
		wraps = 0;
		y1 = y;
		x1 = x - 1;
		if ((x - 1) < 0){
			x1 = xaxis - 1;
			wraps = 1;
		}
		if (gridOne[x1][y1] == 1){
			if (torus == 0 && wraps == 1){
				lifeCounter +=0;
			}
			else{
				lifeCounter++;
		}}
		//midright neighbor
		wraps = 0;
		y1 = y;
		x1 = x + 1;
		if ((x + 1) >= xaxis){
			wraps = 1;
			x1 = 0;
		}
		if (gridOne[x1][y1] == 1){
			if (torus == 0 && wraps == 1){
				lifeCounter +=0;
			}
			else{
				lifeCounter++;
		}}
		//botleft neighbor
		wraps = 0;
		x1 = x - 1;
		y1 = y + 1;
		if ((x - 1) < 0){
			wraps = 1;
			x1 = xaxis - 1;
		}
		if ((y + 1) >= yaxis){
			wraps = 1;
			y1 = 0;
		}
		if (gridOne[x1][y1] == 1){
			if (torus == 0 && wraps == 1){
				lifeCounter +=0;
			}
			else{
				lifeCounter++;
		}}
		//botmid neighbor
		wraps = 0;
		x1 = x;
		y1 = y + 1;
		if ((y + 1) >= yaxis){
			wraps = 1;
			y1 = 0;
		}
		if (gridOne[x1][y1] == 1){
			if (torus == 0 && wraps == 1){
				lifeCounter +=0;
			}
			else{
				lifeCounter++;
		}}
		//botright neighbor
		wraps = 0;
		x1 = x + 1;
		y1 = y + 1;
		if ((x + 1) >= xaxis){
			wraps = 1;
			x1 = 0;
		}
		if ((y + 1) >= yaxis){
			wraps = 1;
			y1 = 0;
		}
		if (gridOne[x1][y1] == 1){
			if (torus == 0 && wraps == 1){
				lifeCounter +=0;
			}
			else{
			lifeCounter++;
		}}
		if (lifeCounter < 2){
			gridTwo[x][y] = 0; //when the current cell has less than 2 live neighbors, it dies
		}
		else if ((lifeCounter == 2 || lifeCounter == 3) && gridOne[x][y] == 1){
			gridTwo[x][y] = 1; //if the cell has 2 or 3 live neighbors and is currently alive, it remains alive
		}
		else if (lifeCounter > 3){
			gridTwo[x][y] = 0; //if the cell has more than 3 neighbors, it dies
		}
		else if (lifeCounter == 3 && gridOne[x][y] == 0){
			gridTwo[x][y] = 1; // if the cell is currently dead but has exactly 3 live neighbors, it comes back to life
		}
	}

int main(int args, char* arg[]){
	if (args < 4 || args > 5){ //if there are more or less arguments in the command line than expected, end the program immediately
		printf("Invalid number of arguments. Try again. \n");
		return 0;
	}
	int xaxis, yaxis, iterations, torus, show, runspeed;
	double pretime, pretimeu, posttime, posttimeu, endtime;
	if (strcmp("wrap", arg[2]) == 0){ //sets the grid to be a torus if the second argument is 'wrap'
		torus = 1;
	}
	else if (strcmp("nowrap", arg[2]) == 0){ //sets the grid to not be a torus if second argument is 'nowrap'
		torus = 0;
	}
	else{
		printf("Invalid value for the 'wrap' argument. Try again.\n"); //invalid input if not 'wrap' or 'nowrap'
		return 0;
	}
	if (strcmp("show", arg[3]) == 0){ //enables visual updates to the grid if third argument is 'show'
		show = 1;
	}
	else if (strcmp("hide", arg[3]) == 0){ //no visual updates to the grid if third argument is 'hide'
		show = 0;
	}
	else{
		printf("Invalid value for the 'show' argument. Try again.\n"); //catches any invalid input for third argument
		return 0;
	}
	if (show == 1 && args == 5){ //sets the game speed based on the fourth argument's input
		if (strcmp("slow", arg[4]) == 0){
			runspeed = 333333;
		}
		else if (strcmp("med", arg[4]) == 0){
			runspeed = 100000;
		}
		else if (strcmp("fast", arg[4]) == 0){
			runspeed = 33333;
		}
		else{
			printf("Your fifth argument is invalid despite choosing the 'show' argument. Try again.\n"); //catches any invalid input for the game speed
			return 0;
		}
	}
	{
		
	};
	int** currGrid = readFile(arg[1], currGrid, &xaxis, &yaxis, &iterations); //initialize the 2D array and send the first argument, the filename, to get information on the grid
	int** nextGrid; //this grid will contain changes to the grid for the next iteration of the game
	nextGrid = malloc(xaxis * sizeof(int*));
	for (int i = 0; i < xaxis; i++){
		nextGrid[i] = malloc(yaxis * sizeof(int));
	}
	for (int i = 0; i < xaxis; i++){
		for (int j = 0; j < yaxis; j++){
			nextGrid[i][j] = 0; //fill the grid with dead cells
		}
	}
	if (show == 1){ //if the third argument was show, print the initial grid
		for (int i = 0; i < xaxis; i++){
			for (int j = 0; j < yaxis; j++){
				if (currGrid[i][j] == 0){
					printf("- ");
				}
				else{
					printf("X ");
				}
			}
			printf("\n");
	}}
	struct timeval timepre;
	gettimeofday(&timepre, NULL); //get the initial time prior to the loop beginning
	pretime = timepre.tv_sec; //store the time in seconds
	pretimeu = timepre.tv_usec; //store the time in nanoseconds 
	for (int i = 0; i < iterations; i++){ //go through this loop for the specified number of iterations given in the file
		for (int j = 0; j < xaxis; j++){
			for (int k = 0; k < yaxis; k++){
				stayingAlive(j, k, xaxis, yaxis, torus, currGrid, nextGrid); //for loop will send every cell of the game in this iteration to determine its status for the next iteration
			}
		}
		for (int l = 0; l < xaxis; l++){
			for (int m = 0; m < yaxis; m++){
				currGrid[l][m] = nextGrid[l][m]; //copy the contents of the future grid's cells to the current grid
				nextGrid[l][m] = 0; //empty the future grid
			}
		}
		system("clear"); //clear the output before displaying the next grid
		if (show == 1){ //if 'show' is enabled, prints the grid after it was updated by the function
			for (int n = 0; n < xaxis; n++){
				for (int o = 0; o < yaxis; o++){
					if (currGrid[n][o] == 0){
						printf("- ");
					}
					else{
						printf("X ");
					}
				}
				printf("\n");
			}
			usleep(runspeed); //sleeps the game depending on the speed of the game
		}}
	struct timeval timepost;
	gettimeofday(&timepost, NULL); //get the time at the end of the game

	posttime = timepost.tv_sec;
	posttimeu = timepost.tv_usec;
	endtime = posttime - pretime + (posttimeu - pretimeu)/1000000; //subtract the initial time from the end time


	if (show == 0){ //prints the final grid only if 'hide' was enabled - this will be the only time the user sees the grid in this case
		for (int n = 0; n < xaxis; n++){
			for (int o = 0; o < yaxis; o++){
				if (currGrid[n][o] == 0){
					printf("- ");
				}
				else{
					printf("X ");
				}
			}
			printf("\n");
	}}

	for (int i = 0; i < xaxis; i++){
		free(currGrid[i]); //free the memory allocated by each spot of the 2D array
	}
	free(currGrid); //free the array
	for (int i = 0; i < xaxis; i++){
		free(nextGrid[i]); //free the memory allocated by each spot of the 2D array
	}
	free(nextGrid); //free the array
	printf("Total time to do %d iterations of %dx%d is %f seconds.\n", iterations, xaxis, yaxis, endtime); //display the total amount of time in seconds it took to run the program
	return 0;
	}