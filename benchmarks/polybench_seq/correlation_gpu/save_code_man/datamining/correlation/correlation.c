/**
 * correlation.c: This file is part of the PolyBench/C 3.2 test suite.
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
#include "correlation.h"


/* Array initialization. */
static
void init_array (int m,
		 int n,
		 DATA_TYPE *float_n,
		 DATA_TYPE POLYBENCH_1D(data,M*N,m*n))
{
  int i, j;

  *float_n = 1.2;

  for (i = 0; i < m; i++)
    for (j = 0; j < n; j++)
      data[i*m+j] = ((DATA_TYPE) i*j) / M;
}


/* DCE code. Must scan the entire live-out data.
   Can be used also to check the correctness of the output. */
static
void print_array(int m,
		 DATA_TYPE POLYBENCH_1D(symmat,M*M,m*m))

{
  int i, j;

  for (i = 0; i < m; i++)
    for (j = 0; j < m; j++) {
      fprintf (stderr, DATA_PRINTF_MODIFIER, symmat[i*m+j]);
      if ((i * m + j) % 20 == 0) fprintf (stderr, "\n");
    }
  fprintf (stderr, "\n");
}


/* Main computational kernel. The whole function will be timed,
   including the call and return. */
static
void kernel_correlation(int m, int n,
			DATA_TYPE float_n,
			DATA_TYPE POLYBENCH_1D(data,M*N,m*n),
			DATA_TYPE POLYBENCH_1D(symmat,M*M,m*m),
			DATA_TYPE POLYBENCH_1D(mean,M,m),
			DATA_TYPE POLYBENCH_1D(stddev,M,m))
{
  int i, j, j1, j2;

  DATA_TYPE eps = 0.1f;

#define sqrt_of_array_cell(x,j) sqrt(x[j])

  /* Determine mean of column vectors of input data matrix */
  #pragma omp target enter data map(to:i) device(1)
  #pragma omp target enter data map(to:mean[0:m]) device(1)
  #pragma omp target enter data map(to:n) device(1)
  #pragma omp target enter data map(to:data[0:m*n]) device(1)
  #pragma omp target enter data map(to:j) device(1)
  #pragma omp target enter data map(to:m) device(1)
  #pragma omp target enter data map(to:float_n) device(1)
  #pragma omp target teams distribute parallel for device(1) private(i,j) shared(data,float_n,m,mean,n) 
  for (j = 0; j < _PB_M; j++)
    {
      mean[j] = 0.0;
      for (i = 0; i < _PB_N; i++)
	mean[j] += data[i*m+j];
      mean[j] /= float_n;
    }

  /* Determine standard deviations of column vectors of data matrix. */
  #pragma omp target enter data map(to:stddev[0:m]) device(1)
  #pragma omp target enter data map(to:eps) device(1)
  #pragma omp target update from(stddev[0:m]) device(1)
  #pragma omp target update from(j) device(1)
  #pragma omp target update to(j) device(1)
  #pragma omp target update from(j) device(1)
  #pragma omp target teams distribute parallel for device(1) private(i,j) shared(data,eps,float_n,m,mean,n,stddev) 
  for (j = 0; j < _PB_M; j++)
    {
      stddev[j] = 0.0;
      for (i = 0; i < _PB_N; i++)
	stddev[j] += (data[i*m+j] - mean[j]) * (data[i*m+j] - mean[j]);
      stddev[j] /= float_n;
      stddev[j] = sqrt_of_array_cell(stddev, j);
      /* The following in an inelegant but usual way to handle
	 near-zero std. dev. values, which below would cause a zero-
	 divide. */
      stddev[j] = stddev[j] <= eps ? 1.0 : stddev[j];
    }

  /* Center and reduce the column vectors. */
  #pragma omp target update from(i) device(1)
  #pragma omp target update to(i) device(1)
  #pragma omp target teams distribute parallel for device(1) collapse(2) private(i,j) shared(data,float_n,m,mean,n,stddev) 
  for (i = 0; i < _PB_N; i++)
    for (j = 0; j < _PB_M; j++)
      {
	data[i*m+j] -= mean[j];
	data[i*m+j] /= sqrt(float_n) * stddev[j];
      }

  /* Calculate the m * m correlation matrix. */
  #pragma omp target enter data map(to:j1) device(1)
  #pragma omp target enter data map(to:symmat[0:m*m]) device(1)
  #pragma omp target enter data map(to:j2) device(1)
  #pragma omp target teams distribute parallel for device(1) private(i,j1,j2) shared(data,m,n,symmat) 
  for (j1 = 0; j1 < _PB_M-1; j1++)
    {
      symmat[j1*m+j1] = 1.0;
      for (j2 = j1+1; j2 < _PB_M; j2++)
	{
	  symmat[j1*m+j2] = 0.0;
	  for (i = 0; i < _PB_N; i++)
	    symmat[j1*m+j2] += (data[i*m+j1] * data[i*m+j2]);
	  symmat[j2*m+j1] = symmat[j1*m+j2];
	}
    }
  #pragma omp target exit data map(delete:i) device(1)
  #pragma omp target exit data map(from:mean[0:m]) device(1)
  #pragma omp target exit data map(delete:n) device(1)
  #pragma omp target exit data map(from:data[0:m*n]) device(1)
  #pragma omp target exit data map(delete:j) device(1)
  #pragma omp target exit data map(delete:m) device(1)
  #pragma omp target exit data map(delete:float_n) device(1)
  #pragma omp target exit data map(from:stddev[0:m]) device(1)
  #pragma omp target exit data map(delete:eps) device(1)
  #pragma omp target exit data map(delete:j1) device(1)
  #pragma omp target exit data map(from:symmat[0:m*m]) device(1)
  #pragma omp target exit data map(delete:j2) device(1)
  symmat[(_PB_M-1)*m+(_PB_M-1)] = 1.0;

}


int main(int argc, char** argv)
{
  /* Retrieve problem size. */
  int n = N;
  int m = M;

  /* Variable declaration/allocation. */
  DATA_TYPE float_n;
  POLYBENCH_1D_ARRAY_DECL(data,DATA_TYPE,M*N,m*n);
  POLYBENCH_1D_ARRAY_DECL(symmat,DATA_TYPE,M*M,m*m);
  POLYBENCH_1D_ARRAY_DECL(mean,DATA_TYPE,M,m);
  POLYBENCH_1D_ARRAY_DECL(stddev,DATA_TYPE,M,m);

  /* Initialize array(s). */
  init_array (m, n, &float_n, POLYBENCH_ARRAY(data));

  /* Start timer. */
  polybench_start_instruments;

  /* Run kernel. */
  kernel_correlation (m, n, float_n,
		      POLYBENCH_ARRAY(data),
		      POLYBENCH_ARRAY(symmat),
		      POLYBENCH_ARRAY(mean),
		      POLYBENCH_ARRAY(stddev));

  /* Stop and print timer. */
  polybench_stop_instruments;
  polybench_print_instruments;

  /* Prevent dead-code elimination. All live-out data must be printed
     by the function call in argument. */
  polybench_prevent_dce(print_array(m, POLYBENCH_ARRAY(symmat)));

  /* Be clean. */
  POLYBENCH_FREE_ARRAY(data);
  POLYBENCH_FREE_ARRAY(symmat);
  POLYBENCH_FREE_ARRAY(mean);
  POLYBENCH_FREE_ARRAY(stddev);

  return 0;
}
