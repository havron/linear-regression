// Samuel Havron <havron@virginia.edu>
// https://www.github.com/samuelhavron
// MIT License

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define MAX_DATA 100

// Fixed point arithmetic
#define SCALE (1 << 16) // 2^16
#define DESCALE(x) (double) x / SCALE

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
  double a, b;
  for (i = 0; i < MAX_DATA; ++i) {
    int dataPoints = fscanf(inputFile, "%lf %lf", &a, &b);
    data.x[i] = a*SCALE;
    data.y[i] = b*SCALE;
    if (dataPoints != 2) {
      if (dataPoints < 0 && feof(inputFile)) {
	break;
      } else {
	fprintf(stderr, "ERROR: Input does not match file format. Check input file.\n");
	exit(1);
      }
    }
    data.n = i + 1; // adjust for index 0
  }
  printf("Loading %d data points ...\n", data.n);
  return data;
}

LinRegResult linear_regression(DataSet theData) {
  
  LinRegResult result;
  int n = theData.n; // number of data points
  double sumx = DESCALE(sum(theData.x, n)); // sum of x
  double sumxx = DESCALE(dotProd(theData.x, theData.x, n)); // sum of each x squared
  double sumy = DESCALE(sum(theData.y, n)); //sum of y
  double sumyy = DESCALE(dotProd(theData.y, theData.y, n)); // sum of each y squared
  double sumxy = DESCALE(dotProd(theData.x, theData.y, n)); // sum of each x * y
  
  double m, b, r;
  // Compute least-squares best fit straight line
  m = (n * sumxy - sumx * sumy) / (n * sumxx - sqr(sumx)); // slope
  b = (sumy * sumxx - (sumx * sumxy)) / (n * sumxx - sqr(sumx)); // y-intercept
  r = (sumxy - sumx * sumy / n) / sqrt((sumxx - sqr(sumx) / n) * (sumyy - sqr(sumy)/ n)); // correlation

  result.m = m * SCALE;
  result.b = b * SCALE;
  result.r = r * SCALE;
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
      printf("\nSlope   \tm = %15.6e\n", DESCALE(linReg.m)); // print slope
      printf("y-intercept\tb = %15.6e\n", DESCALE(linReg.b)); // print y-intercept
      printf("Correlation\tr = %15.6e\n", DESCALE(linReg.r)); // print correlation
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
  double dotProd = 0;
  int result = 0;
  int i;
  for (i = 0; i < n; i++) {
    dotProd += DESCALE(a[i]) * DESCALE(b[i]);
  }
  result = dotProd * SCALE;
  return result;
}

int sum(int a[MAX_DATA], int n) {
  int sum = 0;
  int i;
  for (i = 0; i < n; i++) {
    sum += a[i];
  }
  return sum;
}
