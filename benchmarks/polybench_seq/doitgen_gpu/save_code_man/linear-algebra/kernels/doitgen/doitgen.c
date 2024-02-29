/**
 * doitgen.c: This file is part of the PolyBench/C 3.2 test suite.
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
#include "doitgen.h"


/* Array initialization. */
static
void init_array(int nr, int nq, int np,
		DATA_TYPE POLYBENCH_1D(A,NR*NQ*NP,nr*nq*np),
		DATA_TYPE POLYBENCH_1D(C4,NP*NP,np*np))
{
  int i, j, k;

  for (i = 0; i < nr; i++)
    for (j = 0; j < nq; j++)
      for (k = 0; k < np; k++)
	A[i*nr+j*nq+k] = ((DATA_TYPE) i*j + k) / np;
  for (i = 0; i < np; i++)
    for (j = 0; j < np; j++)
      C4[i*NP+j] = ((DATA_TYPE) i*j) / np;
}


/* DCE code. Must scan the entire live-out data.
   Can be used also to check the correctness of the output. */
static
void print_array(int nr, int nq, int np,
		 DATA_TYPE POLYBENCH_3D(A,NR,NQ,NP,nr,nq,np))
{
  int i, j, k;

  for (i = 0; i < nr; i++)
    for (j = 0; j < nq; j++)
      for (k = 0; k < np; k++) {
	fprintf (stderr, DATA_PRINTF_MODIFIER, A[i][j][k]);
	if (i % 20 == 0) fprintf (stderr, "\n");
      }
  fprintf (stderr, "\n");
}


/* Main computational kernel. The whole function will be timed,
   including the call and return. */
static
void kernel_doitgen(int nr, int nq, int np,
		    DATA_TYPE POLYBENCH_1D(A,NR*NQ*NP,nr*nq*np),
		    DATA_TYPE POLYBENCH_1D(C4,NP*NP,np*np),
		    DATA_TYPE POLYBENCH_1D(sum,NR*NQ*NP,nr*nq*np))
{
  int r, q, p, s;

  #pragma omp target enter data map(to:p) device(1)
  #pragma omp target enter data map(to:sum[0:nr*nq*np]) device(1)
  #pragma omp target enter data map(to:nq) device(1)
  #pragma omp target enter data map(to:r) device(1)
  #pragma omp target enter data map(to:s) device(1)
  #pragma omp target enter data map(to:q) device(1)
  #pragma omp target enter data map(to:np) device(1)
  #pragma omp target enter data map(to:A[0:nr*nq*np]) device(1)
  #pragma omp target enter data map(to:C4[0:nr*nq*np]) device(1)
  #pragma omp target enter data map(to:nr) device(1)
  #pragma omp target teams distribute parallel for device(1) collapse(2) private(p,q,r,s) shared(A,C4,np,nq,nr,sum) 
  for (r = 0; r < _PB_NR; r++)
    for (q = 0; q < _PB_NQ; q++)  {
      for (p = 0; p < _PB_NP; p++)  {
	sum[r*nr+q*nq+p] = 0;
	for (s = 0; s < _PB_NP; s++)
	  sum[r*nr+q*nq+p] = sum[r*nr+q*nq+p] + A[r*nr+q*nq+s] * C4[s*np+p];
      }
      for (p = 0; p < _PB_NR; p++)
	A[r*nr+q*nq+p] = sum[r*nr+q*nq+p];
    }

#pragma omp target exit data map(delete:p) device(1)
#pragma omp target exit data map(from:sum[0:nr*nq*np]) device(1)
#pragma omp target exit data map(delete:nq) device(1)
#pragma omp target exit data map(delete:r) device(1)
#pragma omp target exit data map(delete:s) device(1)
#pragma omp target exit data map(delete:q) device(1)
#pragma omp target exit data map(delete:np) device(1)
#pragma omp target exit data map(from:A[0:nr*nq*np]) device(1)
#pragma omp target exit data map(delete:C4[0:nr*nq*np]) device(1)
#pragma omp target exit data map(delete:nr) device(1)
}


int main(int argc, char** argv)
{
  /* Retrieve problem size. */
  int nr = NR;
  int nq = NQ;
  int np = NP;

  /* Variable declaration/allocation. */
  POLYBENCH_1D_ARRAY_DECL(A,DATA_TYPE,NR*NQ*NP,nr*nq*np);
  POLYBENCH_1D_ARRAY_DECL(sum,DATA_TYPE,NR*NQ*NP,nr*nq*np);
  POLYBENCH_1D_ARRAY_DECL(C4,DATA_TYPE,NP*NP,np*np);

  /* Initialize array(s). */
  init_array (nr, nq, np,
	      POLYBENCH_ARRAY(A),
	      POLYBENCH_ARRAY(C4));

  /* Start timer. */
  polybench_start_instruments;

  /* Run kernel. */
  kernel_doitgen (nr, nq, np,
		  POLYBENCH_ARRAY(A),
		  POLYBENCH_ARRAY(C4),
		  POLYBENCH_ARRAY(sum));

  /* Stop and print timer. */
  polybench_stop_instruments;
  polybench_print_instruments;

  /* Prevent dead-code elimination. All live-out data must be printed
     by the function call in argument. */
  polybench_prevent_dce(print_array(nr, nq, np,  POLYBENCH_ARRAY(A)));

  /* Be clean. */
  POLYBENCH_FREE_ARRAY(A);
  POLYBENCH_FREE_ARRAY(sum);
  POLYBENCH_FREE_ARRAY(C4);

  return 0;
}
