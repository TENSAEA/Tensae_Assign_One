# Pi Parallel Assignment: Build and Run Guide

## Build

```bash
gcc -O3 -march=native -pthread -o pi-pthreads pi-pthreads.c
gcc -O3 -march=native -fopenmp -o pi-openmp pi-openmp.c
```

## Calibrate iterations (≥5 seconds sequential)

```bash
chmod +x find_iters.sh
./find_iters.sh 1000000
```

Use the suggested ITERS value in the run scripts below.

## Run experiments and collect CSVs

```bash
chmod +x run_pthreads.sh run_openmp.sh
./run_pthreads.sh ITERS
./run_openmp.sh ITERS
```

This produces `pthreads_results.csv` and `openmp_results.csv` with columns:
`threads,sequential_time,parallel_time,speedup,efficiency`.

## Plot

```bash
python3 plot_speedup_efficiency.py pthreads_results.csv openmp_results.csv
```

Outputs:
- `speedup_pthreads.png`, `efficiency_pthreads.png`
- `speedup_openmp.png`, `efficiency_openmp.png`

## Notes
- Both programs print approximation accuracy and timing.
- Efficiency = speedup / threads.
- Lines of code (rough estimate):

```bash
wc -l pi-serial.c pi-pthreads.c pi-openmp.c
```
