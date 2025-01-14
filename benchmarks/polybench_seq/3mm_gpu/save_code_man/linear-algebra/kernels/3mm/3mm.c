/**
 * 3mm.c: This file is part of the PolyBench/C 3.2 test suite.
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
#include "3mm.h"


/* Array initialization. */
static
void init_array(int ni, int nj, int nk, int nl, int nm,
		DATA_TYPE POLYBENCH_1D(A,NI*NK,ni*nk),
		DATA_TYPE POLYBENCH_1D(B,NK*NJ,nk*nj),
		DATA_TYPE POLYBENCH_1D(C,NJ*NM,nj*nm),
		DATA_TYPE POLYBENCH_1D(D,NM*NL,nm*nl))
{
  int i, j;

  for (i = 0; i < ni; i++)
    for (j = 0; j < nk; j++)
      A[i*NI+j] = ((DATA_TYPE) i*j) / ni;
  for (i = 0; i < nk; i++)
    for (j = 0; j < nj; j++)
      B[i*NK+j] = ((DATA_TYPE) i*(j+1)) / nj;
  for (i = 0; i < nj; i++)
    for (j = 0; j < nm; j++)
      C[i*NJ+j] = ((DATA_TYPE) i*(j+3)) / nl;
  for (i = 0; i < nm; i++)
    for (j = 0; j < nl; j++)
      D[i*NM+j] = ((DATA_TYPE) i*(j+2)) / nk;
}


/* DCE code. Must scan the entire live-out data.
   Can be used also to check the correctness of the output. */
static
void print_array(int ni, int nl,
		 DATA_TYPE POLYBENCH_1D(G,NI*NL,ni*nl))
{
  int i, j;

  for (i = 0; i < ni; i++)
    for (j = 0; j < nl; j++) {
	fprintf (stderr, DATA_PRINTF_MODIFIER, G[i*NI+j]);
	if ((i * ni + j) % 20 == 0) fprintf (stderr, "\n");
    }
  fprintf (stderr, "\n");
}


/* Main computational kernel. The whole function will be timed,
   including the call and return. */
static
void kernel_3mm(int ni, int nj, int nk, int nl, int nm,
		DATA_TYPE POLYBENCH_1D(E,NI*NJ,ni*nj),
		DATA_TYPE POLYBENCH_1D(A,NI*NK,ni*nk),
		DATA_TYPE POLYBENCH_1D(B,NK*NJ,nk*nj),
		DATA_TYPE POLYBENCH_1D(F,NJ*NL,nj*nl),
		DATA_TYPE POLYBENCH_1D(C,NJ*NM,nj*nm),
		DATA_TYPE POLYBENCH_1D(D,NM*NL,nm*nl),
		DATA_TYPE POLYBENCH_1D(G,NI*NL,ni*nl))
{
  int i, j, k;

  /* E := A*B */
  #pragma omp target enter data map(to:B[0:NI*NK]) device(1)
  #pragma omp target enter data map(to:nk) device(1)
  #pragma omp target enter data map(to:k) device(1)
  #pragma omp target enter data map(to:ni) device(1)
  #pragma omp target enter data map(to:E[0:NI*NK]) device(1)
  #pragma omp target enter data map(to:i) device(1)
  #pragma omp target enter data map(to:nj) device(1)
  #pragma omp target enter data map(to:A[0:NI*NK]) device(1)
  #pragma omp target enter data map(to:j) device(1)
  #pragma omp target teams distribute parallel for device(1) collapse(2) private(i,j,k) shared(A,B,E,ni,nj,nk) 
  for (i = 0; i < _PB_NI; i++)
    for (j = 0; j < _PB_NJ; j++)
      {
	E[i*NI+j] = 0;
	for (k = 0; k < _PB_NK; ++k)
	  E[i*NI+j] += A[i*NI+k] * B[k*NK+j];
      }
  /* F := C*D */
  #pragma omp target enter data map(to:nm) device(1)
  #pragma omp target enter data map(to:D[0:NI*NK]) device(1)
  #pragma omp target enter data map(to:C[0:NI*NK]) device(1)
  #pragma omp target enter data map(to:nl) device(1)
  #pragma omp target enter data map(to:F[0:NI*NK]) device(1)
  #pragma omp target update from(i) device(1)
  #pragma omp target update to(i) device(1)
  #pragma omp target teams distribute parallel for device(1) collapse(2) private(i,j,k) shared(C,D,F,nj,nl,nm) 
  for (i = 0; i < _PB_NJ; i++)
    for (j = 0; j < _PB_NL; j++)
      {
	F[i*NJ+j] = 0;
	for (k = 0; k < _PB_NM; ++k)
	  F[i*NJ+j] += C[i*NJ+k] * D[k*NM+j];
      }
  /* G := E*F */
  #pragma omp target enter data map(to:G[0:NI*NK]) device(1)
  #pragma omp target update from(i) device(1)
  #pragma omp target update to(i) device(1)
  #pragma omp target teams distribute parallel for device(1) collapse(2) private(i,j,k) shared(E,F,G,ni,nj,nl) 
  for (i = 0; i < _PB_NI; i++)
    for (j = 0; j < _PB_NL; j++)
      {
	G[i*NI+j] = 0;
	for (k = 0; k < _PB_NJ; ++k)
	  G[i*NI+j] += E[i*NI+k] * F[k*NJ+j];
      }

#pragma omp target exit data map(delete:B[0:NI*NK]) device(1)
#pragma omp target exit data map(delete:nk) device(1)
#pragma omp target exit data map(delete:k) device(1)
#pragma omp target exit data map(delete:ni) device(1)
#pragma omp target exit data map(from:E[0:NI*NK]) device(1)
#pragma omp target exit data map(delete:i) device(1)
#pragma omp target exit data map(delete:nj) device(1)
#pragma omp target exit data map(delete:A[0:NI*NK]) device(1)
#pragma omp target exit data map(delete:j) device(1)
#pragma omp target exit data map(delete:nm) device(1)
#pragma omp target exit data map(delete:D[0:NI*NK]) device(1)
#pragma omp target exit data map(delete:C[0:NI*NK]) device(1)
#pragma omp target exit data map(delete:nl) device(1)
#pragma omp target exit data map(from:F[0:NI*NK]) device(1)
#pragma omp target exit data map(from:G[0:NI*NK]) device(1)
}


int main(int argc, char** argv)
{
  /* Retrieve problem size. */
  int ni = NI;
  int nj = NJ;
  int nk = NK;
  int nl = NL;
  int nm = NM;

  /* Variable declaration/allocation. */
  POLYBENCH_1D_ARRAY_DECL(E, DATA_TYPE, NI* NJ, ni* nj);
  POLYBENCH_1D_ARRAY_DECL(A, DATA_TYPE, NI* NK, ni* nk);
  POLYBENCH_1D_ARRAY_DECL(B, DATA_TYPE, NK* NJ, nk* nj);
  POLYBENCH_1D_ARRAY_DECL(F, DATA_TYPE, NJ* NL, nj* nl);
  POLYBENCH_1D_ARRAY_DECL(C, DATA_TYPE, NJ* NM, nj* nm);
  POLYBENCH_1D_ARRAY_DECL(D, DATA_TYPE, NM* NL, nm* nl);
  POLYBENCH_1D_ARRAY_DECL(G, DATA_TYPE, NI* NL, ni* nl);

  /* Initialize array(s). */
  init_array (ni, nj, nk, nl, nm,
	      POLYBENCH_ARRAY(A),
	      POLYBENCH_ARRAY(B),
	      POLYBENCH_ARRAY(C),
	      POLYBENCH_ARRAY(D));

  /* Start timer. */
  polybench_start_instruments;

  /* Run kernel. */
  kernel_3mm (ni, nj, nk, nl, nm,
	      POLYBENCH_ARRAY(E),
	      POLYBENCH_ARRAY(A),
	      POLYBENCH_ARRAY(B),
	      POLYBENCH_ARRAY(F),
	      POLYBENCH_ARRAY(C),
	      POLYBENCH_ARRAY(D),
	      POLYBENCH_ARRAY(G));

  /* Stop and print timer. */
  polybench_stop_instruments;
  polybench_print_instruments;

  /* Prevent dead-code elimination. All live-out data must be printed
     by the function call in argument. */
  polybench_prevent_dce(print_array(ni, nl,  POLYBENCH_ARRAY(G)));

  /* Be clean. */
  POLYBENCH_FREE_ARRAY(E);
  POLYBENCH_FREE_ARRAY(A);
  POLYBENCH_FREE_ARRAY(B);
  POLYBENCH_FREE_ARRAY(F);
  POLYBENCH_FREE_ARRAY(C);
  POLYBENCH_FREE_ARRAY(D);
  POLYBENCH_FREE_ARRAY(G);

  return 0;
}
