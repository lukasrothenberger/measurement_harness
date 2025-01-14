/**
 * gesummv.c: This file is part of the PolyBench/C 3.2 test suite.
 *
 *
 * Contact: Louis-Noel Pouchet <pouchet@cse.ohio-state.edu>
 * Web address: http://polybench.sourceforge.net
 */
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <math.h>

/* Include polybench common header. */
#include <polybench.h>

/* Include benchmark-specific header. */
/* Default data type is double, default size is 4000. */
#include "gesummv.h"


/* Array initialization. */
static
void init_array(int n,
		DATA_TYPE *alpha,
		DATA_TYPE *beta,
		DATA_TYPE POLYBENCH_1D(A,N*N,n*n),
		DATA_TYPE POLYBENCH_1D(B,N*N,n*n),
		DATA_TYPE POLYBENCH_1D(x,N,n))
{
  int i, j;

  *alpha = 43532;
  *beta = 12313;
  for (i = 0; i < n; i++)
    {
      x[i] = ((DATA_TYPE) i) / n;
      for (j = 0; j < n; j++) {
	A[i*n+j] = ((DATA_TYPE) i*j) / n;
	B[i*n+j] = ((DATA_TYPE) i*j) / n;
      }
    }
}


/* DCE code. Must scan the entire live-out data.
   Can be used also to check the correctness of the output. */
static
void print_array(int n,
		 DATA_TYPE POLYBENCH_1D(y,N,n))

{
  int i;

  for (i = 0; i < n; i++) {
    fprintf (stderr, DATA_PRINTF_MODIFIER, y[i]);
    if (i % 20 == 0) fprintf (stderr, "\n");
  }
}


/* Main computational kernel. The whole function will be timed,
   including the call and return. */
static
void kernel_gesummv(int n,
		    DATA_TYPE alpha,
		    DATA_TYPE beta,
		    DATA_TYPE POLYBENCH_1D(A,N*N,n*n),
		    DATA_TYPE POLYBENCH_1D(B,N*N,n*n),
		    DATA_TYPE POLYBENCH_1D(tmp,N,n),
		    DATA_TYPE POLYBENCH_1D(x,N,n),
		    DATA_TYPE POLYBENCH_1D(y,N,n))
{
  int i, j;

  #pragma omp target enter data map(to:n) device(1)
  #pragma omp target enter data map(to:j) device(1)
  #pragma omp target enter data map(to:i) device(1)
  #pragma omp target enter data map(to:y[0:n]) device(1)
  #pragma omp target enter data map(to:beta) device(1)
  #pragma omp target enter data map(to:A[0:n*n]) device(1)
  #pragma omp target enter data map(to:tmp[0:n]) device(1)
  #pragma omp target enter data map(to:x[0:n]) device(1)
  #pragma omp target enter data map(to:B[0:n*n]) device(1)
  #pragma omp target enter data map(to:alpha) device(1)
  #pragma omp target teams distribute parallel for device(1) private(i,j) shared(A,B,alpha,beta,n,tmp,x,y) 
  for (i = 0; i < _PB_N; i++)
    {
      tmp[i] = 0;
      y[i] = 0;
      for (j = 0; j < _PB_N; j++)
	{
	  tmp[i] = A[i*n+j] * x[j] + tmp[i];
	  y[i] = B[i*n+j] * x[j] + y[i];
	}
      y[i] = alpha * tmp[i] + beta * y[i];
    }

#pragma omp target exit data map(delete:n) device(1)
#pragma omp target exit data map(delete:j) device(1)
#pragma omp target exit data map(delete:i) device(1)
#pragma omp target exit data map(from:y[0:n]) device(1)
#pragma omp target exit data map(delete:beta) device(1)
#pragma omp target exit data map(delete:A[0:n*n]) device(1)
#pragma omp target exit data map(from:tmp[0:n]) device(1)
#pragma omp target exit data map(delete:x[0:n]) device(1)
#pragma omp target exit data map(delete:B[0:n*n]) device(1)
#pragma omp target exit data map(delete:alpha) device(1)
}


int main(int argc, char** argv)
{
  /* Retrieve problem size. */
  int n = N;

  /* Variable declaration/allocation. */
  DATA_TYPE alpha;
  DATA_TYPE beta;
  POLYBENCH_1D_ARRAY_DECL(A, DATA_TYPE, N*N, n*n);
  POLYBENCH_1D_ARRAY_DECL(B, DATA_TYPE, N*N, n*n);
  POLYBENCH_1D_ARRAY_DECL(tmp, DATA_TYPE, N, n);
  POLYBENCH_1D_ARRAY_DECL(x, DATA_TYPE, N, n);
  POLYBENCH_1D_ARRAY_DECL(y, DATA_TYPE, N, n);


  /* Initialize array(s). */
  init_array (n, &alpha, &beta,
	      POLYBENCH_ARRAY(A),
	      POLYBENCH_ARRAY(B),
	      POLYBENCH_ARRAY(x));

  /* Start timer. */
  polybench_start_instruments;

  /* Run kernel. */
  kernel_gesummv (n, alpha, beta,
		  POLYBENCH_ARRAY(A),
		  POLYBENCH_ARRAY(B),
		  POLYBENCH_ARRAY(tmp),
		  POLYBENCH_ARRAY(x),
		  POLYBENCH_ARRAY(y));

  /* Stop and print timer. */
  polybench_stop_instruments;
  polybench_print_instruments;

  /* Prevent dead-code elimination. All live-out data must be printed
     by the function call in argument. */
  polybench_prevent_dce(print_array(n, POLYBENCH_ARRAY(y)));

  /* Be clean. */
  POLYBENCH_FREE_ARRAY(A);
  POLYBENCH_FREE_ARRAY(B);
  POLYBENCH_FREE_ARRAY(tmp);
  POLYBENCH_FREE_ARRAY(x);
  POLYBENCH_FREE_ARRAY(y);

  return 0;
}
