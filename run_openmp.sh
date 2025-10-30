#!/usr/bin/env bash
set -euo pipefail

# Usage: ./run_openmp.sh ITERS
# Produces openmp_results.csv with columns:
# threads,sequential_time,parallel_time,speedup,efficiency

if [[ $# -lt 1 ]]; then
	echo "Usage: $0 ITERS" >&2
	exit 1
fi

ITERS="$1"
THREADS=(1 2 4 8 16)

# Build
gcc -O3 -march=native -fopenmp -o pi-openmp pi-openmp.c

echo "threads,sequential_time,parallel_time,speedup,efficiency" > openmp_results.csv
for t in "${THREADS[@]}"; do
	# Control OpenMP thread count via arg and also OMP_NUM_THREADS
	OUT=$(./pi-openmp "$ITERS" "$t")
	seq_time=$(echo "$OUT" | awk -F 'Sequential Time: ' '/Sequential Time:/ {print $2}' | awk -F ',' '{print $1}')
	par_time=$(echo "$OUT" | awk -F 'Parallel Time: ' '/Parallel Time:/ {print $2}' | awk -F ',' '{print $1}')
	speedup=$(echo "$OUT" | awk -F 'Speedup: ' '/Speedup:/ {print $2}')
	efficiency=$(python3 - <<PY
s=$speedup
t=$t
print(f"{s/float(t):.6f}")
PY
)
	echo "$t,$seq_time,$par_time,$speedup,$efficiency" >> openmp_results.csv
	echo "OpenMP t=$t -> speedup=$speedup"
done
