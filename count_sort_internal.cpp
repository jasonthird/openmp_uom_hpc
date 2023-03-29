#include <cstdlib>
#include <cstring>
#include <iostream>
#include <omp.h>
#include <fstream>
#include <vector>
#include <string>

#define NUM_THREADS 16

void Count_sort(int a[], int n) {
    int i, j, count;
    int* temp = (int*) std::malloc(n*sizeof(int));
    #pragma omp parallel for private(i, j, count) shared(a, temp)
    for (i = 0; i < n; i++) {
        count = 0;
        for (j = 0; j < n; j++)
            if (a[j] < a[i])
                count++;
            else if (a[j] == a[i] && j < i)
                count++;
        temp[count] = a[i];
    }
    std::memcpy(a, temp, n*sizeof(int));
    free(temp);
}

int main(){
    omp_set_num_threads(16);
    
    //read from file and store in a vector
    std::ifstream input("input.txt");
    std::vector<int> v;
    
    //read from file and store in a vector
    std::string line;
    
    if (input.is_open()){
        while (getline(input, line)){
            v.push_back(std::stoi(line));
        }
        input.close();
    }
    else std::cout << "Unable to open file";

    //convert vector to array
    int* a = v.data();
    int n = v.size();

    //sort
    Count_sort(a, n);

    //write to file
    std::ofstream output("output.txt");
    for(int i = 0; i < n; i++){
        output << a[i] << std::endl;
    }
    return 0;

}