/**
 * fdtd-2d.c: This file is part of the PolyBench/C 3.2 test suite.
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
/* Default data type is double, default size is 50x1000x1000. */
#include "fdtd-2d.h"


/* Array initialization. */
static
void init_array (int tmax,
		 int nx,
		 int ny,
		 DATA_TYPE POLYBENCH_1D(ex,NX*NY,nx*ny),
		 DATA_TYPE POLYBENCH_1D(ey,NX*NY,nx*ny),
		 DATA_TYPE POLYBENCH_1D(hz,NX*NY,nx*ny),
		 DATA_TYPE POLYBENCH_1D(_fict_,TMAX,tmax))
{
  int i, j;

  for (i = 0; i < tmax; i++)
    _fict_[i] = (DATA_TYPE) i;
  for (i = 0; i < nx; i++)
    for (j = 0; j < ny; j++)
      {
	ex[i*nx+j] = ((DATA_TYPE) i*(j+1)) / nx;
	ey[i*nx+j] = ((DATA_TYPE) i*(j+2)) / ny;
	hz[i*nx+j] = ((DATA_TYPE) i*(j+3)) / nx;
      }
}


/* DCE code. Must scan the entire live-out data.
   Can be used also to check the correctness of the output. */
static
void print_array(int nx,
		 int ny,
		 DATA_TYPE POLYBENCH_1D(ex,NX*NY,nx*ny),
		 DATA_TYPE POLYBENCH_1D(ey,NX*NY,nx*ny),
		 DATA_TYPE POLYBENCH_1D(hz,NX*NY,nx*ny))
{
  int i, j;

  for (i = 0; i < nx; i++)
    for (j = 0; j < ny; j++) {
      fprintf(stderr, DATA_PRINTF_MODIFIER, ex[i*nx+j]);
      fprintf(stderr, DATA_PRINTF_MODIFIER, ey[i*nx+j]);
      fprintf(stderr, DATA_PRINTF_MODIFIER, hz[i*nx+j]);
      if ((i * nx + j) % 20 == 0) fprintf(stderr, "\n");
    }
  fprintf(stderr, "\n");
}


/* Main computational kernel. The whole function will be timed,
   including the call and return. */
static
void kernel_fdtd_2d(int tmax,
		    int nx,
		    int ny,
		    DATA_TYPE POLYBENCH_1D(ex,NX*NY,nx*ny),
		    DATA_TYPE POLYBENCH_1D(ey,NX*NY,nx*ny),
		    DATA_TYPE POLYBENCH_1D(hz,NX*NY,nx*ny),
		    DATA_TYPE POLYBENCH_1D(_fict_,TMAX,tmax))
{
  int t, i, j;


  for(t = 0; t < _PB_TMAX; t++)
    {
      for (j = 0; j < _PB_NY; j++)
	ey[0*nx+j] = _fict_[t];
      #pragma omp target enter data map(to:ny) device(1)
      #pragma omp target enter data map(to:ey[0:nx*ny]) device(1)
      #pragma omp target enter data map(to:j) device(1)
      #pragma omp target enter data map(to:i) device(1)
      #pragma omp target enter data map(to:hz[0:nx*ny]) device(1)
      #pragma omp target enter data map(to:nx) device(1)
      #pragma omp target teams distribute parallel for device(1) collapse(2) private(i,j) shared(ey,hz,nx,ny) 
      for (i = 1; i < _PB_NX; i++)
	for (j = 0; j < _PB_NY; j++)
	  ey[i*nx+j] = ey[i*nx+j] - 0.5*(hz[i*nx+j]-hz[(i-1)*nx+j]);
      #pragma omp target enter data map(to:ex[0:nx*ny]) device(1)
      #pragma omp target update from(i) device(1)
      #pragma omp target update to(i) device(1)
      #pragma omp target teams distribute parallel for device(1) collapse(2) private(i,j) shared(ex,hz,nx,ny) 
      for (i = 0; i < _PB_NX; i++)
	for (j = 1; j < _PB_NY; j++)
	  ex[i*nx+j] = ex[i*nx+j] - 0.5*(hz[i*nx+j]-hz[i*nx+(j-1)]);
      #pragma omp target update from(i) device(1)
      #pragma omp target update to(i) device(1)
      #pragma omp target teams distribute parallel for device(1) collapse(2) private(i,j) shared(ex,ey,hz,nx,ny) 
      for (i = 0; i < _PB_NX - 1; i++)
	for (j = 0; j < _PB_NY - 1; j++)
	  hz[i*nx+j] = hz[i*nx+j] - 0.7*  (ex[i*nx+(j+1)] - ex[i*nx+j] +
				       ey[(i+1)*nx+j] - ey[i*nx+j]);
  #pragma omp target exit data map(from:ny) device(1)
  #pragma omp target exit data map(from:ey[0:nx*ny]) device(1)
  #pragma omp target exit data map(delete:j) device(1)
  #pragma omp target exit data map(delete:i) device(1)
  #pragma omp target exit data map(from:hz[0:nx*ny]) device(1)
  #pragma omp target exit data map(from:nx) device(1)
  #pragma omp target exit data map(from:ex[0:nx*ny]) device(1)
    }
}


int main(int argc, char** argv)
{
  /* Retrieve problem size. */
  int tmax = TMAX;
  int nx = NX;
  int ny = NY;

  /* Variable declaration/allocation. */
  POLYBENCH_1D_ARRAY_DECL(ex,DATA_TYPE,NX*NY,nx*ny);
  POLYBENCH_1D_ARRAY_DECL(ey,DATA_TYPE,NX*NY,nx*ny);
  POLYBENCH_1D_ARRAY_DECL(hz,DATA_TYPE,NX*NY,nx*ny);
  POLYBENCH_1D_ARRAY_DECL(_fict_,DATA_TYPE,TMAX,tmax);

  /* Initialize array(s). */
  init_array (tmax, nx, ny,
	      POLYBENCH_ARRAY(ex),
	      POLYBENCH_ARRAY(ey),
	      POLYBENCH_ARRAY(hz),
	      POLYBENCH_ARRAY(_fict_));

  /* Start timer. */
  polybench_start_instruments;

  /* Run kernel. */
  kernel_fdtd_2d (tmax, nx, ny,
		  POLYBENCH_ARRAY(ex),
		  POLYBENCH_ARRAY(ey),
		  POLYBENCH_ARRAY(hz),
		  POLYBENCH_ARRAY(_fict_));


  /* Stop and print timer. */
  polybench_stop_instruments;
  polybench_print_instruments;

  /* Prevent dead-code elimination. All live-out data must be printed
     by the function call in argument. */
  polybench_prevent_dce(print_array(nx, ny, POLYBENCH_ARRAY(ex),
				    POLYBENCH_ARRAY(ey),
				    POLYBENCH_ARRAY(hz)));

  /* Be clean. */
  POLYBENCH_FREE_ARRAY(ex);
  POLYBENCH_FREE_ARRAY(ey);
  POLYBENCH_FREE_ARRAY(hz);
  POLYBENCH_FREE_ARRAY(_fict_);

  return 0;
}
