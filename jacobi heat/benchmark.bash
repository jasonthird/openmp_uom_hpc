#!/bin/bash
#this scripts needs sudo permissions to drop the cache

#get all .c files and compile them
for i in *.c; do
    echo "compiling $i"
    gcc -fopenmp $i -O2 -march=native -mtune=native -o ${i%.*}_gcc.out -lm
    clang -fopenmp $i -O2 -march=native -mtune=native -o ${i%.*}_clang.out -lm
done

#get all .cpp files and compile them
for i in *.cpp; do
    echo "compiling $i"
    g++ -fopenmp $i -O2 -march=native -mtune=native -o ${i%.*}_g++.out
    clang++ -fopenmp $i -O2 -march=native -mtune=native -o ${i%.*}_clang++.out
done

for p in *.out; do
    echo ""
    echo "-----------running $p-----------"
    echo ""
    #if the file contains seq then only run it once
    if [[ $p == *"seq"* ]]; then
        sync
        sudo sh -c "echo  3 > /proc/sys/vm/drop_caches"
        START=$(date +%s.%N)
        echo "$p, N = $i"
        ./$p
        END=$(date +%s.%N)
        echo ""
        DIFF=$(echo "$END - $START" | bc)
        echo "Total Time: $DIFF seconds"
        echo""
        continue
    fi
    for N in 1 2 4 8 16; do
        sync
        sudo sh -c "echo  3 > /proc/sys/vm/drop_caches"
        echo "number of threads: $N"
        START=$(date +%s.%N)
        OMP_NUM_THREADS=$N ./$p
        END=$(date +%s.%N)
        echo ""
        DIFF=$(echo "$END - $START" | bc)
        echo "Total Time: $DIFF seconds"
        echo""
    done
done

#the julia one
for N in 1 2 4 8 16; do
    echo "number of threads: $N"
    sync
    sudo sh -c "echo  3 > /proc/sys/vm/drop_caches"
    START=$(date +%s.%N)
    julia -t $N jacobi_heat_julia.jl
    END=$(date +%s.%N)
    echo ""
    DIFF=$(echo "$END - $START" | bc)
    echo "Total Time: $DIFF seconds"
    echo""
done