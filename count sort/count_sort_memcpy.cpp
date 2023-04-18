#include <cstdlib>
#include <cstring>
#include <iostream>
#include <omp.h>
#include <fstream>
#include <vector>
#include <string>

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

    //convert vector to array
    int* a = v.data();
    n = v.size();

    //sort
    double start = omp_get_wtime();
    Count_sort(a, n);
    double end = omp_get_wtime();
    std::cout << "Time: " << end - start << std::endl;
    //write to file
    std::ofstream output("output.txt");
    for(int i = 0; i < v.size(); i++){
        output << v[i] << std::endl;
    }
    return 0;

}
