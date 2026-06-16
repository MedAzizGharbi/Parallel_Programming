#!/bin/bash
#SBATCH --job-name=matmul_benchmark
#SBATCH --output=/home/fd0007977/out/bench.out.%j
#SBATCH --error=/home/fd0007977/out/bench.err.%j
#SBATCH --time=00:05:00
#SBATCH --cpus-per-task=8

export OMP_NUM_THREADS=$SLURM_CPUS_PER_TASK

N_VALUES=(500 1000 2000)
THREADS_VALUES=(1 2 4 8)

OUTPUT=/home/fd0007977/out/results.csv

for N in "${N_VALUES[@]}"; do
    for T in "${THREADS_VALUES[@]}"; do
        for run in {1..3}; do
            ./matmul $N $T >> $OUTPUT
        done
    done
done
