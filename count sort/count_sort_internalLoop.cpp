#include <iostream>
#include <omp.h>
#include <fstream>
#include <vector>
#include <string>

#define NUM_THREADS 16

void Count_sort(std::vector<int> &a) {
    int i, j, count;
    std::vector<int> temp(a.size());
    for (i = 0; i < a.size(); i++) {
        count = 0;
        #pragma omp parallel for reduction(+:count) //use bing chat (gpt4) to find that one
        //just gave it the code of this function and told it to use openmp on the inner loop
        //seems to be almost the same as parallelizing the outer loop on gcc, on clang is a lot slower but both achieve similar speeds
        for (j = 0; j < a.size(); j++)
            if (a[j] < a[i])
                count++;
            else if (a[j] == a[i] && j < i)
                count++;
        temp[count] = a[i];
    }
    a.swap(temp);
    temp.clear();
}

int main(){
    omp_set_num_threads(NUM_THREADS);
    
    //read from file and store in a vector
    std::ifstream input("input.txt");
    std::vector<int> v;
    std::string line;
    
    if (input.is_open()){
        while (getline(input, line)){
            v.emplace_back(std::stoi(line));
        }
        input.close();
    }
    else std::cout << "Unable to open file";


    //sort
    Count_sort(v);

    //write to file
    std::ofstream output("output.txt");
    for(int i = 0; i < v.size(); i++){
        output << v[i] << std::endl;
    }
    return 0;

}
