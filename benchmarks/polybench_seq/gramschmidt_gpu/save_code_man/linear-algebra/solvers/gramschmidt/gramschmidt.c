/**
 * gramschmidt.c: This file is part of the PolyBench/C 3.2 test suite.
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
/* Default data type is double, default size is 512. */
#include "gramschmidt.h"


/* Array initialization. */
static
void init_array(int ni, int nj,
		DATA_TYPE POLYBENCH_1D(A,NI*NJ,ni*nj),
		DATA_TYPE POLYBENCH_1D(R,NJ*NJ,nj*nj),
		DATA_TYPE POLYBENCH_1D(Q,NI*NJ,ni*nj))
{
  int i, j;

  for (i = 0; i < ni; i++)
    for (j = 0; j < nj; j++) {
      A[i*ni+j] = ((DATA_TYPE) i*j) / ni;
      Q[i*ni+j] = ((DATA_TYPE) i*(j+1)) / nj;
    }
  for (i = 0; i < nj; i++)
    for (j = 0; j < nj; j++)
      R[i*nj+j] = ((DATA_TYPE) i*(j+2)) / nj;
}


/* DCE code. Must scan the entire live-out data.
   Can be used also to check the correctness of the output. */
static
void print_array(int ni, int nj,
		 DATA_TYPE POLYBENCH_1D(A,NI*NJ,ni*nj),
		 DATA_TYPE POLYBENCH_1D(R,NJ*NJ,nj*nj),
		 DATA_TYPE POLYBENCH_1D(Q,NI*NJ,ni*nj))
{
  int i, j;

  for (i = 0; i < ni; i++)
    for (j = 0; j < nj; j++) {
	fprintf (stderr, DATA_PRINTF_MODIFIER, A[i*ni+j]);
	if (i % 20 == 0) fprintf (stderr, "\n");
    }
  fprintf (stderr, "\n");
  for (i = 0; i < nj; i++)
    for (j = 0; j < nj; j++) {
	fprintf (stderr, DATA_PRINTF_MODIFIER, R[i*nj+j]);
	if (i % 20 == 0) fprintf (stderr, "\n");
    }
  fprintf (stderr, "\n");
  for (i = 0; i < ni; i++)
    for (j = 0; j < nj; j++) {
	fprintf (stderr, DATA_PRINTF_MODIFIER, Q[i*ni+j]);
	if (i % 20 == 0) fprintf (stderr, "\n");
    }
  fprintf (stderr, "\n");
}


/* Main computational kernel. The whole function will be timed,
   including the call and return. */
static
void kernel_gramschmidt(int ni, int nj,
			DATA_TYPE POLYBENCH_1D(A,NI*NJ,ni*nj),
			DATA_TYPE POLYBENCH_1D(R,NJ*NJ,nj*nj),
			DATA_TYPE POLYBENCH_1D(Q,NI*NJ,ni*nj))
{
  int i, j, k;

  DATA_TYPE nrm;

  #pragma omp target enter data map(to:k) device(1)
  #pragma omp target enter data map(to:nj) device(1)
  #pragma omp target enter data map(to:R[0:ni*nj]) device(1)
  #pragma omp target enter data map(to:Q[0:ni*nj]) device(1)
  #pragma omp target enter data map(to:j) device(1)
  #pragma omp target enter data map(to:i) device(1)
  #pragma omp target enter data map(to:ni) device(1)
  #pragma omp target enter data map(to:nrm) device(1)
  #pragma omp target enter data map(to:A[0:ni*nj]) device(1)
  #pragma omp target teams distribute parallel for device(1) private(i,j,k,nrm) shared(A,Q,R,ni,nj) 
  for (k = 0; k < _PB_NJ; k++)
    {
      nrm = 0;
      for (i = 0; i < _PB_NI; i++)
        nrm += A[i*ni+k] * A[i*ni+k];
      R[k*nj+k] = sqrt(nrm);
      for (i = 0; i < _PB_NI; i++)
        Q[i*ni+k] = A[i*ni+k] / R[k*nj+k];
      for (j = k + 1; j < _PB_NJ; j++)
	{
	  R[k*nj+j] = 0;
	  for (i = 0; i < _PB_NI; i++)
	    R[k*nj+j] += Q[i*ni+k] * A[i*ni+j];
	  for (i = 0; i < _PB_NI; i++)
	    A[i*ni+j] = A[i*ni+j] - Q[i*ni+k] * R[k*nj+j];
	}
    }

#pragma omp target exit data map(delete:k) device(1)
#pragma omp target exit data map(delete:nj) device(1)
#pragma omp target exit data map(from:R[0:ni*nj]) device(1)
#pragma omp target exit data map(from:Q[0:ni*nj]) device(1)
#pragma omp target exit data map(delete:j) device(1)
#pragma omp target exit data map(delete:i) device(1)
#pragma omp target exit data map(delete:ni) device(1)
#pragma omp target exit data map(delete:nrm) device(1)
#pragma omp target exit data map(from:A[0:ni*nj]) device(1)
}


int main(int argc, char** argv)
{
  /* Retrieve problem size. */
  int ni = NI;
  int nj = NJ;

  /* Variable declaration/allocation. */
  POLYBENCH_1D_ARRAY_DECL(A,DATA_TYPE,NI*NJ,ni*nj);
  POLYBENCH_1D_ARRAY_DECL(R,DATA_TYPE,NJ*NJ,nj*nj);
  POLYBENCH_1D_ARRAY_DECL(Q,DATA_TYPE,NI*NJ,ni*nj);

  /* Initialize array(s). */
  init_array (ni, nj,
	      POLYBENCH_ARRAY(A),
	      POLYBENCH_ARRAY(R),
	      POLYBENCH_ARRAY(Q));

  /* Start timer. */
  polybench_start_instruments;

  /* Run kernel. */
  kernel_gramschmidt (ni, nj,
		      POLYBENCH_ARRAY(A),
		      POLYBENCH_ARRAY(R),
		      POLYBENCH_ARRAY(Q));

  /* Stop and print timer. */
  polybench_stop_instruments;
  polybench_print_instruments;

  /* Prevent dead-code elimination. All live-out data must be printed
     by the function call in argument. */
  polybench_prevent_dce(print_array(ni, nj, POLYBENCH_ARRAY(A), POLYBENCH_ARRAY(R), POLYBENCH_ARRAY(Q)));

  /* Be clean. */
  POLYBENCH_FREE_ARRAY(A);
  POLYBENCH_FREE_ARRAY(R);
  POLYBENCH_FREE_ARRAY(Q);

  return 0;
}
