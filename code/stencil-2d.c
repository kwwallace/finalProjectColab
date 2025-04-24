/* 
 * Author:   Justin LaForge Kyle Wallace
 * 
 * File:     stencil-2d-hybrid.c
 *
 * Purpose:  Perform stencil simulation in serial runtime
 *
 * Run:      ./stencil-2d-hybrid.c -n <num iters> -i <in> -o <out> -v <debug>
 * 
 * 	for Debugging:
 * 		0: does not print anything to the screen other than error messages (this is the
 *		option if -v is not specified)
 *		1: basic debugging information (file sizes, names, etc.). Minimal output
 *  	2: verbose output. Print state of matrix after each iteration, like this:
 *
 * Input:    Binary file with stencil matrix
 * 
 * Output:   Output stencil matrix binary file
 *
 * Errors:   Usage errors and file permission errors
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "utilities.c"
#include <getopt.h>

void usage(char **argv){
    printf("Usage: %s -n <num iters> -i <in file> -o <out file> -d <debug: 0,1,2>\n", argv[0]);
}

// Set arguments
void setArgs(int argc, char **argv, int *n, char **in, char **out, int *debug){
	int opt;

	while((opt = getopt(argc, argv, "n:i:o:v")) != -1){
		switch(opt){
			case 'n':
				*n = atoi(optarg);
				break;
			case 'i':
				*in = optarg;
				break;
			case 'o':
				*out = optarg;
				break;
			case 'v':
				*debug = atoi(optarg);
				break;
			default:
				usage(argv);
				exit(1);
		}
	}
	if(*in == NULL ||*out == NULL){
		perror("Files -i and -o must be provided");
		exit(EXIT_FAILURE);
	}
    
	
}

int main(int argc, char **argv){
    int n=1,debug=0;
    char *in = NULL;
    char *out = NULL;
    
    //set args
    setArgs(argc, argv, &n, &in, &out, &debug);

    double *matrix;
	double *newMatrix;
    int rows, cols;

    newMatrix = malloc(n*n*sizeof(double));
    if (newMatrix == NULL) {
       fprintf(stderr, "Can't allocate storage\n");
       exit(-1);
    }

    Read_matrix(in, &matrix, &rows, &cols);
    // Do something

	newMatrix = matrix;
	double *temp;
	// Loop iterations
	for(int o=0; o<n; o++){
		// Loop rows
		for(int i=1;i<rows-1;i++){
			//Loop Cols
			for(int j=1; j<cols-1;j++){
				newMatrix[i * cols + j] = (
					matrix[(i-1) * cols + (j-1)] + matrix[(i-1) * cols + j] + matrix[(i-1) * cols + (j+1)] +
					matrix[i * cols + (j-1)]     + matrix[i * cols + j]     + matrix[i * cols + (j+1)] +
					matrix[(i+1) * cols + (j-1)] + matrix[(i+1) * cols + j] + matrix[(i+1) * cols + (j+1)]
				) / 9.0;
				 
			}
		}
		temp = matrix;
		matrix = newMatrix;
		newMatrix = temp;
	}
    Print_matrix(matrix, rows, cols);

	write_memory_to_file(matrix,rows,cols,out);
    
/*
     // Writing matrix to binary file
     FILE *file = fopen(out, "wb");
     if (!file) {
         fprintf(stderr, "Error: Unable to open file for writing.\n");
		 free(matrix);
		 free(newMatrix);
         return EXIT_FAILURE;
     }
 
     // Write the dimensions
     if (fwrite(&cols, sizeof(int), 1, file) != 1 || fwrite(&cols, sizeof(int), 1, file) != 1) {
         fprintf(stderr, "Error: Failed to write matrix dimensions.\n");
         fclose(file);
		 free(matrix);
		 free(newMatrix);
         return EXIT_FAILURE;
     }
 
     // Write the matrix data
     if (fwrite(matrix, sizeof(double), cols * cols, file) != (size_t)(cols * cols)) {
         fprintf(stderr, "Error: Failed to write matrix data.\n");
         fclose(file);
		 free(matrix);
		 free(newMatrix);
         return EXIT_FAILURE;
     }
 
     fclose(file);
	*/


    free(matrix);
    free(newMatrix);
    return 0;
}