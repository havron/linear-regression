// Samuel Havron <havron@virginia.edu>
// https://www.github.com/samuelhavron
// MIT License

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define MAX_DATA 100

// Fixed point arithmetic
#define SHIFT_AMOUNT 16 // 2^16 = 65536
#define SHIFT_MASK ((1 << SHIFT_AMOUNT) - 1) // 65535, draw out fraction with bitwise '&'

typedef struct {
  int x[MAX_DATA]; // independent points
  int y[MAX_DATA]; // dependent points
  int n; // number of data points
} DataSet;

typedef struct {
  int m; // slope
  int b; // y-intercept
  int r; // correlation constant
} LinRegResult;

// forward declarations
DataSet load_data(FILE *inputFile);
LinRegResult linear_regression(DataSet theData);
int sqr(int x);
int dotProd(int a[MAX_DATA], int b[MAX_DATA], int n);
int sum(int a[MAX_DATA], int n);

DataSet load_data(FILE *inputFile) {
  int i;
  DataSet data;
  for (i = 0; i < MAX_DATA; ++i) {
    int dataPoints = fscanf(inputFile, "%lf %lf", &data.x[i], &data.y[i]); // How to convert to fixed point arithmetic?
    if (dataPoints != 2) {
      if (dataPoints < 0 && feof(inputFile)) {
	break;
      } else {
	fprintf(stderr, "ERROR: Input does not match file format. Check input file.\n");
	exit(1);
      }
    }
    data.n = i;
  }
  printf("Loading %d data points ...\n", data.n);
  return data;
}

LinRegResult linear_regression(DataSet theData) {

  LinRegResult result;
  int n = theData.n; // number of data points
  int sumx = sum(theData.x, n); // sum of x
  int sumxx = dotProd(theData.x, theData.x, n); // sum of each x squared
  int sumy = sum(theData.y, n); //sum of y
  int sumyy = dotProd(theData.y, theData.y, n); // sum of each y squared
  int sumxy = dotProd(theData.x, theData.y, n); // sum of each x * y

  // Compute least-squares best fit straight line
  result.m = ((n * sumxy - sumx * sumy) << SHIFT_AMOUNT) / ((n * sumxx - sqr(sumx)) << SHIFT_AMOUNT); // slope
  result.b = ((sumy * sumxx - sumx * sumxy) << SHIFT_AMOUNT) / ((n * sumxx - sqr(sumx)) << SHIFT_AMOUNT); // y-intercept
  result.r = ((sumxy - sumx * sumy) << SHIFT_AMOUNT / n) / sqrt(((sumxx - sqr(sumx)) << SHIFT_AMOUNT / n) * ((sumyy - sqr(sumy)) << SHIFT_AMOUNT / n)); // correlation

  return result;
}

int main(int argc, char *argv[]) {
  printf("Linear Regression\n");
  printf("=================\n\n");

  // Check for a filename
  if (argc == 2) {
    FILE *input = fopen(argv[1], "r");
    
    if (input == NULL) {
      perror(argv[1]);
      exit(1);

    } else {
      DataSet theData = load_data(input);
      fclose(input);
     
      LinRegResult linReg = linear_regression(theData);
      printf("\nSlope   \tm = %d\n", linReg.m >> SHIFT_AMOUNT); // print slope
      printf("y-intercept\tb = %d\n", linReg.b >> SHIFT_AMOUNT); // print y-intercept
      printf("Correlation\tr = %d\n", linReg.r >> SHIFT_AMOUNT); // print correlation
    }

  } else {
    printf("ERROR: Must enter filename after ./linReg\n");

  }
  return 0;
}

int sqr(int x) {
  return x * x;
}

int dotProd(int a[MAX_DATA], int b[MAX_DATA], int n) {
  int dotProd = 0;
  int i;
  for (i = 0; i < n; i++) {
    dotProd += (a[i] * b[i]) << SHIFT_AMOUNT;
  }
  return dotProd;
}

int sum(int a[MAX_DATA], int n) {
  int sum = 0;
  int i;
  for (i = 0; i < n; i++) {
    sum += a[i] << SHIFT_AMOUNT;
  }
  return sum;
}
