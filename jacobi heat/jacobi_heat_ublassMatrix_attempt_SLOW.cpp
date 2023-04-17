#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <cmath>
#include "omp.h"

#define MAXSIZE 500
#define ITERATIONS 1000000
#define ROW 50
#define COL 50
#define START 100.0
#define ACCURACY 27

int main(){
    int i, j, k;
    double diff = 0;
    boost::numeric::ublas::matrix<float> table1 = boost::numeric::ublas::zero_matrix<double>(MAXSIZE, MAXSIZE);
    boost::numeric::ublas::matrix<float> table2 = boost::numeric::ublas::zero_matrix<double>(MAXSIZE, MAXSIZE);
    // iteractions
    double start = omp_get_wtime();
    for(k = 0; k < ITERATIONS; k++){
        table1 (ROW, COL) = START;
        diff = 0.0;
        #pragma omp parallel for private(i,j) shared(table2) reduction(+:diff)
        for(i=1;i<MAXSIZE-1;i++)
            for(j=1;j<MAXSIZE-1;j++) {
            float y = 0.25 *(table1(i-1,j) + table1(i+1,j) + table1(i,j-1) + table1(i,j+1));
            table2.insert_element(i,j,y);
            diff += (table2(i,j)-table1(i,j))*(table2(i,j)-table1(i,j));
            }
    
        diff = sqrt(diff);
        //printf("diff = %3.25f\n\n", diff);        
        if (diff < ACCURACY) {
            printf ("\n\nConvergence in %d iterations\n\n", k);
            break;
        }
        std::swap(table1, table2);
    }
    double end = omp_get_wtime();
    std::cout << "Time: " << end - start << std::endl;
}

