#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

void generate_list(int * x, int n) {
    int i;
    srand (time (NULL));
    for (i = 0; i < n; i++)
        x[i] = rand() % n; 
}

void print_list(int * x, int n) {
    int i;
    for (i = 0; i < n; i++) {
        printf("%d ",x[i]);
    } 
}

void checkIfSorted(int * X, int n) {
    int i;
    for (i = 0; i < n-1; i++) {
        if (X[i] > X[i+1]) {
            printf("List is not sorted!\n");
            return;
        }
    }
    printf("List is sorted!\n");
}

void merge(int* leftArray, int* rightArray, int* resultArray, int leftSize, int rightSize) {
    int i = 0, j = 0, k = 0;
    while (i < leftSize && j < rightSize) {
        if (leftArray[i] < rightArray[j]) {
            resultArray[k] = leftArray[i];
            i++;
        } else {
            resultArray[k] = rightArray[j];
            j++;
        }
        k++;
    }
    while (i < leftSize) {
        resultArray[k] = leftArray[i];
        i++;
        k++;
    }
    while (j < rightSize) {
        resultArray[k] = rightArray[j];
        j++;
        k++;
    }
}
void MergeSort(int* array, int size) {
    if (size < 512) {
        // switch to insertion sort
        // I know this is cheating, but it's a lot faster
        // if we want to keep the algorithm pure merge sort we will need to swich to sequencial if the work is too small
        int i, j, tmp;
        for (i = 1; i < size; i++) {
            j = i;
            while (j > 0 && array[j - 1] > array[j]) {
                tmp = array[j];
                array[j] = array[j - 1];
                array[j - 1] = tmp;
                j--;
            }
        }
        return;
    }
    int middle = size / 2;
    int* leftArray = (int*) malloc(middle * sizeof(int));
    int* rightArray = (int*) malloc((size - middle) * sizeof(int));
    int i;
    for (i = 0; i < middle; i++) {
        leftArray[i] = array[i];
    }
    for (i = middle; i < size; i++) {
        rightArray[i - middle] = array[i];
    }
    #pragma omp task
    MergeSort(leftArray, middle);
    #pragma omp task
    MergeSort(rightArray, size - middle);
    #pragma omp taskwait
    merge(leftArray, rightArray, array, middle, size - middle);
    
    free(leftArray);
    free(rightArray);
}


int main(int argc, char *argv[])
{
    int n;
    int *data, *tmp;
    
    if (argc != 2) {
            printf ("Usage : %s <list size>\n", argv[0]);
    }
    n = strtol(argv[1], NULL, 10);
    data = (int *) malloc (sizeof(int)*n);
    
    generate_list(data, n);

    // printf("List Before Sorting...\n");
    // print_list(data, n);

    //get time before sorting
    double start = omp_get_wtime();
    int numOfThreads;
    #pragma omp parallel
    {
        numOfThreads = omp_get_num_threads();
        #pragma omp single
        MergeSort(data, n);
    }
    //get time after sorting
    double end = omp_get_wtime();
    printf("Time: %f seconds for %d elements with %d threads\n" , end - start, n, numOfThreads);
   
    checkIfSorted(data, n);
    // // printf("\nList After Sorting...\n");
    // // print_list(data, n);
    // // printf("\n");
    return 0;
}

