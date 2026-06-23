#!/bin/bash
OUT="results.csv"
echo "N,threads,schedule,chunk,time_kernel,best_n,best_len" > "$OUT"

N=100000000

THREADS=(1 2 4 8)

REPS=(1 2 3)

declare -a CONFIGS=(
	"static 1"
	"static 16"
	"dynamic 1"
	"dynamic 16"
	"guided 1"
)


for P in "${THREADS[@]}"; do
	for cfg in "${CONFIGS[@]}"; do
		read sched chunk <<< "$cfg"
		for rep in "${REPS[@]}"; do
			output=$(./collatz "$N" "$P" "$sched" "$chunk")
			if [ "$rep" -eq 3 ]; then
				echo "$output" >> "$OUT"
			fi
		done
	done
done

