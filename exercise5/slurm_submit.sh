#!/usr/bin/env bash
#SBATCH --job-name=matmul_benchmark
#SBATCH --output=/home/fd0007977/out/bench.out.%j
#SBATCH --error=/home/fd0007977/out/bench.err.%j
#SBATCH --time=00:15:00

N=1000000000
THREADS_VALUES=(1 2 4 8 16 32 64)

OUTPUT=/home/fd0007977/out/results.csv

for T in "${THREADS_VALUES[@]}"; do
	for run in {1..3}; do
		./pi_par_loop $N $T >> $OUTPUT
	done
done
