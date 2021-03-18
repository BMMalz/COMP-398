//Programmers: Michael Allen and Benjamin Malz
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <pthread.h>

pthread_barrier_t Bpit;
pthread_barrier_t Total;
pthread_mutex_t printT;
int roundCount,totalCount;

struct threadData{// struct for worker threads
	int threadTid, firstRow, lastRow, firstCol, lastCol, totalCol, totalRow, torus, xaxis, yaxis ,iterations;
	int **currGrid, **nextGrid;
};
struct printer{ //struct for printer thread
	int show, xaxis, yaxis, runspeed, iterations;
	int **currGrid, **nextGrid;

};

int **readFile(char* file, int ** newGrid, int* xaxis, int* yaxis, int* iterations){ //reads file, creates grid from file contents
	FILE* reader;
	int row, col;
	if ((reader = fopen(file, "r"))){
	fscanf(reader, "%i", xaxis);
	fscanf(reader, "%i", yaxis);
	fscanf(reader, "%i", iterations);
	newGrid = malloc(*xaxis * sizeof(int*));
	for (int i = 0; i < *xaxis; i++){
		newGrid[i] = malloc(*yaxis * sizeof(int));
	}
	for (int i = 0; i < *xaxis; i++){
		for (int j = 0; j < *yaxis; j++){
			newGrid[i][j] = 0;
		}
	}
	while (fscanf(reader, "%d %d\n", &row, &col) != EOF){
		newGrid[row][col] = 1;
	}
	fclose(reader);
	return newGrid;
}
	else{
		printf("Invalid file. Closing the program.\n");
		exit(0);
	}
}

void stayingAlive(int x, int y, int xaxis, int yaxis, int torus, int ** gridOne, int ** gridTwo){ //Grid checking method
	//topleft neighbor
		int x1, y1, lifeCounter, wraps;
		x1 = x - 1;
		y1 = y - 1;
		wraps = 0;
		lifeCounter = 0;
		if ((x - 1) < 0){
			x1 = xaxis - 1;
			wraps = 1;
		}
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
			gridTwo[x][y] = 0;
		}
		else if ((lifeCounter == 2 || lifeCounter == 3) && gridOne[x][y] == 1){
			gridTwo[x][y] = 1;
			roundCount ++;
		}
		else if (lifeCounter > 3){
			gridTwo[x][y] = 0;
		}
		else if (lifeCounter == 3 && gridOne[x][y] == 0){
			gridTwo[x][y] = 1;
			roundCount ++;
		}
	}
void* CompThread(void* package){ //starter method for worker threads
	struct threadData *unwrap; 
	unwrap = (struct threadData *)package;
	struct threadData info = *unwrap;
	for (int i = 0; i < info.iterations; i++){
		// COMPUTE GRIDS
		for (int j = info.firstRow; j <= info.lastRow; j++){
			for (int k = info.firstCol; k <= info.lastCol; k++){
				stayingAlive(j, k, info.xaxis, info.yaxis, info.torus, info.currGrid, info.nextGrid);
			}
		}
		pthread_barrier_wait(&Bpit);
		if(info.threadTid == 0){
		pthread_mutex_unlock(&printT);
		}
		pthread_barrier_wait(&Total);
	}
	return NULL;
}

void* printGrid(void* package){ //starter method for printer thread
	struct printer *unwrap; 
	unwrap = (struct printer *)package;
	struct printer info = *unwrap;

	for (int i = 0; i < info.iterations; i++){
		pthread_mutex_lock(&printT);

		system("clear");
		for (int l = 0; l < info.xaxis; l++){
			for (int m = 0; m < info.yaxis; m++){
				info.currGrid[l][m] = info.nextGrid[l][m];
				info.nextGrid[l][m] = 0;
			}
		}
		
			if (info.show == 1){
				for (int n = 0; n < info.xaxis; n++){
					for (int o = 0; o < info.yaxis; o++){
						if (info.currGrid[n][o] == 0){
							printf("- ");
						}
						else{
							printf("X ");
						}
					}
					printf("\n");
				}
			printf("Round count: %d \n", roundCount);
			totalCount += roundCount;
			roundCount = 0;

			usleep(info.runspeed);
		}

		pthread_barrier_wait(&Total);

	}
	return NULL;
}

int main(int args, char* arg[]){
	if (args < 6 || args > 7){
		printf("Invalid number of arguments. Try again. \n");
		return 0;
	}

	roundCount =0;
	totalCount =0;
	int xaxis, yaxis, iterations, torus, show, runspeed, orientation;
	time_t pretime, posttime, totaltime;
	/*pretime =0;
	posttime =0;
	endtime =0;
	pretimeu =0;
	posttimeu = 0;*/

	if (strcmp("wrap", arg[4]) == 0){
		torus = 1;
	}
	else if (strcmp("nowrap", arg[4]) == 0){
		torus = 0;
	}
	else{
		printf("Invalid value for the 'wrap' argument. Try again.\n");
		return 0;
	}
	if (strcmp("show", arg[5]) == 0){
		show = 1;
	}
	else if (strcmp("hide", arg[5]) == 0){
		show = 0;
	}
	else{
		printf("Invalid value for the 'show' argument. Try again.\n");
		return 0;
	}
	if (show == 1 && args == 7){
		if (strcmp("slow", arg[6]) == 0){
			runspeed = 333333;
		}
		else if (strcmp("med", arg[6]) == 0){
			runspeed = 100000;
		}
		else if (strcmp("fast", arg[6]) == 0){
			runspeed = 33333;
		}
		else{
			printf("Your 7th argument is invalid despite choosing the 'show' argument. Try again.\n");
			return 0;
		}
	}

	int threads = atoi(arg[2]);
	if (strcmp("row", arg[3]) == 0){
			orientation = 0;
		}
	else if (strcmp("col", arg[3]) == 0){
			orientation = 1;
		}
	int** currGrid = NULL;
	int** nextGrid = NULL;
	currGrid = readFile(arg[1], currGrid, &xaxis, &yaxis, &iterations);
	nextGrid = malloc(xaxis * sizeof(int*));
	for (int i = 0; i < xaxis; i++){
		nextGrid[i] = malloc(yaxis * sizeof(int));
	}
	for (int i = 0; i < xaxis; i++){
		for (int j = 0; j < yaxis; j++){
			nextGrid[i][j] = 0;
		}
	}

	/*struct timeval timepre;
	struct timeval timepost;
	clock_gettime(CLOCK_REALTIME ,&timepre); //get the initial time prior to the loop beginning
	pretime = timepre.tv_sec; //store the time in seconds
	pretimeu = timepre.tv_usec; //store the time in nanoseconds */
	time(&pretime);
	struct threadData* threadArray = malloc(threads * sizeof(struct threadData));
	/////////////
	if (orientation == 0){ // row orientaion
		int lopsidedNum = yaxis % threads;
		int divisor = yaxis/threads;
		int minRow = -1;
		int maxRow = -1;
		for (int i = 0; i < threads; i++){
			
			threadArray[i].threadTid = i;
			if (i < lopsidedNum){
				minRow = maxRow + 1;
				maxRow += divisor + 1;
				threadArray[i].totalRow = divisor + 1;
			}
			else{
				minRow = maxRow + 1;
				maxRow += divisor;
				threadArray[i].totalRow = divisor;
			}
			threadArray[i].totalCol = xaxis;
			threadArray[i].firstRow = minRow;
			threadArray[i].lastRow = maxRow;
			threadArray[i].firstCol = 0;
			threadArray[i].lastCol = xaxis - 1;

		}
	}
	else if (orientation == 1){ // column orientaion
		int lopsidedNum = xaxis % threads;
		int divisor = xaxis/threads;
		int minCol = -1;
		int maxCol = -1;
		for (int i = 0; i < threads; i++){
			
			threadArray[i].threadTid = i;
			if (i < lopsidedNum){
				minCol = maxCol + 1;
				maxCol += divisor + 1;
				threadArray[i].totalCol = divisor + 1;
			}
			else{
				minCol = maxCol + 1;
				maxCol += divisor;
				threadArray[i].totalCol = divisor;
			}
			threadArray[i].totalRow = xaxis;
			threadArray[i].firstCol = minCol;
			threadArray[i].lastCol = maxCol;
			threadArray[i].firstRow = 0;
			threadArray[i].lastRow = xaxis - 1;

		}
	}
	/////////////
	for (int i = 0; i < threads; i++){
		printf("curr: %p, next: %p\n", currGrid, nextGrid);
		threadArray[i].xaxis = xaxis;
		threadArray[i].yaxis = yaxis;
		threadArray[i].torus = torus;
		threadArray[i].currGrid = currGrid;
		threadArray[i].nextGrid = nextGrid;
		threadArray[i].iterations = iterations;

		}
		pthread_barrier_init((&Bpit),NULL,threads); //creation of first barrier
		pthread_barrier_init((&Total),NULL,(threads+1)); //creation of second barrier
		pthread_mutex_init((&printT),NULL); //creation of mutex for printing
		pthread_mutex_lock(&printT); //locking of print mutex
		pthread_t* tid = malloc(iterations * sizeof(pthread_t)); //creation of array for worker thread IDs
		pthread_t printerT; //printer thread ID
		void* package; //struct pointer
		for (int i = 0; i < threads; i++){
			package = &threadArray[i];
			pthread_create((&tid[i]),NULL,CompThread, package); //pthread creation
			package = NULL;
		}
		struct printer print;
		print.xaxis = xaxis;
		print.yaxis = yaxis;
		print.show = show;
		print.iterations = iterations;
		print.currGrid = currGrid;
		print.nextGrid = nextGrid;
		print.runspeed = runspeed;
		package = &print;
		pthread_create((&printerT),NULL,printGrid,package); //printer pthread creation

	for(int i = 0; i < threads; i++){
		pthread_join(tid[i], NULL); //waiting for worker threads
	}
	pthread_join(printerT, NULL); //waiting for printer thread
	system("clear");
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
	printf("#%2d threads, %s-wise partitioning\n", threads, arg[3]);
	for (int i = 0; i < threads; i++){
		printf("tid%2d :rows: %2d:%2d (%2d) cols: %2d:%2d (%2d)\n",threadArray[i].threadTid,threadArray[i].firstRow,threadArray[i].lastRow,threadArray[i].totalRow,threadArray[i].firstCol,threadArray[i].lastCol,threadArray[i].totalCol);
	}
	printf("Total number of cells %d\n",totalCount);
	//memory cleanup
	for (int i = 0; i < xaxis; i++){
		free(currGrid[i]);
	}
	free(currGrid);
	for (int i = 0; i < xaxis; i++){
		free(nextGrid[i]);
	}
	free(nextGrid);
	free(tid);
	free(threadArray);
	pthread_barrier_destroy(&Bpit);
	pthread_barrier_destroy(&Total);
	pthread_mutex_unlock(&printT);
	pthread_mutex_destroy(&printT);

	//gettimeofday(&timepost, NULL); //get the time at the end of the game
	time(&posttime);
	totaltime = posttime - pretime;
	/*posttime = timepost.tv_sec;
	posttimeu = timepost.tv_usec;
	printf("%f,%f,%f,%f,%f\n",pretime,(pretimeu/1000000.0f) ,posttime,(posttimeu/1000000.0f), posttime - pretime);*/
	printf("Total time to do %d iterations of %dx%d is %ld seconds.\n", iterations, xaxis, yaxis, totaltime); //gettimeofday is interrupted by system time, we cannot get an accurate value, this is the best we can do
	return 0;
	}