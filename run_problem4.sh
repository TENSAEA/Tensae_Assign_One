#!/usr/bin/env bash
set -euo pipefail

# One-shot runner for Problem 4 (Pthreads + OpenMP pi)
# - Calibrates ITERS (>=5s sequential)
# - Builds and runs both variants for threads 1,2,4,8,16
# - Generates speedup/efficiency plots
# Optional arg: starting iterations for calibration (default 1,000,000)

START_ITERS=${1:-1000000}

ROOT_DIR=$(cd "$(dirname "$0")" && pwd)
cd "$ROOT_DIR"

# Ensure scripts are executable
chmod +x find_iters.sh run_pthreads.sh run_openmp.sh || true

# Calibrate iterations (parse number from output)
CAL_OUT=$(./find_iters.sh "$START_ITERS")
echo "$CAL_OUT"
ITERS=$(echo "$CAL_OUT" | awk '/Suggested ITERS:/ {print $3}')
if [[ -z "${ITERS}" ]]; then
	echo "Failed to determine ITERS" >&2
	exit 1
fi

echo "Using ITERS=$ITERS"

# Run experiments
./run_pthreads.sh "$ITERS"
./run_openmp.sh "$ITERS"

# Plot
python3 plot_speedup_efficiency.py pthreads_results.csv openmp_results.csv

# Summary
echo "--- Generated files ---"
ls -1 pthreads_results.csv openmp_results.csv speedup_*.png efficiency_*.png 2>/dev/null || true
