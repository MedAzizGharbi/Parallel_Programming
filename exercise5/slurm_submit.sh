#!/usr/bin/env bash
#SBATCH --job-name=matmul_benchmark
#SBATCH --output=/home/fd0007977/out/bench.out.%j
#SBATCH --error=/home/fd0007977/out/bench.err.%j
#SBATCH --time=00:15:00
#SBATCH --cpus-per-task=64


OUTPUT=/home/fd0007977/out/results_mandelBrot.csv

echo "w,h,max_iter,threads,schedule,chunk,run,time" > "$OUTPUT"

W=2000
H=2000
MAX_ITER=10000

for P in 1 2 4 8 16 32 64; do
	for REP in 1 2 3; do
		for SCHED in static dynamic guided; do
			if [ "$SCHED" = "guided" ]; then
				CHUNKS="1"
			else
				CHUNKS="1 16"
			fi

			for CHUNK in $CHUNKS; do
				./mandelbrot $W $H $MAX_ITER $P $SCHED $CHUNK >> "$OUTPUT"
			done
		done
	done
done

