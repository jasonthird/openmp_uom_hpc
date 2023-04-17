//compile with clang++ jacobi_heat.cpp -fopenmp -O2 -march=native -mtune=native
//or g++ jacobi_heat.cpp -fopenmp -O2 -march=native -mtune=native

#include <cstdio>
#include <cmath>
#include <omp.h>
#include <array>

#define maxsize 500
#define iterations 1000000
#define row 50
#define col 50
#define start 100
#define accuracy 27

int main()
{
	int k,i,j;
	auto table1 = new double[maxsize][maxsize];
	auto table2 = new double[maxsize][maxsize];
	double diff;

	//set number of threads
	std::fill_n(&table1[0][0], maxsize*maxsize, 0);
	std::fill_n(&table2[0][0], maxsize*maxsize, 0);
    k=0;
	double start_time = omp_get_wtime();
	#pragma omp parallel shared(table1,diff,table2,k) default(none)
	{
        while (k < iterations)
		{
			#pragma omp single
			{
				/* create a heat source */
				table1[row][col] = start;
				diff = 0.0;
			}
			
			#pragma omp for private(i,j) reduction(+:diff) 
				for(i=1;i<maxsize-1;i++)
				for(j=1;j<maxsize-1;j++) {
					table2[i][j] = 0.25 *(table1[i-1][j] + table1[i+1][j] + table1[i][j-1] + table1[i][j+1]);
					diff += (table2[i][j]-table1[i][j])*(table2[i][j]-table1[i][j]);
				}
			/* print result */
			// for(i=0;i<maxsize;i++)
			// {
			// 	for(j=0;j<maxsize;j++)
			// 	printf("%5.0f ",table2[i][j]);
			// 	printf("\n");
			// }
			// printf("\n");
			
			/* print difference and check convergence */
			#pragma omp single
			{
				diff = sqrt(diff);
                if (diff < accuracy) {
                    printf ("\n\nConvergence in %d iterations\n\n", k);
                    printf("diff = %3.25f\n\n", diff);
                    k=iterations;
                } else{
                    k++;
                    std::swap(table1, table2);
                }
			}
		}
	}
	double end_time = omp_get_wtime();
	printf("Time: %f seconds", end_time - start_time);
	delete[] table1;
	delete[] table2;
	return 0;
}
