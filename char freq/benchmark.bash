#!/bin/bash

#get all .c files and compile them
for i in *.c; do
    echo "compiling $i"
    gcc -fopenmp $i -O2 -march=native -mtune=native -o ${i%.*}_gcc.out
    clang -fopenmp $i -O2 -march=native -mtune=native -o ${i%.*}_clang.out
done

for p in *.out; do
    echo ""
    echo "-----------running $p-----------"
    echo ""
    for i in 100000000 1000000000 2500000000; do
        echo ""
        echo "---size of array: $i---"
        echo ""
        #generate the input file
        base64 /dev/urandom | head -c $i > file.txt
        #if the file contains seq then only run it once
        if [[ $p == *"seq"* ]]; then
            START=$(date +%s.%N)
            echo "$p, N = $i"
            ./$p file.txt
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
            OMP_NUM_THREADS=$N ./$p file.txt 
            END=$(date +%s.%N)
            printf "\n"
            DIFF=$(echo "$END - $START" | bc)
            echo "Total Time: $DIFF seconds"
            printf "\n"
        done
    done
done