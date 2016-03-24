// Matrix A times vector b
// c = A x b
// N = {4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192}
// добавлено измерение времени работы

#include <stdio.h>
#include <stdlib.h>                    // malloc(), free(), strtol(), exit()
#include <omp.h>

double **dmatrix(int, int);
double  *dvector(int);
void free_dmatrix(double **);
void free_dvector(double *);

int main(int argc, char *argv[]) {
   double **A, *b, *c, sum;
   long i, j, N;
   double t_start, t_end;

	omp_set_num_threads(2);

   N = 10000;
   A = dmatrix(N, N);
   b = dvector(N);
   c = dvector(N);
   if ( !A || !b || !c ) {
       printf("Not enough memory!\n");
       exit(2);
   }

   for (i=0; i < N; i++)
      for (j=0; j < N; j++)
         A[i][j] = (double)i;
   for (i=0; i < N; i++)
      b[i] = 1.0;

   t_start = omp_get_wtime();
   #pragma omp parallel for shared(A, b, c, N) private(i, j, sum)
   for (i=0; i < N; i++) {
      sum = 0.0;
      for (j=0; j < N; j++)
         sum += A[i][j] * b[j];
      c[i] = sum;
   }
   t_end = omp_get_wtime();
   printf("%10ld %20.15lf\n", N, t_end - t_start);

   if (argc == 3 && argv[2][0] == '+')
      for (i=0; i < N; i++)
         printf("%5.1lf\n", c[i]);

   free_dvector(c);
   free_dvector(b);
   free_dmatrix(A);

   return 0;
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
