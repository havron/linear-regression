// Samuel Havron <sgh7cc@virginia.edu>
// https://www.github.com/samuelhavron
// MIT License
//!! WARNING: load_data function NOT COMPLETE, does not compile

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#define MAX_DATA 100

// forward declarations
struct dataSet *load_data(FILE *inputFile);
double *linear_regression(struct dataSet *theData);
double sqr(double x);
double sum(double a[MAX_DATA],  double b[MAX_DATA], int opCode);
void dataSet_clear(struct dataSet *theData);


struct dataSet {
  double x[MAX_DATA]; // independent points
  double y[MAX_DATA]; // dependent points
  int n; // number of data points
};

// Initialize the dataSet and check for input errors
struct dataSet *dataSet_init(double x[MAX_DATA], double y[MAX_DATA]) {

  //!! Compiling error - why?
  struct dataSet *theData = malloc(sizeof(struct dataSet));
  assert(theData != NULL);
  
  // Obtain the length of x and y arrays
  int sizeX = sizeof(x) / sizeof(double);
  int sizeY = sizeof(y) / sizeof(double);

  // Check that every point x has a corresponding point y
  if (sizeX != sizeY) {
    printf("ERROR: unequal amount of x (%d) and y (%d) data points.\n", sizeX, sizeY);
    dataSet_clear(theData);
    return NULL;
  }
  
  // Put the int arrays into the struct
  memcpy(theData->x, x, sizeof(x));
  memcpy(theData->y, y, sizeof(y));
  theData->n = sizeX;

  return theData;
}

 // Free memory when computation is finished
void dataSet_clear(struct dataSet *theData) {
    assert(theData != NULL);
    free(theData->x);
    free(theData->y);
    free(theData);
}
 
 /* Parses data file into x and y
    FILE STRUCTURE IS AS FOLLOWS
    x[0],x[1], ... ,x[n];y[0],y[1],...,y[n]
    x and y  data points separated by commas (no spaces)
    semicolon (;) to mark end of x points and beginning of y points
 */
//!! WARNING: FUNCTION INCOMPLETE
struct dataSet *load_data(FILE *inputFile) {
  
  //!! Convert file data to a string? Does this work?
  int ch;
  char inputString[] = "";
  while ((ch = fgetc(inputFile)) != EOF) {
    strcat(inputString, (char*) fgetc(inputFile)); //!! compiling error from casting
  }
  
  // Split the input stream into x and y comma separated values
  char *xCSV = strtok(inputString, ";");
  char *yCSV = strtok(NULL, ";");
       	
  //!! How to extract x and y arrays from xCSV and yCSV? 
  //!! strtok does not accept char pointer
  
  double x[MAX_DATA] = {0}; //!! Placeholder
  double y[MAX_DATA] = {0}; //!! Placeholder

  // Put input into a DataSet struct
  if (dataSet_init(x, y)) { // check for proper initialiation   
    return dataSet_init(x, y);
	      	      
  } else {
    printf("The data could not be loaded.\n");
    return NULL;
  }
}

double *linear_regression(struct dataSet *theData) {

 double b; // y-intercept of best fit line
 double m; // slope of best fit line
 double r; // correlation coefficient

 int n = theData->n;
 // two array inputs given to sum() to avoid NULL error
 double sumx = sum(theData->x, theData->x, 0); // sum of x, not squared
 double sumx2 = sum(theData->x, theData->x, 1); // sum of x, each x sqaured
 double sumy = sum(theData->y, theData->y, 0); //sum of y
 double sumy2 = sum(theData->y, theData->y, 1); // sum of y**2
 double sumxy = sum(theData->x, theData->y, 2); // sum of x * y

 // Compute least-sqaures best fit straight line.
 m = (n * sumxy - sumx * sumy) / (n * sumx2 - sqr(sumx)); // compute slope
 b = (sumy * sumx2 - sumx * sumxy) / (n * sumx2 - sqr(sumx));
 r = (sumxy - sumx * sumy / n) / sqrt((sumx2 - sqr(sumx) / n) * (sumy2 - sqr(sumy) / n));

 double *result = malloc(sizeof(double) * 3);
 *result = m;
 *(result + 1) = b;
 *(result + 2) = r;
 return result;
}

int main(int argc, char *argv[]) {
  printf("Linear Regression\n");

  // Check for a filename
  if (argc == 2) {
    FILE *input = fopen(argv[1], "r");
    
    // Handle file not found error
    if (input == NULL) {
      perror(argv[1]);

    } else {
      struct dataSet *theData = load_data(input);
      
      if (theData != NULL) {
	double *linReg = linear_regression(theData);
	printf("\nSlope   \tm = %15.6e\n", *linReg); // print slope
	printf("y-intercept\tb = %15.6e\n",*(linReg+1)); // print y-intercept
	printf("Correlation\tr = %15.6e\n", *(linReg+2)); // print correlation
	dataSet_clear(theData); // free struct
	free(linReg);
      }
    }

  } else {
    printf("ERROR: Must enter filename after ./linReg\n");

  }
  return 0;
}


//! should have some testing code


double sqr(double x) {
    return x * x;
}

// opCodes: sum of a[] (0), sum of sqr(a[]) (1),
// sum of a[] * b[] (2)
double sum(double a[MAX_DATA], double b[MAX_DATA], int opCode) {
  double sum = 0;
  int i = 0;

  if (opCode == 0) {
    for (i = 0; i < (sizeof(a) / sizeof(double)); i++) {
      sum += a[i];
    }

  } else if (opCode == 1) {
    for (i = 0; i < (sizeof(a) / sizeof(double)); i++) {
      sum += sqr(a[i]);
    }

  } else if (opCode == 2) {
     for (i = 0; i < (sizeof(a) / sizeof(double)); i++) {
       sum += a[i] * b[i]; // a[] and b[] can only be the same size before this function is called
    }

  }
  return sum;
}
