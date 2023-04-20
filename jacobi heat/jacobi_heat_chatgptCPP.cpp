//fed file jacobi_heat_seq.c and asked "can you convert that code to cpp, use vectors" 
//then asked if he can parallelize it with openmp and it came up with this
//it even added comments to the code
//then asked it "can you improve the code quality even further, modernize it and use the latest c++ standards and features?"
//and then asked it "add a NUM_THREADS above"
//and then asked it "add a timer that measures how long the k loop was executing and then print the result in screen"
//and thats the result

#include <iostream>
#include <vector>
#include <cmath>
#include <chrono>
#include <omp.h>

#define MAXSIZE 500
#define ITERATIONS 1000000
#define ROW 50
#define COL 50
#define START 100
#define ACCURACY 27
#define NUM_THREADS 8

int main() {
    std::vector<std::vector<double>> table1(MAXSIZE, std::vector<double>(MAXSIZE));
    std::vector<std::vector<double>> table2(MAXSIZE, std::vector<double>(MAXSIZE));
    double diff;

    // initialize both tables
    for (int i = 0; i < MAXSIZE; i++) {
        for (int j = 0; j < MAXSIZE; j++) {
            table1[i][j] = 0;
            table2[i][j] = 0;
        }
    }

    // set the number of threads
    omp_set_num_threads(NUM_THREADS);

    // repeat for each iteration
    auto start = std::chrono::high_resolution_clock::now(); // start timer
    for (int k = 0; k < ITERATIONS; k++) {
        // create a heat source
        table1[ROW][COL] = START;

        // difference initialization
        diff = 0.0;

        // perform the calculations
        #pragma omp parallel for reduction(+:diff)
        for (int i = 1; i < MAXSIZE - 1; i++) {
            for (int j = 1; j < MAXSIZE - 1; j++) {
                table2[i][j] = 0.25 * (table1[i-1][j] + table1[i+1][j] + table1[i][j-1] + table1[i][j+1]);
                diff += (table2[i][j] - table1[i][j]) * (table2[i][j] - table1[i][j]);
            }
        }

        // print difference and check convergence
        diff = std::sqrt(diff);
        // std::cout << "diff = " << diff << std::endl;

        if (diff < ACCURACY) {
            std::cout << "Convergence in " << k << " iterations" << std::endl;
            break;
        }

        // copy new table to old table
        for (int i = 0; i < MAXSIZE; i++) {
            for (int j = 0; j < MAXSIZE; j++) {
                table1[i][j] = table2[i][j];
            }
        }
    }
    auto end = std::chrono::high_resolution_clock::now(); // end timer

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Time taken: " << duration.count() << " microseconds" << std::endl;

    return 0;
}
