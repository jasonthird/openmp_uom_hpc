#include <stdio.h>
#include <math.h>
#include <omp.h>

#define maxsize 500
#define iterations 1000000
#define row 50
#define col 50
#define start 100
#define accuracy 27
#define num_threads 16

int main(int argc, char* argv[])
{
  int i, j, k;
  double table1[maxsize][maxsize];
  double table2[maxsize][maxsize];
  double diff;

  //set number of threads
  omp_set_num_threads(num_threads);

  /* initialize both tables*/

  for(i=0;i<maxsize;i++)
    for(j=0;j<maxsize;j++)
    {
      table1[i][j]=0;
      table2[i][j]=0;
    }

  /* repeate for each iteration */
  for(k = 0; k < iterations; k++) 
  {
    
    /* create a heat source */
    table1[row][col] = start;
    
    /* difference initialization */
    diff = 0.0;

  #pragma omp parallel for private(i,j) shared (table1,table2) reduction(+:diff) schedule(auto)
  for(i=1;i<maxsize-1;i++){
      double temp[maxsize-2][maxsize-2];
      for(j=1;j<maxsize-1;j++) {
          temp[i-1][j-1] = 0.25 *(table1[i-1][j] + table1[i+1][j] + table1[i][j-1] + table1[i][j+1]);
          diff += (temp[i-1][j-1]-table1[i][j])*(temp[i-1][j-1]-table1[i][j]);
      }
      for(j=1;j<maxsize-1;j++) {
          table2[i][j] = temp[i-1][j-1];
      }
  }

    // /* print result */
    // for(i=0;i<maxsize;i++)
    // {
    //   for(j=0;j<maxsize;j++)
    //     printf("%5.0f ",table2[i][j]);
    //   printf("\n");
    // }
    // printf("\n");
  
    /* print difference and check convergence */
    diff = sqrt(diff);
    //printf("diff = %3.25f\n\n", diff);

    if (diff < accuracy) {
    printf ("\n\nConvergence in %d iterations\n\n", k);
          break;
  }	
  
  /* copy new table to old table */ 
    for(i=0;i<maxsize;i++)
      for(j=0;j<maxsize;j++)
        table1[i][j]=table2[i][j];
  }

  return 0;
}
