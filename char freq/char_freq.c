#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <omp.h>

#define N 128
#define base 0

int main (int argc, char *argv[]) {
	
	FILE *pFile;
	long file_size;
	char * buffer;
	char * filename;
	size_t result;
	long i, j,*freq;

        if (argc != 2) {
		printf ("Usage : %s <file_name>\n", argv[0]);
		return 1;
        }
	filename = argv[1];
	pFile = fopen ( filename , "rb" );
	if (pFile==NULL) {printf ("File error\n"); return 2;}

	freq = (long*) malloc (sizeof(long)*N);

	// obtain file size:
	fseek (pFile , 0 , SEEK_END);
	file_size = ftell (pFile);
	rewind (pFile);
	printf("file size is %ld\n", file_size);
	
	// allocate memory to contain the file:
	buffer = (char*) malloc (sizeof(char)*file_size);
	if (buffer == NULL) {printf ("Memory error\n"); return 3;}

	// copy the file into the buffer:
	result = fread (buffer,1,file_size,pFile);
	if (result != file_size) {printf ("Reading error\n"); return 4;} 
	
    memset(freq, 0, N*sizeof(int));
    
    //get time before parallel region
    double start = omp_get_wtime();
    #pragma omp parallel for private(i) reduction(+:freq[:N]) firstprivate(buffer, file_size)
	for (i=0; i<file_size; i++){
		freq[buffer[i] - base]++;
	}		
    //get time after parallel region
    double end = omp_get_wtime();

	// for (j=0; j<N; j++){
	// 	printf("%ld = %ld\n", j+base, freq[j]);
	// }	
    printf("Time taken: %f seconds for %ld bytes" , end-start, file_size);

	fclose (pFile);
	free (buffer);

	return 0;
}
