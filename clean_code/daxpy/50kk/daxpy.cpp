#include <stdlib.h>
#include <stdio.h>

void daxpy(int n, double a, double *x, double *y) {
    for (int i = 0; i < n; ++i)
        y[i] = a * x[i] + y[i];
}

int main(int argc, const char* argv[]) {
    static int n = 50000000; static double a = 2.0; //n = 100000000;
    double *x = (double *) malloc(n * sizeof(double));
    double *y = (double *) malloc(n * sizeof(double));
    // Initialize x, y 
    for(int i = 0; i < n; ++i){
        x[i] = 1.0;
        y[i] = 2.0;
    }
    daxpy(n, a, x, y); // Invoke daxpy kernel
    // Check if all values are 4.0
    double error = 0.0;  
    for(int i = 0; i < n; i++){
        error += y[i] - 4.0;
    }
    printf("Error: %f\n", error);
    free(x); free(y);
return 0;
}

