#!/usr/bin/env bash
set -euo pipefail

# Usage: ./find_iters.sh [start]
# Compiles pi-pthreads and finds a number of iterations so that
# the sequential part runs at least 5 seconds.

START=${1:-1000000}

# Build dependency first
gcc -O3 -march=native -pthread -o pi-pthreads pi-pthreads.c

iters=$START
while : ; do
	OUT=$(./pi-pthreads "$iters" 1)
	seq_time=$(echo "$OUT" | awk -F 'Sequential Time: ' '/Sequential Time:/ {print $2}' | awk -F ',' '{print $1}')
	# If seq_time is empty, avoid infinite loop
	if [[ -z "$seq_time" ]]; then
		echo "Failed to parse sequential time" >&2
		echo "$OUT" >&2
		exit 1
	fi
	awk_check=$(python3 - <<PY
val=float("$seq_time")
print(1 if val >= 5.0 else 0)
PY
)
	if [[ "$awk_check" == "1" ]]; then
		break
	fi
	iters=$(( iters * 2 ))
done

echo "Suggested ITERS: $iters (sequential_time ~ $seq_time s)"
