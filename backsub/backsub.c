#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <omp.h>

int main ( int argc, char *argv[] )  {

int   i, j, N;
double *x, *b, **a, sum;
char any;

	if (argc != 2) {
		printf ("Usage : %s <matrix size>\n", argv[0]);
                exit(1);
	}
	N = strtol(argv[1], NULL, 10);

	/* Allocate space for matrices */
	a = (double **) malloc ( N * sizeof ( double *) );
	for ( i = 0; i < N; i++) 
		a[i] = ( double * ) malloc ( (i+1) * sizeof ( double ) );
	b = ( double * ) malloc ( N * sizeof ( double ) );
	x = ( double * ) malloc ( N * sizeof ( double ) );

	/* Create floats between 0 and 1. Diagonal elents between 2 and 3. */
	srand ( time ( NULL));
	for (i = 0; i < N; i++) {
		x[i] = 0.0;
		b[i] = (double)rand()/(RAND_MAX*2.0-1.0);
		a[i][i] = 2.0+(double)rand()/(RAND_MAX*2.0-1.0);
		for (j = 0; j < i; j++) 
			a[i][j] = (double)rand()/(RAND_MAX*2.0-1.0);;
	} 

        /* Calulation */
	double start = omp_get_wtime();
	for (i = 0; i < N; i++) {
		sum = 0.0;
		#pragma omp parallel for reduction(+:sum) firstprivate(i,N) shared(x,a,b) private(j) default(none)
		for (j = 0; j < i; j++) {
			sum = sum + (x[j] * a[i][j]);
			//printf ("%d %d %f %f %f \t \n", i, j, x[j], a[i][j], sum);
		}	
		x[i] = (b[i] - sum) / a[i][i];
		//printf ("%d %f %f %f %f \n", i, b[i], sum, a[i][i], x[i]);
	}
	double end = omp_get_wtime();

    // /* Print result */
	// for (i = 0; i < N; i++) {
	// 	for (j = 0; j <= i; j++)
	// 		printf ("%f \t", a[i][j]);	
	// 	printf ("%f \t%f\n", x[i], b[i]);
	// }

   /* Check result */
	// for (i = 0; i < N; i++) {
	// 	sum = 0.0;
	// 	for (j = 0; j <= i; j++) 
	// 		sum = sum + (x[j]*a[i][j]);	
	// 	if (fabs(sum - b[i]) > 0.00001) {
	// 		printf("%f != %f\n", sum, b[i]);
	// 		printf("Validation Failed...\n");
	// 	}
	// }
	printf("Time taken: %f seconds for %d elements " , end - start, N);
	return 0;
}
