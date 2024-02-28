/**
 * 2mm.c: This file is part of the PolyBench/C 3.2 test suite.
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
#include "2mm.h"


/* Array initialization. */
static
void init_array(int ni, int nj, int nk, int nl,
		DATA_TYPE *alpha,
		DATA_TYPE *beta,
		DATA_TYPE POLYBENCH_1D(A,NI*NK,ni*nl),
		DATA_TYPE POLYBENCH_1D(B,NK*NJ,nk*nj),
		DATA_TYPE POLYBENCH_1D(C,NL*NJ,nl*nj),
		DATA_TYPE POLYBENCH_1D(D,NI*NL,ni*nl))
{
  int i, j;

  *alpha = 32412;
  *beta = 2123;
  for (i = 0; i < ni; i++)
    for (j = 0; j < nk; j++)
      A[i*ni+j] = ((DATA_TYPE) i*j) / ni;
  for (i = 0; i < nk; i++)
    for (j = 0; j < nj; j++)
      B[i*nk+j] = ((DATA_TYPE) i*(j+1)) / nj;
  for (i = 0; i < nl; i++)
    for (j = 0; j < nj; j++)
      C[i*nl+j] = ((DATA_TYPE) i*(j+3)) / nl;
  for (i = 0; i < ni; i++)
    for (j = 0; j < nl; j++)
      D[i*ni+j] = ((DATA_TYPE) i*(j+2)) / nk;
}


/* DCE code. Must scan the entire live-out data.
   Can be used also to check the correctness of the output. */
static
void print_array(int ni, int nl,
		 DATA_TYPE POLYBENCH_1D(D,NI*NL,ni*nl))
{
  int i, j;

  for (i = 0; i < ni; i++)
    for (j = 0; j < nl; j++) {
	fprintf (stderr, DATA_PRINTF_MODIFIER, D[i*ni+j]);
	if ((i * ni + j) % 20 == 0) fprintf (stderr, "\n");
    }
  fprintf (stderr, "\n");
}


/* Main computational kernel. The whole function will be timed,
   including the call and return. */
static
void kernel_2mm(int ni, int nj, int nk, int nl,
		DATA_TYPE alpha,
		DATA_TYPE beta,
		DATA_TYPE POLYBENCH_1D(tmp,NI*NJ,ni*nj),
		DATA_TYPE POLYBENCH_1D(A,NI*NK,ni*nk),
		DATA_TYPE POLYBENCH_1D(B,NK*NJ,nk*nj),
		DATA_TYPE POLYBENCH_1D(C,NL*NJ,nl*nj),
		DATA_TYPE POLYBENCH_1D(D,NI*NL,ni*nl))
{
  int i, j, k;

  /* D := alpha*A*B*C + beta*D */
  #pragma omp target enter data map(to:B[0:nk*nl]) device(2)
  #pragma omp target enter data map(to:k) device(2)
  #pragma omp target enter data map(to:A[0:nk*nl]) device(2)
  #pragma omp target enter data map(to:nk) device(2)
  #pragma omp target enter data map(to:alpha) device(2)
  #pragma omp target enter data map(to:ni) device(2)
  #pragma omp target enter data map(to:j) device(2)
  #pragma omp target enter data map(to:i) device(2)
  #pragma omp target enter data map(to:nj) device(2)
  #pragma omp target enter data map(to:tmp[0:nk*nl]) device(2)
  #pragma omp target teams distribute parallel for device(2) collapse(2) private(i,j,k) shared(A,B,alpha,ni,nj,nk,tmp) 
  for (i = 0; i < _PB_NI; i++)
    for (j = 0; j < _PB_NJ; j++)
      {
	tmp[i*_PB_NI+j] = 0;
	for (k = 0; k < _PB_NK; ++k)
	  tmp[i*_PB_NI+j] += alpha * A[i*_PB_NI+k] * B[k*_PB_NK+j];
      }
  #pragma omp target enter data map(to:D[0:nk*nl]) device(2)
  #pragma omp target enter data map(to:C[0:nk*nl]) device(2)
  #pragma omp target enter data map(to:beta) device(2)
  #pragma omp target teams distribute parallel for device(2) collapse(2) private(i,j,k) shared(C,D,beta,ni,nj,nk,nl,tmp) 
  for (i = 0; i < _PB_NI; i++)
    for (j = 0; j < _PB_NL; j++)
      {
	D[i*_PB_NI+j] *= beta;
	for (k = 0; k < _PB_NJ; ++k)
	  D[i*_PB_NI+j] += tmp[i*_PB_NI+k] * C[k*_PB_NK+j];
      }

#pragma omp target exit data map(delete:B[0:nk*nl]) device(2)
#pragma omp target exit data map(delete:C[0:nk*nl]) device(2)
#pragma omp target exit data map(from:D[0:nk*nl]) device(2)
#pragma omp target exit data map(delete:beta) device(2)
#pragma omp target exit data map(delete:k) device(2)
#pragma omp target exit data map(delete:A[0:nk*nl]) device(2)
#pragma omp target exit data map(delete:nk) device(2)
#pragma omp target exit data map(delete:alpha) device(2)
#pragma omp target exit data map(delete:ni) device(2)
#pragma omp target exit data map(delete:j) device(2)
#pragma omp target exit data map(delete:i) device(2)
#pragma omp target exit data map(delete:nj) device(2)
#pragma omp target exit data map(from:tmp[0:nk*nl]) device(2)
}


int main(int argc, char** argv)
{
  /* Retrieve problem size. */
  int ni = NI;
  int nj = NJ;
  int nk = NK;
  int nl = NL;

  /* Variable declaration/allocation. */
  DATA_TYPE alpha;
  DATA_TYPE beta;
  POLYBENCH_1D_ARRAY_DECL(tmp,DATA_TYPE,NI*NJ,ni*nj);
  POLYBENCH_1D_ARRAY_DECL(A,DATA_TYPE,NI*NK,ni*nk);
  POLYBENCH_1D_ARRAY_DECL(B,DATA_TYPE,NK*NJ,nk*nj);
  POLYBENCH_1D_ARRAY_DECL(C,DATA_TYPE,NL*NJ,nl*nj);
  POLYBENCH_1D_ARRAY_DECL(D,DATA_TYPE,NI*NL,ni*nl);

  /* Initialize array(s). */
  init_array (ni, nj, nk, nl, &alpha, &beta,
	      POLYBENCH_ARRAY(A),
	      POLYBENCH_ARRAY(B),
	      POLYBENCH_ARRAY(C),
	      POLYBENCH_ARRAY(D));

  /* Start timer. */
  polybench_start_instruments;

  /* Run kernel. */
  kernel_2mm (ni, nj, nk, nl,
	      alpha, beta,
	      POLYBENCH_ARRAY(tmp),
	      POLYBENCH_ARRAY(A),
	      POLYBENCH_ARRAY(B),
	      POLYBENCH_ARRAY(C),
	      POLYBENCH_ARRAY(D));

  /* Stop and print timer. */
  polybench_stop_instruments;
  polybench_print_instruments;

  /* Prevent dead-code elimination. All live-out data must be printed
     by the function call in argument. */
  polybench_prevent_dce(print_array(ni, nl,  POLYBENCH_ARRAY(D)));

  /* Be clean. */
  POLYBENCH_FREE_ARRAY(tmp);
  POLYBENCH_FREE_ARRAY(A);
  POLYBENCH_FREE_ARRAY(B);
  POLYBENCH_FREE_ARRAY(C);
  POLYBENCH_FREE_ARRAY(D);

  return 0;
}
