// Samuel Havron <sgh7cc@virginia.edu>
// https://www.github.com/samuelhavron
// MIT License

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#define MAX_DATA 100

// forward declarations
struct DataSet load_data(FILE *inputFile);
struct LinRegResult linear_regression(struct DataSet theData);
double sqr(double x);
double dotProd(double a[MAX_DATA], double b[MAX_DATA], int n);
double sum(double a[MAX_DATA], int n);

struct DataSet {
  double x[MAX_DATA]; // independent points
  double y[MAX_DATA]; // dependent points
  int n; // number of data points
};

struct LinRegResult {
  double m; // slope
  double b; // y-intercept
  double r; // correlation constant
};

struct DataSet load_data(FILE *inputFile) {
  int i;
  struct DataSet data;
  for (i = 0; i < MAX_DATA; ++i) {
    int dataPoints = fscanf(inputFile, "%lf %lf", &data.x[i], &data.y[i]);
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

struct LinRegResult linear_regression(struct DataSet theData) {

  double m; // slope of best fit line
  double b; // y-intercept of best fit line
  double r; // correlation coefficient

  int n = theData.n; // number of data points
  double sumx = sum(theData.x, n); // sum of x
  double sumxx = dotProd(theData.x, theData.x, n); // sum of each x sqaured
  double sumy = sum(theData.y, n); //sum of y
  double sumyy = dotProd(theData.y, theData.y, n); // sum of each y squared
  double sumxy = dotProd(theData.x, theData.y, n); // sum of each x * y

  // Compute least-squares best fit straight line
  m = (n * sumxy - sumx * sumy) / (n * sumxx - sqr(sumx));
  b = (sumy * sumxx - sumx * sumxy) / (n * sumxx - sqr(sumx));
  r = (sumxy - sumx * sumy / n) / sqrt((sumxx - sqr(sumx) / n) * (sumyy - sqr(sumy) / n));

  struct LinRegResult result;
  result.m = m;
  result.b = b;
  result.r = r;
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
      struct DataSet theData = load_data(input);
      fclose(input);
     
      struct LinRegResult linReg = linear_regression(theData);
      printf("\nSlope   \tm = %15.6e\n", linReg.m); // print slope
      printf("y-intercept\tb = %15.6e\n", linReg.b); // print y-intercept
      printf("Correlation\tr = %15.6e\n", linReg.r); // print correlation
    }

  } else {
    printf("ERROR: Must enter filename after ./linReg\n");

  }
  return 0;
}

double sqr(double x) {
  return x * x;
}

double dotProd(double a[MAX_DATA], double b[MAX_DATA], int n) {
  double dotProd = 0;
  int i;
  for (i = 0; i < n; i++) {
    dotProd += a[i] * b[i];
  }
  return dotProd;
}

double sum(double a[MAX_DATA], int n) {
  double sum = 0;
  int i;
  for (i = 0; i < n; i++) {
    sum += a[i];
  }
  return sum;
}
