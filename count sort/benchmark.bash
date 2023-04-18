#!/bin/bash

#get all .c files and compile them
for i in *.c; do
    echo "compiling $i"
    gcc -fopenmp $i -O2 -march=native -mtune=native -o ${i%.*}_gcc.out
    clang -fopenmp $i -O2 -march=native -mtune=native -o ${i%.*}_clang.out
done

#get all .cpp files and compile them
for i in *.cpp; do
    echo "compiling $i"
    g++ -fopenmp $i -O2 -march=native -mtune=native -o ${i%.*}_g++.out
    clang++ -fopenmp $i -O2 -march=native -mtune=native -o ${i%.*}_clang++.out
done

#run all the files and output the time
for p in *.out; do
    echo ""
    echo "-----------running $p-----------"
    echo ""
    for i in 20000 80000 100000 300000 500000; do
        echo ""
        echo "---size of array: $i---"
        echo ""
        #if the file contains seq then only run it once
        if [[ $p == *"seq"* ]]; then
            START=$(date +%s.%N)
            echo "$p, N = $i"
            ./$p $i
            END=$(date +%s.%N)
            DIFF=$(echo "$END - $START" | bc)
            echo "Total Time: $DIFF seconds"
            echo ""
            continue
        fi
        for N in 1 2 4 8 16; do
            echo "number of threads: $N"
            if [[ $p == *"++"* ]]; then
                START=$(date +%s.%N)
                OMP_NUM_THREADS=$N ./$p -b $i
                END=$(date +%s.%N)
                DIFF=$(echo "$END - $START" | bc)
                echo "Total Time: $DIFF seconds"
                echo ""
                continue
            fi
            START=$(date +%s.%N)
            OMP_NUM_THREADS=$N ./$p $i 
            END=$(date +%s.%N)
            DIFF=$(echo "$END - $START" | bc)
            echo "Total Time: $DIFF seconds"
            echo ""
        done
    done
done




