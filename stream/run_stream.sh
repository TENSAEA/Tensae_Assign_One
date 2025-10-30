#!/usr/bin/env bash
set -euo pipefail

# Usage: ./run_stream.sh ARRAY_SIZE
# Runs STREAM for threads 1,2,4,8,16 and writes stream_results.csv

if [[ $# -lt 1 ]]; then
	echo "Usage: $0 ARRAY_SIZE" >&2
	exit 1
fi

ARRAY_SIZE="$1"
THREADS=(1 2 4 8 16)

# Build
ARRAY_SIZE="$ARRAY_SIZE" make -s clean all

# CSV header
echo "threads,copy,scale,add,triad" > stream_results.csv

for t in "${THREADS[@]}"; do
	# Run and parse MB/s lines
	OUT=$(OMP_NUM_THREADS="$t" ./stream)
	copy=$(echo "$OUT" | awk '/Copy/{print $2}')
	scale=$(echo "$OUT" | awk '/Scale/{print $2}')
	add=$(echo "$OUT" | awk '/Add/{print $2}')
	triad=$(echo "$OUT" | awk '/Triad/{print $2}')
	echo "$t,$copy,$scale,$add,$triad" >> stream_results.csv
	echo "t=$t -> Copy=$copy MB/s Scale=$scale MB/s Add=$add MB/s Triad=$triad MB/s"
done
