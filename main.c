//prject 2 v2
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#define num_threads 27
#define num_threads2 11
#define MAX_LEN 80
#define FIRST 0
#define LAST 8
#define TRUE 1
#define FALSE 0


/*
	Initialize the array which worker threads can update to 1 if the
	corresponding region ofnum_thread the sudoku puzzle they were responsible
 	for is valid.
*/


int valid[num_threads] = {0};
double totaltime = 0;


// Struct that stores the data to be passed to threads
typedef struct {
	int row;
	int column;
} parameters;

// Sudoku puzzle to be solved
int sudoku[9][9] = {0};

void *isColumValid11(void* param){
    parameters *params = (parameters*) param;
    int column = params->column;
   

    int validityArray2[9] = {0};
    int i;
    int j;
    for(i = 0; i < 9; i++){
        for(j = 0; j < 9; j++){
            int num = sudoku[j][i];
            if (num < 1 || num > 9 || validityArray2[num - 1] == 1) {
			    pthread_exit(NULL);
    		} else {
	    		validityArray2[num - 1] = 1;
            }
        }
        memset(validityArray2, 0, 9* sizeof(validityArray2[0]));   
    }
    valid[9] = 1;
    pthread_exit(NULL);
}

void *isRowValid11(void* param){
    parameters *params = (parameters*) param;

    int row = params->row;
    

    int validityArray2[9] = {0};
    int i;
    int j;
    for(i = 0; i < 9; i++){
        for(j = 0; j < 9; j++){
            int num = sudoku[i][j];
            if (num < 1 || num > 9 || validityArray2[num - 1] == 1) {
			    pthread_exit(NULL);
    		} else {
	    		validityArray2[num - 1] = 1;
	    	}
        }
        memset(validityArray2, 0, 9* sizeof(validityArray2[0]));
    }
    valid[10] = 1;
    pthread_exit(NULL);
}

// Method that determines if numbers 1-9 only appear once in a column
void *isColumnValid(void* param) {
	// Confirm that parameters indicate a valid col subsection
	parameters *params = (parameters*) param;
	int row = params->row;
	int col = params->column;
	if (row != 0 || col > 8) {
		fprintf(stderr, "Invalid row or column for col subsection! row=%d, col=%d\n", row, col);
		pthread_exit(NULL);
	}

	// Check if numbers 1-9 only appear once in the column
	int validityArray[9] = {0};
	int i;
	for (i = 0; i < 9; i++) {
		int num = sudoku[i][col];
		if (num < 1 || num > 9 || validityArray[num - 1] == 1) {
			pthread_exit(NULL);
		} else {
			validityArray[num - 1] = 1;
		}
	}
	// If reached this point, col subsection is valid.
	valid[18 + col] = 1;
	pthread_exit(NULL);
}

// Method that determines if numbers 1-9 only appear once in a row
void *isRowValid(void* param) {
	// Confirm that parameters indicate a valid row subsection
	parameters *params = (parameters*) param;
	int row = params->row;
	int col = params->column;
	if (col != 0 || row > 8) {
		fprintf(stderr, "Invalid row or column for row subsection! row=%d, col=%d\n", row, col);
		pthread_exit(NULL);
	}

	// Check if numbers 1-9 only appear once in the row
	int validityArray[9] = {0};
	int i;
	for (i = 0; i < 9; i++) {
		// If the corresponding index for the number is set to 1, and the number is encountered again,
		// the valid array will not be updated and the thread will exit.
		int num = sudoku[row][i];
		if (num < 1 || num > 9 || validityArray[num - 1] == 1) {
			pthread_exit(NULL);
		} else {
			validityArray[num - 1] = 1;
		}
	}
	// If reached this point, row subsection is valid.
	valid[9 + row] = 1;
	pthread_exit(NULL);
}

// Method that determines if numbers 1-9 only appear once in a 3x3 subsection
void *is3x3Valid(void* param) {
	// Confirm that parameters indicate a valid 3x3 subsection
	parameters *params = (parameters*) param;
	int row = params->row;
	int col = params->column;
	if (row > 6 || row % 3 != 0 || col > 6 || col % 3 != 0) {
		fprintf(stderr, "Invalid row or column for subsection! row=%d, col=%d\n", row, col);
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
	// If reached this point, 3x3 subsection is valid.
	valid[row + col/3] = 1; // Maps the subsection to an index in the first 8 indices of the valid array
	pthread_exit(NULL);
}


int main(int argc, char *argv[]) {
    
    int a_word = atoi(argv[1]);
    
    

    FILE *file;  /* declare a FILE pointer  */
    file = fopen("input.txt", "r");  /* open a text file for reading */

    for(int m=0; m < 9; m++){

        for(int n = 0; n < 9; n++)
            {
                fscanf(file, "%d" ,&sudoku[m][n]);
                printf("%d ", sudoku[m][n]);            

            }
            printf("\n");
    }
    if (a_word == 2){
	    pthread_t threads[num_threads];

	    int threadIndex = 0;
	    int i,j;
	// Create 9 threads for 9 3x3 subsections, 9 threads for 9 columns and 9 threads for 9 rows.
	// This will end up with a total of 27 threads.
	    for (i = 0; i < 9; i++) {
		    for (j = 0; j < 9; j++) {
			    if(i%3 == 0 && j%3 == 0) {
				    parameters *data = (parameters *) malloc(sizeof(parameters));
				    data->row = i;
				    data->column = j;
				    pthread_create(&threads[threadIndex++], NULL, is3x3Valid, data); // 3x3 subsection threads
			    }
                if(i == 0){
				    parameters *columnData = (parameters *) malloc(sizeof(parameters));
				    columnData->row = i;
				    columnData->column = j;
				    pthread_create(&threads[threadIndex++], NULL, isColumnValid, columnData);	// column threads
			    }
			    if(j == 0){
				    parameters *rowData = (parameters *) malloc(sizeof(parameters));
				    rowData->row = i;
				    rowData->column = j;
				    pthread_create(&threads[threadIndex++], NULL, isRowValid, rowData); // row threads
			    }
		    }
        }

        //27 thread check
	    for (i = 0; i < num_threads; i++) {
		    pthread_join(threads[i], NULL);			// Wait for all threads to finish
    	}

	// If any of the entries in the valid array are 0, then the sudoku solution is invalid
	    for (i = 0; i < num_threads; i++) {
    		if (valid[i] == 0) {
		    	printf("Sudoku solution is invalid!\n");
			    return EXIT_SUCCESS;
		    }
	    }
	    printf("Sudoku solution is valid !\n");
	    return EXIT_SUCCESS;
	}
    
    if (a_word == 1){
        pthread_t threads2[num_threads2];

	    int threadIndex2 = 0;
	
    	int k,l;
        for (k = 0; k < 9; k++) {
	    	for (l = 0; l < 9; l++) {
                if(k%3 == 0 && l%3 == 0) {
			        parameters *data = (parameters *) malloc(sizeof(parameters));
				    data->row = k;
				    data->column = l;
				    pthread_create(&threads2[threadIndex2++], NULL, is3x3Valid, data); // 3x3 subsection threads
                }
            }
		}

        parameters *columnData = (parameters *) malloc(sizeof(parameters));
		columnData->row = 0;
		columnData->column = 0;
		pthread_create(&threads2[threadIndex2++], NULL, isColumValid11, columnData);

        parameters *rowData = (parameters *) malloc(sizeof(parameters));
		rowData->row = 0;
		rowData->column = 0;
	    pthread_create(&threads2[threadIndex2++], NULL, isRowValid11, rowData);


        //11 thread check
        for (k = 0; k < num_threads2; k++) {
		    pthread_join(threads2[k], NULL);			// Wait for all threads to finish
    	}

	    // If any of the entries in the valid array are 0, then the sudoku solution is invalid
	    for (k = 0; k < num_threads2; k++) {
		    if (valid[k] == 0) {
			    printf("Sudoku solution is invalid!\n");
			    return EXIT_SUCCESS;
    		}
    	}
    	printf("Sudoku solution is valid !\n");
    	return EXIT_SUCCESS;
	}
    
  return 0;
}
