/**
 * mvt.c: This file is part of the PolyBench/C 3.2 test suite.
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
#include "mvt.h"


/* Array initialization. */
static
void init_array(int n,
		DATA_TYPE POLYBENCH_1D(x1,N,n),
		DATA_TYPE POLYBENCH_1D(x2,N,n),
		DATA_TYPE POLYBENCH_1D(y_1,N,n),
		DATA_TYPE POLYBENCH_1D(y_2,N,n),
		DATA_TYPE POLYBENCH_1D(A,N*N,n*n))
{
  int i, j;

  for (i = 0; i < n; i++)
    {
      x1[i] = ((DATA_TYPE) i) / n;
      x2[i] = ((DATA_TYPE) i + 1) / n;
      y_1[i] = ((DATA_TYPE) i + 3) / n;
      y_2[i] = ((DATA_TYPE) i + 4) / n;
      for (j = 0; j < n; j++)
	A[i*n+j] = ((DATA_TYPE) i*j) / N;
    }
}


/* DCE code. Must scan the entire live-out data.
   Can be used also to check the correctness of the output. */
static
void print_array(int n,
		 DATA_TYPE POLYBENCH_1D(x1,N,n),
		 DATA_TYPE POLYBENCH_1D(x2,N,n))

{
  int i;

  for (i = 0; i < n; i++) {
    fprintf (stderr, DATA_PRINTF_MODIFIER, x1[i]);
    fprintf (stderr, DATA_PRINTF_MODIFIER, x2[i]);
    if (i % 20 == 0) fprintf (stderr, "\n");
  }
}


/* Main computational kernel. The whole function will be timed,
   including the call and return. */
static
void kernel_mvt(int n,
		DATA_TYPE POLYBENCH_1D(x1,N,n),
		DATA_TYPE POLYBENCH_1D(x2,N,n),
		DATA_TYPE POLYBENCH_1D(y_1,N,n),
		DATA_TYPE POLYBENCH_1D(y_2,N,n),
		DATA_TYPE POLYBENCH_1D(A,N*N,n*n))
{
  int i, j;

  #pragma omp target enter data map(to:n) device(1)
  #pragma omp target enter data map(to:i) device(1)
  #pragma omp target enter data map(to:x1[0:n]) device(1)
  #pragma omp target enter data map(to:A[0:n*n]) device(1)
  #pragma omp target enter data map(to:y_1[0:n]) device(1)
  #pragma omp target enter data map(to:j) device(1)
  #pragma omp target teams distribute parallel for device(1) collapse(2) private(i,j) shared(A,n,x1,y_1) 
  for (i = 0; i < _PB_N; i++)
    for (j = 0; j < _PB_N; j++)
      x1[i] = x1[i] + A[i*n+j] * y_1[j];
  #pragma omp target update from(j) device(1)
  #pragma omp target update from(A[0:n*n]) device(1)
  #pragma omp target update from(i) device(1)
  #pragma omp target enter data map(to: x2[0:n]) device(1)
  #pragma omp target enter data map(to:y_2[0:n]) device(1)
  #pragma omp parallel for collapse(2) private(i,j) shared(A,n,x2,y_2) 
  for (i = 0; i < _PB_N; i++)
    for (j = 0; j < _PB_N; j++)
      x2[i] = x2[i] + A[j*n+i] * y_2[j];

#pragma omp target exit data map(delete:n) device(1)
#pragma omp target exit data map(delete:i) device(1)
#pragma omp target exit data map(from:x1[0:n]) device(1)
#pragma omp target exit data map(from:x2[0:n]) device(1)
#pragma omp target exit data map(delete:A[0:n*n]) device(1)
#pragma omp target exit data map(delete:y_1[0:n]) device(1)
#pragma omp target exit data map(delete:y_2[0:n]) device(1)
#pragma omp target exit data map(delete:j) device(1)
}


int main(int argc, char** argv)
{
  /* Retrieve problem size. */
  int n = N;

  /* Variable declaration/allocation. */
  POLYBENCH_1D_ARRAY_DECL(A, DATA_TYPE, N*N, n*n);
  POLYBENCH_1D_ARRAY_DECL(x1, DATA_TYPE, N, n);
  POLYBENCH_1D_ARRAY_DECL(x2, DATA_TYPE, N, n);
  POLYBENCH_1D_ARRAY_DECL(y_1, DATA_TYPE, N, n);
  POLYBENCH_1D_ARRAY_DECL(y_2, DATA_TYPE, N, n);


  /* Initialize array(s). */
  init_array (n,
	      POLYBENCH_ARRAY(x1),
	      POLYBENCH_ARRAY(x2),
	      POLYBENCH_ARRAY(y_1),
	      POLYBENCH_ARRAY(y_2),
	      POLYBENCH_ARRAY(A));

  /* Start timer. */
  polybench_start_instruments;

  /* Run kernel. */
  kernel_mvt (n,
	      POLYBENCH_ARRAY(x1),
	      POLYBENCH_ARRAY(x2),
	      POLYBENCH_ARRAY(y_1),
	      POLYBENCH_ARRAY(y_2),
	      POLYBENCH_ARRAY(A));

  /* Stop and print timer. */
  polybench_stop_instruments;
  polybench_print_instruments;

  /* Prevent dead-code elimination. All live-out data must be printed
     by the function call in argument. */
  polybench_prevent_dce(print_array(n, POLYBENCH_ARRAY(x1), POLYBENCH_ARRAY(x2)));

  /* Be clean. */
  POLYBENCH_FREE_ARRAY(A);
  POLYBENCH_FREE_ARRAY(x1);
  POLYBENCH_FREE_ARRAY(x2);
  POLYBENCH_FREE_ARRAY(y_1);
  POLYBENCH_FREE_ARRAY(y_2);

  return 0;
}
