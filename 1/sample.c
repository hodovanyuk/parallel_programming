#include <stdio.h>
#include <stdlib.h>   
#include <math.h>                 // malloc(), free(), strtol(), exit()
#include <omp.h>

double **dmatrix(int, int);
double  *dvector(int);
void free_dmatrix(double **);
void free_dvector(double *);
void fun();

int main(int argc, char *argv[]) {
	int i ,j ;
	for(i=0,j=10;i<8;i++){
		j=j*10;
		fun(j);
		}
   return 0;
}
void fun(long N){
	   double *a, *b, *c;
   long i;
   double t_start, t_end;

  
   a = dvector(N);
   b = dvector(N);
   c = dvector(N);
   if ( !a || !b || !c ) {
       printf("Not enough memory!\n");
       exit(2);
   }

   for (i=0; i < N; i++)
      a[i] = sqrtl(i);
   for (i=0; i < N; i++)
      b[i] = 1.0;

   t_start = omp_get_wtime();
   
  // #pragma omp parallel for
	for(i=0;i<N;i++){
		c[i] = a[i]*b[i];
	}
   
   t_end = omp_get_wtime();
   
   printf("%10ld %20.15lf\n", N, t_end - t_start);
  


   free_dvector(c);
   free_dvector(b);
   free_dvector(a);

	}
// allocates a double matrix with range [0..m-1][0..n-1]
double **dmatrix(int m, int n) {
   double **a;
   int i;

   a    = (double **) malloc(m   * sizeof(double *));
   a[0] = (double  *) malloc(m*n * sizeof(double  ));
   for (i=1; i < m; i++)
      a[i] = a[i-1] + n;

   return a;
}

// frees a matrix allocated with dmatrix
void free_dmatrix(double **a) {
   free(a[0]);
   free(a);
}

// allocates a double vector with range [0..n-1]
double *dvector(int n) {
   double *a;

   a = (double *)malloc(n * sizeof(double));

   return a;
}

// frees a vector allocated with dvector
void free_dvector(double *a) {
   free(a);
}
