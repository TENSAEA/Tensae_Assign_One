#!/usr/bin/env bash
set -euo pipefail

# Usage: ./run_pthreads.sh ITERS
# Produces pthreads_results.csv with columns:
# threads,sequential_time,parallel_time,speedup,efficiency

if [[ $# -lt 1 ]]; then
	echo "Usage: $0 ITERS" >&2
	exit 1
fi

ITERS="$1"
THREADS=(1 2 4 8 16)

# Build
gcc -O3 -march=native -pthread -o pi-pthreads pi-pthreads.c

echo "threads,sequential_time,parallel_time,speedup,efficiency" > pthreads_results.csv
for t in "${THREADS[@]}"; do
	OUT=$(./pi-pthreads "$ITERS" "$t")
	seq_time=$(echo "$OUT" | awk -F 'Sequential Time: ' '/Sequential Time:/ {print $2}' | awk -F ',' '{print $1}')
	par_time=$(echo "$OUT" | awk -F 'Parallel Time: ' '/Parallel Time:/ {print $2}' | awk -F ',' '{print $1}')
	speedup=$(echo "$OUT" | awk -F 'Speedup: ' '/Speedup:/ {print $2}')
	# Compute efficiency = speedup / t
	efficiency=$(python3 - <<PY
s=$speedup
t=$t
print(f"{s/float(t):.6f}")
PY
)
	echo "$t,$seq_time,$par_time,$speedup,$efficiency" >> pthreads_results.csv
	echo "Pthreads t=$t -> speedup=$speedup"
done
