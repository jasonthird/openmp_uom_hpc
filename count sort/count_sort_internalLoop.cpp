#include <iostream>
#include <omp.h>
#include <fstream>
#include <vector>
#include <string>


void Count_sort(std::vector<int> &a) {
    int i, j, count;
    std::vector<int> temp(a.size());
    for (i = 0; i < a.size(); i++) {
        count = 0;
        #pragma omp parallel for reduction(+:count) //used bing chat (gpt4) to find that one
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

int main(int argc, char *argv[]){

    //if -b flag is passed then use random numbers in range 0-1000 for benchmarking
    std::vector<int> v;
    long n;
    if (argc == 2){
        std::cout << "please specify the size of the array to sort" << std::endl;
    } else if (argc ==3) {
        switch(argv[1][0]){
            case '-':

                switch(argv[1][1]){
                    case 'b':
                        n = std::stol(argv[2]);
                        v.resize(n);
                        for (int i = 0; i < n; i++){
                            v[i] = (rand() % (1000 + 1));
                        }
                    break;
                    default:
                        std::cout << "invalid flag" << std::endl;
                    break;
                }
                break;
            default:
                std::cout << "invalid flag" << std::endl;
                break;
        }
    }
    else{
        //read from file and store in a vector
        std::ifstream input("input.txt");
        
        std::string line;
        
        if (input.is_open()){
            while (getline(input, line)){
                v.emplace_back(std::stoi(line));
            }
            input.close();
        }
        else std::cout << "Unable to open file";
    }

    //sort
    double start = omp_get_wtime();
    Count_sort(v);
    double end = omp_get_wtime();
    std::cout << "Time: " << end - start << std::endl;
    //write to file
    std::ofstream output("output.txt");
    for(int i = 0; i < v.size(); i++){
        output << v[i] << std::endl;
    }
    return 0;

}