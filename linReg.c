// Samuel Havron <sgh7cc@virginia.edu>
// https://www.github.com/samuelhavron
// MIT License

/* KNOWN FILE FORMAT BREAKING BUGS:
   - one line contains only an x data point (more than one line with one point fixed)
   - line contains more than two data points
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#define MAX_DATA 100
#define COLS 2

// forward declarations
struct dataSet *load_data(FILE *inputFile);
double *linear_regression(struct dataSet *theData);
double sqr(double x);
double sum(double a[MAX_DATA], double b[MAX_DATA], int opCode, int n);
void dataSet_clear(struct dataSet *theData);

struct dataSet {
  double x[MAX_DATA]; // independent points
  double y[MAX_DATA]; // dependent points
  int n; // number of data points
};

// Initialize the dataSet and check for input errors
struct dataSet *dataSet_init(double x[MAX_DATA], double y[MAX_DATA], int n) {

  struct dataSet *theData = malloc(sizeof(struct dataSet));
  assert(theData != NULL);
 
  // Put the int arrays into the struct
  int i;
  for (i = 0; i < n; i++) {
    *(theData->x + i) = x[i];
    *(theData->y + i) = y[i];
  }
 
  theData->n = n;

  return theData;
}

// Free memory when computation is finished
void dataSet_clear(struct dataSet *theData) {
  assert(theData != NULL);
  if(theData != NULL) free(theData);
}
 
/* Parses data file into x and y
   FILE STRUCTURE IS AS FOLLOWS
   x[0] y[0]
   x[1] y[1]
   :    :
   x[n] y[n]
   x values are in sequential order
*/
struct dataSet *load_data(FILE *inputFile) {
  int i, j;
  int nx = 0, ny = 0;
  double Xval, Yval;
  double x[MAX_DATA];
  double y[MAX_DATA];

  for (j = 0; j < COLS - 1; j++) { // for each column (x and y)
    for (i = 0; !feof(inputFile); i++) { // for each row
      if (j == 0) { // x array
	fscanf(inputFile, "%lf %lf\n", &Xval, &Yval);
	if (i > 0 && x[i-1] >= Xval) {
	  printf("ERROR: Data points must be in successive order of independent (x) values.\n");
	  printf("x value (%lf) is < or = to previous x value (%lf).\n", Xval, x[i-1]);
	  exit(1);
	}
	
        x[i] = Xval;
	nx += 1;
	if(nx > MAX_DATA) {
	  printf("ERROR: Data points cannot exceed %d.\n", MAX_DATA);
	  exit(1);
	}

	if (feof(inputFile)) {
	  clearerr(inputFile);
	  rewind(inputFile);
	  j = 1; // move on to y array
	  i = -1; // index will reset to 0 with for loop increment
	} // end of x array
      } else if (j == 1) { // y array
	fscanf(inputFile, "%lf %lf\n", &Xval, &Yval); 
	y[i] = Yval;
	ny += 1;
	if(ny > MAX_DATA) {
	  printf("ERROR: Data points cannot exceed %d.\n", MAX_DATA);
	  exit(1);
	}

      } // end of y array
    }
  }

  //!TODO! Detect lone x data point: when y is 0 for a given line

  // Check the amount of line numbers against the number of data points
  clearerr(inputFile);
  rewind(inputFile);
  char *line = NULL;
  size_t len = 0;
  ssize_t read;
  int lines = 0;
  while((read = getline(&line, &len, inputFile)) != -1) {
      lines += 1;
  }
  free(line);
  
  // Error detection
  if (nx != ny) {
    printf("ERROR: Unequal amount of x (%d) and y (%d) data points.\n", nx, ny);
    exit(1);
  } else if (nx == 0) {
    printf("ERROR: No data points found. Check data format.\n");
    exit(1);
  } else if (nx != lines) {
    printf("ERROR: Data format incorrect.");
    printf(" %d lines detected and %d data points found.\n", lines, nx);
    printf("Check data format.\n");
    exit(1);
  }
  
  printf("Loading %d data points ...\n", nx);
  // Put input into a DataSet struct  
  return dataSet_init(x, y, nx);
}

double *linear_regression(struct dataSet *theData) {

  double b; // y-intercept of best fit line
  double m; // slope of best fit line
  double r; // correlation coefficient

  int n = theData->n;
  // two array inputs given to sum() to avoid NULL error
  double sumx = sum(theData->x, theData->x, 0, n); // sum of x, not squared
  double sumx2 = sum(theData->x, theData->x, 1, n); // sum of x, each x sqaured
  double sumy = sum(theData->y, theData->y, 0, n); //sum of y
  double sumy2 = sum(theData->y, theData->y, 1, n); // sum of y**2
  double sumxy = sum(theData->x, theData->y, 2, n); // sum of x * y

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
  printf("=================\n\n");

  // Check for a filename
  if (argc == 2) {
    FILE *input = fopen(argv[1], "r");
    
    // Handle file not found error
    if (input == NULL) {
      perror(argv[1]);
      exit(1);

    } else {
      struct dataSet *theData = load_data(input);
      fclose(input);
     
      if (theData != NULL) {
	double *linReg = linear_regression(theData);
	printf("\nSlope   \tm = %15.6e\n", *linReg); // print slope
	printf("y-intercept\tb = %15.6e\n",*(linReg+1)); // print y-intercept
	printf("Correlation\tr = %15.6e\n", *(linReg+2)); // print correlation
	dataSet_clear(theData); // free struct
	if(linReg != NULL) free(linReg);
      } else {
	printf("ERROR: The data did not load (NULL).\n");
	exit(1);
      }
    }

  } else {
    printf("ERROR: Must enter filename after ./linReg\n");

  }
  return 0;
}

double sqr(double x) {
  return x * x;
}

// opCodes: sum of a[] (0), sum of sqr(a[]) (1),
// sum of a[] * b[] (2)
double sum(double a[MAX_DATA], double b[MAX_DATA], int opCode, int n) {
  double sum = 0;
  int i = 0;

  if (opCode == 0) {
    for (i = 0; i < n; i++) {
      sum += a[i];
    }

  } else if (opCode == 1) {
    for (i = 0; i < n; i++) {
      sum += sqr(a[i]);
    }

  } else if (opCode == 2) {
    for (i = 0; i < n; i++) {
      sum += a[i] * b[i]; // a[] and b[] can only be the same size before this function is called
    }

  }
  return sum;
}
