/**
 * syr2k.c: This file is part of the PolyBench/C 3.2 test suite.
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
#include "syr2k.h"


/* Array initialization. */
static
void init_array(int ni, int nj,
		DATA_TYPE *alpha,
		DATA_TYPE *beta,
		DATA_TYPE POLYBENCH_1D(C,NI*NI,ni*ni),
		DATA_TYPE POLYBENCH_1D(A,NI*NJ,ni*nj),
		DATA_TYPE POLYBENCH_1D(B,NI*NJ,ni*nj))
{
  int i, j;

  *alpha = 32412;
  *beta = 2123;
  for (i = 0; i < ni; i++)
    for (j = 0; j < nj; j++) {
      A[i*ni+j] = ((DATA_TYPE) i*j) / ni;
      B[i*ni+j] = ((DATA_TYPE) i*j) / ni;
    }
  for (i = 0; i < ni; i++)
    for (j = 0; j < ni; j++)
      C[i*ni+j] = ((DATA_TYPE) i*j) / ni;
}


/* DCE code. Must scan the entire live-out data.
   Can be used also to check the correctness of the output. */
static
void print_array(int ni,
		 DATA_TYPE POLYBENCH_1D(C,NI*NI,ni*ni))
{
  int i, j;

  for (i = 0; i < ni; i++)
    for (j = 0; j < ni; j++) {
	fprintf (stderr, DATA_PRINTF_MODIFIER, C[i*ni+j]);
	if ((i * ni + j) % 20 == 0) fprintf (stderr, "\n");
    }
  fprintf (stderr, "\n");
}


/* Main computational kernel. The whole function will be timed,
   including the call and return. */
static
void kernel_syr2k(int ni, int nj,
		  DATA_TYPE alpha,
		  DATA_TYPE beta,
		  DATA_TYPE POLYBENCH_1D(C,NI*NI,ni*ni),
		  DATA_TYPE POLYBENCH_1D(A,NI*NJ,ni*nj),
		  DATA_TYPE POLYBENCH_1D(B,NI*NJ,ni*nj))
{
  int i, j, k;

  /*    C := alpha*A*B' + alpha*B*A' + beta*C */
  #pragma omp target enter data map(to:j) device(1)
  #pragma omp target enter data map(to:i) device(1)
  #pragma omp target enter data map(to:ni) device(1)
  #pragma omp target enter data map(to:beta) device(1)
  #pragma omp target enter data map(to:C[0:ni*ni]) device(1)
  #pragma omp target teams distribute parallel for device(1) collapse(2) private(i,j) shared(C,beta,ni) 
  for (i = 0; i < _PB_NI; i++)
    for (j = 0; j < _PB_NI; j++)
      C[i*ni+j] *= beta;
  #pragma omp target enter data map(to:k) device(1)
  #pragma omp target enter data map(to:A[0:ni*nj]) device(1)
  #pragma omp target enter data map(to:alpha) device(1)
  #pragma omp target enter data map(to:B[0:ni*nj]) device(1)
  #pragma omp target enter data map(to:nj) device(1)
  #pragma omp target update from(i) device(1)
  #pragma omp target update to(i) device(1)
  #pragma omp target teams distribute parallel for device(1) collapse(2) private(i,j,k) shared(A,B,C,alpha,ni,nj) 
  for (i = 0; i < _PB_NI; i++)
    for (j = 0; j < _PB_NI; j++)
      for (k = 0; k < _PB_NJ; k++)
	{
	  C[i*ni+j] += alpha * A[i*ni+k] * B[j*ni+k];
	  C[i*ni+j] += alpha * B[i*ni+k] * A[j*ni+k];
	}

#pragma omp target exit data map(delete:j) device(1)
#pragma omp target exit data map(delete:i) device(1)
#pragma omp target exit data map(delete:ni) device(1)
#pragma omp target exit data map(delete:beta) device(1)
#pragma omp target exit data map(from:C[0:ni*ni]) device(1)
#pragma omp target exit data map(delete:k) device(1)
#pragma omp target exit data map(delete:A[0:ni*nj]) device(1)
#pragma omp target exit data map(delete:alpha) device(1)
#pragma omp target exit data map(delete:B[0:ni*nj]) device(1)
#pragma omp target exit data map(delete:nj) device(1)
}


int main(int argc, char** argv)
{
  /* Retrieve problem size. */
  int ni = NI;
  int nj = NJ;

  /* Variable declaration/allocation. */
  DATA_TYPE alpha;
  DATA_TYPE beta;
  POLYBENCH_1D_ARRAY_DECL(C,DATA_TYPE,NI*NI,ni*ni);
  POLYBENCH_1D_ARRAY_DECL(A,DATA_TYPE,NI*NJ,ni*nj);
  POLYBENCH_1D_ARRAY_DECL(B,DATA_TYPE,NI*NJ,ni*nj);

  /* Initialize array(s). */
  init_array (ni, nj, &alpha, &beta,
	      POLYBENCH_ARRAY(C),
	      POLYBENCH_ARRAY(A),
	      POLYBENCH_ARRAY(B));

  /* Start timer. */
  polybench_start_instruments;

  /* Run kernel. */
  kernel_syr2k (ni, nj,
		alpha, beta,
		POLYBENCH_ARRAY(C),
		POLYBENCH_ARRAY(A),
		POLYBENCH_ARRAY(B));

  /* Stop and print timer. */
  polybench_stop_instruments;
  polybench_print_instruments;

  /* Prevent dead-code elimination. All live-out data must be printed
     by the function call in argument. */
  polybench_prevent_dce(print_array(ni, POLYBENCH_ARRAY(C)));

  /* Be clean. */
  POLYBENCH_FREE_ARRAY(C);
  POLYBENCH_FREE_ARRAY(A);
  POLYBENCH_FREE_ARRAY(B);

  return 0;
}
