/**
 Keith Alexander & Julian De La Cruz
 CSC345-01
 Project 2
 */

#include <pthread.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define numbers_in_threads 27
#define MAX_LEN 80
#define numbers_in_threads2 11

int valid[numbers_in_threads] = {0};

/**This stores all the data that
    will be later on passed into threads */
typedef struct {
	int row;
	int column;
} parameters;

/** This is a 2x2 integer array that will contain
    the numbers corresponding to the sudoku puzzle
    in the input file. */
int sudoku[9][9] = {0};

//This function checks the columns and ensures they contain numbers 1-9
void *checkColumnValidity(void* param){
    parameters *params = (parameters*) param;
    int column = params->column;
   

    int checkIfValid[9] = {0};
    int i;
    int j;
    //for loop increments through columns and checks the numbers
    for(i = 0; i < 9; i++){
        for(j = 0; j < 9; j++){
            int num = sudoku[j][i];
            if (num < 1 || num > 9 || checkIfValid[num - 1] == 1) {
			    pthread_exit(NULL);
    		} else {
	    		checkIfValid[num - 1] = 1;
            }
        }
        memset(checkIfValid, 0, 9* sizeof(checkIfValid[0]));
    }
    valid[9] = 1;
    pthread_exit(NULL);
}

//This functions checks the rows and ensures they contain numbers 1-9
void *checkRowValidity(void* param){
    parameters *params = (parameters*) param;
    int row = params->row;
    int checkIfValid[9] = {0};
    int i;
    int j;
    //for loop increments through the rows and checks the numbers
    for(i = 0; i < 9; i++){
        for(j = 0; j < 9; j++){
            int num = sudoku[i][j];
            if (num < 1 || num > 9 || checkIfValid[num - 1] == 1) {
			    pthread_exit(NULL);
    		} else {
	    		checkIfValid[num - 1] = 1;
	    	}
        }
        memset(checkIfValid, 0, 9* sizeof(checkIfValid[0]));
    }
    valid[10] = 1;
    pthread_exit(NULL);
}

// This function checks to make sure there are the correct number of columns
void *columnValidityCheck(void* param) {
	parameters *params = (parameters*) param;
	int col = params->column;
    int row = params->row;
	if (row != 0 || col > 8) {
        fprintf(stderr, "There is an invalid row/column within the column section: row=%d, col=%d\n", row, col);
		pthread_exit(NULL);
	}
    
	int validityArray[9] = {0};
	int i;
    //Checks the numbers
	for (i = 0; i < 9; i++) {
		int num = sudoku[i][col];
		if (num < 1 || num > 9 || validityArray[num - 1] == 1) {
			pthread_exit(NULL);
		} else {
			validityArray[num - 1] = 1;
		}
	}
	valid[18 + col] = 1;
	pthread_exit(NULL);
}

//Function checks to make sure there are the correct number of rows
void *rowValidityCheck(void* param) {
	parameters *params = (parameters*) param;
	int col = params->column;
    int row = params->row;
	if (col != 0 || row > 8) {
        fprintf(stderr, "There is an invalid row/column within the row section: row=%d, col=%d\n", row, col);
		pthread_exit(NULL);
	}

	int validityArray[9] = {0};
	int i;
	for (i = 0; i < 9; i++) {
		int num = sudoku[row][i];
		if (num < 1 || num > 9 || validityArray[num - 1] == 1) {
			pthread_exit(NULL);
		} else {
			validityArray[num - 1] = 1;
		}
	}
	valid[9 + row] = 1;
	pthread_exit(NULL);
}

/** This function will check to make sure that 1-9 appear once each
    in each given 3x3 section.
 */
void *checkValidity3x3(void* param) {
	parameters *params = (parameters*) param;
	int row = params->row;
	int col = params->column;
	if (row > 6 || row % 3 != 0 || col > 6 || col % 3 != 0) {
		fprintf(stderr, "One of the rows or columns is incorrect. row=%d, col=%d\n", row, col);
		pthread_exit(NULL);
	}
	int validityArray[9] = {0};
	int i, j;
	for (i = row; i < row + 3; i++) {
		for (j = col; j < col + 3; j++) {
			int num = sudoku[i][j];
			if (num < 1 || num > 9 || validityArray[num - 1] == 1) {
				pthread_exit(NULL);
			} else {
				validityArray[num - 1] = 1;
			}
		}
	}

	valid[row + col/3] = 1;
	pthread_exit(NULL);
}

//main function where the actual requirements are run
int main(int argc, char *argv[]) {
    
    //user input from command line. Will be used for 1 and 2 as per project instructions
    int userInputNum = atoi(argv[1]);
   
    clock_t begin = clock();

    FILE *file;
    file = fopen("input.txt", "r");  /* Opens the text file containing our sudoku game */

    //Stores the numbers from the input file and prints them to the screen
    for(int m=0; m < 9; m++){
        for(int n = 0; n < 9; n++)
            {
                fscanf(file, "%d" ,&sudoku[m][n]);
                printf("%d ", sudoku[m][n]);            

            }
            printf("\n");
    }
    
    /** This code runs if the user inputs a 2. This creates multiple threads to check
     each 3x3 game, each column, and each row.
     */
    if (userInputNum == 2){
	    pthread_t threads[numbers_in_threads];

	    int threadPlace = 0;
	    int i,j;
        
        /**This creates 27 threads. 9 of these are for each 3x3 game.
         Another 9 are for the 9 columns.
         The last set of 9 threads is for the 9 rows. */
	    for (i = 0; i < 9; i++) {
		    for (j = 0; j < 9; j++) {
			    if(i%3 == 0 && j%3 == 0) {
                    //Creates the threads for the 3x3 games
				    parameters *data = (parameters *) malloc(sizeof(parameters));
				    data->row = i;
				    data->column = j;
				    pthread_create(&threads[threadPlace++], NULL, checkValidity3x3, data);
			    }
                if(i == 0){
                    //Creates the threads for the columns
				    parameters *infoInColumn = (parameters *) malloc(sizeof(parameters));
				    infoInColumn->row = i;
				    infoInColumn->column = j;
				    pthread_create(&threads[threadPlace++], NULL, columnValidityCheck, infoInColumn);
			    }
			    if(j == 0){
                    //Creates the threads for the rows
				    parameters *infoInRow = (parameters *) malloc(sizeof(parameters));
				    infoInRow->row = i;
				    infoInRow->column = j;
				    pthread_create(&threads[threadPlace++], NULL, rowValidityCheck, infoInRow);
			    }
		    }
        }

        
	    for (i = 0; i < numbers_in_threads; i++) {
		    pthread_join(threads[i], NULL);
    	}

		begin = clock() - begin;
		double time_taken = ((double)begin)/CLOCKS_PER_SEC;
        
        /*8This checks every value in the array to see if there are any 0s.
         If there are, then the sudoku board does not have a valid solution. */
	    for (i = 0; i < numbers_in_threads; i++) {
    		if (valid[i] == 0) {
                printf("SOLUTION: NO. ( %f seconds )\n" , time_taken);
			    return EXIT_SUCCESS;
		    }
	    }
        printf("SOLUTION: YES ( %f seconds )\n" , time_taken);
	    return EXIT_SUCCESS;
	}
    
    /** This code runs if the user input a 1. Checks if the sudoku board in the input file
        is a valid solution. */
    if (userInputNum == 1){
        pthread_t threads2[numbers_in_threads2];

	    int threadPlace2 = 0;
	
    	int y,z;
        for (y = 0; y < 9; y++) {
	    	for (z = 0; z < 9; z++) {
                if(y%3 == 0 && z%3 == 0) {
			        parameters *data = (parameters *) malloc(sizeof(parameters));
				    data->row = y;
				    data->column = z;
				    pthread_create(&threads2[threadPlace2++], NULL, checkValidity3x3, data);
                }
            }
		}

        parameters *infoInColumn = (parameters *) malloc(sizeof(parameters));
		infoInColumn->row = 0;
		infoInColumn->column = 0;
		pthread_create(&threads2[threadPlace2++], NULL, checkColumnValidity, infoInColumn);

        parameters *infoInRow = (parameters *) malloc(sizeof(parameters));
		infoInRow->row = 0;
		infoInRow->column = 0;
	    pthread_create(&threads2[threadPlace2++], NULL, checkRowValidity, infoInRow);


        for (y = 0; y < numbers_in_threads2; y++) {
		    pthread_join(threads2[y], NULL);
    	}

		begin = clock() - begin;
		double time_taken = ((double)begin)/CLOCKS_PER_SEC;
        
	    /** This checks every value in the array to see if there are any 0s.
        If there are, then the sudoku board does not have a valid solution */
        int v;
	    for (v = 0; v < numbers_in_threads2; v++) {
		    if (valid[v] == 0) {
			    printf("SOLUTION: NO. ( %f seconds )\n" ,time_taken);
			    return EXIT_SUCCESS;
    		}
    	}
    	printf("SOLUTION: YES. ( %f seconds )\n" , time_taken);
    	return EXIT_SUCCESS;
	}
    
  return 0;
}
