#!/bin/bash

#get all .c files and compile them
for i in *.c; do
    echo "compiling $i"
    gcc -fopenmp $i -O2 -march=native -mtune=native -o ${i%.*}_gcc.out
    clang -fopenmp $i -O2 -march=native -mtune=native -o ${i%.*}_clang.out
done

#get all .f90 files and compile them
for i in *.f90; do
    echo "compiling $i"
    flang -fopenmp $i -O2 -march=native -mtune=native -o ${i%.*}_fortran.out
done

for p in *.out; do
    echo ""
    echo "-----------running $p-----------"
    echo ""
    for i in 50000000 100000000 2000000000; do
        #generate the input file
        base64 /dev/urandom | head -c $i > file.txt
        for s in 312 xd openmp; do
                echo ""
        echo "---size of array and string to match: $i+$s---"
        echo ""
        #if the file contains seq then only run it once
            if [[ $p == *"seq"* ]]; then
                echo "$p, N = $i"
                START=$(date +%s.%N)
                ./$p file.txt $s
                END=$(date +%s.%N)
                DIFF=$(echo "$END - $START" | bc)
                printf "\n"
                echo "Total Time: $DIFF seconds"
                printf "\n"
                continue
            fi
            for N in 1 2 4 8 16; do
                echo "number of threads: $N"
                START=$(date +%s.%N)
                OMP_NUM_THREADS=$N ./$p file.txt $s
                END=$(date +%s.%N)
                printf "\n"
                DIFF=$(echo "$END - $START" | bc)
                echo "Total Time: $DIFF seconds"
                printf "\n"
            done
        done
    done
done