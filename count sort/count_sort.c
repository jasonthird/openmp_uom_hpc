//hpc course university of macedonia
//ΚΩΝΣΤΑΝΤΙΝΟΣ ΜΑΡΓΑΡΙΤΗΣ sequential jacobi heat
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define UPPER 1000
#define LOWER 0

int main(int argc, char *argv[])
{
   int *x, *y;
   int i, j, my_num, my_place;
   
   if (argc != 2) {
		printf ("Usage : %s <array_size>\n", argv[0]);
		return 1;
   }
   
   int n = strtol(argv[1], NULL, 10);
   x = ( int * ) malloc ( n * sizeof ( int ) );
   y = ( int * ) malloc ( n * sizeof ( int ) );

   for (i=0; i<n; i++)
		//x[i] = n - i;
		x[i] = (rand() % (UPPER - LOWER + 1)) + LOWER;
    
    double start = omp_get_wtime();
    #pragma omp parallel for private(i,j,my_num,my_place) shared(x,y,n)
   for (j=0; j<n; j++) {

     my_num = x[j];
     my_place = 0;
     for (i=0; i<n; i++)
		if ((my_num > x[i]) || ((my_num == x[i]) && (j < i))) 
			my_place++;
     y[my_place] = my_num;
   }  
    double end = omp_get_wtime();

    printf("Time taken: %f seconds \n", end - start);
   
  //  for (i=0; i<n; i++) 
	// 	printf("%d\n", y[i]);
			
   return 0;
}
