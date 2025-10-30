import csv
import sys
from pathlib import Path
import matplotlib.pyplot as plt

# Usage: python3 plot_speedup_efficiency.py [pthreads_results.csv] [openmp_results.csv]
# Defaults to files in current directory.

pthreads_csv = Path(sys.argv[1]) if len(sys.argv) > 1 else Path("pthreads_results.csv")
openmp_csv = Path(sys.argv[2]) if len(sys.argv) > 2 else Path("openmp_results.csv")


def read_csv(path: Path):
	threads = []
	speedup = []
	eff = []
	if not path.exists():
		return threads, speedup, eff
	with path.open() as f:
		reader = csv.DictReader(f)
		for row in reader:
			threads.append(int(row["threads"]))
			speedup.append(float(row["speedup"]))
			eff.append(float(row["efficiency"]))
	return threads, speedup, eff


pt_t, pt_s, pt_e = read_csv(pthreads_csv)
om_t, om_s, om_e = read_csv(openmp_csv)

# Create plots

def plot_xy(x, y, title, ylabel, filename):
	plt.figure(figsize=(6,4))
	if x and y:
		plt.plot(x, y, marker='o', linestyle='-', label='Measured')
		plt.plot(x, x, linestyle='--', color='gray', label='Ideal (speedup=threads)' if ylabel=='Speedup' else None)
	plt.title(title)
	plt.xlabel('Threads')
	plt.ylabel(ylabel)
	plt.xticks(x)
	plt.grid(True, linestyle=':')
	plt.legend(loc='best')
	plt.tight_layout()
	plt.savefig(filename, dpi=150)
	print(f"Wrote {filename}")

if pt_t:
	plot_xy(pt_t, pt_s, 'Pthreads Speedup', 'Speedup', 'speedup_pthreads.png')
	plot_xy(pt_t, pt_e, 'Pthreads Efficiency', 'Efficiency', 'efficiency_pthreads.png')
else:
	print("No pthreads CSV found or empty; skipping pthreads plots")

if om_t:
	plot_xy(om_t, om_s, 'OpenMP Speedup', 'Speedup', 'speedup_openmp.png')
	plot_xy(om_t, om_e, 'OpenMP Efficiency', 'Efficiency', 'efficiency_openmp.png')
else:
	print("No openmp CSV found or empty; skipping OpenMP plots")
