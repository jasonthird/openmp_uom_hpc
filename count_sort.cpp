#include <cstdlib>
#include <cstring>
#include <iostream>
#include <omp.h>
#include <fstream>
#include <vector>
#include <string>

#define NUM_THREADS 16

void Count_sort(std::vector<int> &a) {
    int i, j, count;
    std::vector<int> temp(a.size());
    #pragma omp parallel for private(i, j, count) shared(a, temp)
    for (i = 0; i < a.size(); i++) {
        count = 0;
        for (j = 0; j < a.size(); j++)
            if (a[j] < a[i])
                count++;
            else if (a[j] == a[i] && j < i)
                count++;
        temp[count] = a[i];
    }
    //wanted to avoid memcpy
    //tryed swaping the pointers of arrays at first but sagment faults when using gcc, worked on clang though
    //thats why I switched to vectors. they are much easier to work with
    //this might seem more efficient but its accually it doesn't really matter
    //spent too much time on this soo Ill just leave it like this
    a.swap(temp);
    temp.clear();
}

int main(){
    omp_set_num_threads(NUM_THREADS);
    
    //read from file and store in a vector
    std::ifstream input("input.txt");
    std::vector<int> v;
    
    //read from file and store in a vector
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