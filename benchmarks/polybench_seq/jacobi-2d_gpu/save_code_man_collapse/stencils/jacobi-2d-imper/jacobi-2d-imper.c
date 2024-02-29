/**
 * jacobi-2d-imper.c: This file is part of the PolyBench/C 3.2 test suite.
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
/* Default data type is double, default size is 20x1000. */
#include "jacobi-2d-imper.h"


/* Array initialization. */
static
void init_array (int n,
		 DATA_TYPE POLYBENCH_1D(A,N*N,n*n),
		 DATA_TYPE POLYBENCH_1D(B,N*N,n*n))
{
  int i, j;

  for (i = 0; i < n; i++)
    for (j = 0; j < n; j++)
      {
	A[i*n+j] = ((DATA_TYPE) i*(j+2) + 2) / n;
	B[i*n+j] = ((DATA_TYPE) i*(j+3) + 3) / n;
      }
}


/* DCE code. Must scan the entire live-out data.
   Can be used also to check the correctness of the output. */
static
void print_array(int n,
		 DATA_TYPE POLYBENCH_1D(A,N*N,n*n))

{
  int i, j;

  for (i = 0; i < n; i++)
    for (j = 0; j < n; j++) {
      fprintf(stderr, DATA_PRINTF_MODIFIER, A[i*n+j]);
      if ((i * n + j) % 20 == 0) fprintf(stderr, "\n");
    }
  fprintf(stderr, "\n");
}


/* Main computational kernel. The whole function will be timed,
   including the call and return. */
static
void kernel_jacobi_2d_imper(int tsteps,
			    int n,
			    DATA_TYPE POLYBENCH_1D(A,N*N,n*n),
			    DATA_TYPE POLYBENCH_1D(B,N*N,n*n))
{
  int t, i, j;

  for (t = 0; t < _PB_TSTEPS; t++)
    {
      #pragma omp target enter data map(to:n) device(1)
      #pragma omp target enter data map(to:A[0:n*n]) device(1)
      #pragma omp target enter data map(to:j) device(1)
      #pragma omp target enter data map(to:i) device(1)
      #pragma omp target enter data map(to:B[0:n*n]) device(1)
      #pragma omp target update to(j) device(1)
      #pragma omp target teams distribute parallel for device(1) collapse(2) private(i,j) shared(A,B,n) 
      for (i = 1; i < _PB_N - 1; i++)
	for (j = 1; j < _PB_N - 1; j++)
	  B[i*n+j] = 0.2 * (A[i*n+j] + A[i*n+(j-1)] + A[i*n+(1+j)] + A[(1+i)*n+j] + A[(i-1)*n+j]);
      #pragma omp target update to(j) device(1)
      #pragma omp target update from(i) device(1)
      #pragma omp target update to(j) device(1)
      #pragma omp target update to(i) device(1)
      #pragma omp target update to(B[0:n*n]) device(1)
      #pragma omp target teams distribute parallel for device(1) collapse(2) private(i,j) shared(A,B,n) 
      for (i = 1; i < _PB_N-1; i++)
	for (j = 1; j < _PB_N-1; j++)
	  A[i*n+j] = B[i*n+j];
  #pragma omp target exit data map(from:n) device(1)
  #pragma omp target exit data map(from:A[0:n*n]) device(1)
  #pragma omp target exit data map(delete:j) device(1)
  #pragma omp target exit data map(delete:i) device(1)
  #pragma omp target exit data map(from:B[0:n*n]) device(1)
    }
}


int main(int argc, char** argv)
{
  /* Retrieve problem size. */
  int n = N;
  int tsteps = TSTEPS;

  /* Variable declaration/allocation. */
  POLYBENCH_1D_ARRAY_DECL(A, DATA_TYPE, N*N, n*n);
  POLYBENCH_1D_ARRAY_DECL(B, DATA_TYPE, N*N, n*n);


  /* Initialize array(s). */
  init_array (n, POLYBENCH_ARRAY(A), POLYBENCH_ARRAY(B));

  /* Start timer. */
  polybench_start_instruments;

  /* Run kernel. */
  kernel_jacobi_2d_imper (tsteps, n, POLYBENCH_ARRAY(A), POLYBENCH_ARRAY(B));

  /* Stop and print timer. */
  polybench_stop_instruments;
  polybench_print_instruments;

  /* Prevent dead-code elimination. All live-out data must be printed
     by the function call in argument. */
  polybench_prevent_dce(print_array(n, POLYBENCH_ARRAY(A)));

  /* Be clean. */
  POLYBENCH_FREE_ARRAY(A);
  POLYBENCH_FREE_ARRAY(B);

  return 0;
}
