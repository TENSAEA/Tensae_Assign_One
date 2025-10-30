import csv
from pathlib import Path
import matplotlib.pyplot as plt

# Usage: python3 plot_stream.py [stream_results.csv]

csv_path = Path('stream_results.csv')
if not csv_path.exists():
	print('stream_results.csv not found')
	exit(1)

threads, copy, scale, add, triad = [], [], [], [], []
with csv_path.open() as f:
	reader = csv.DictReader(f)
	for r in reader:
		threads.append(int(r['threads']))
		copy.append(float(r['copy']))
		scale.append(float(r['scale']))
		add.append(float(r['add']))
		triad.append(float(r['triad']))

plt.figure(figsize=(7,5))
plt.plot(threads, copy, marker='o', label='Copy')
plt.plot(threads, scale, marker='o', label='Scale')
plt.plot(threads, add, marker='o', label='Add')
plt.plot(threads, triad, marker='o', label='Triad')
plt.xlabel('Threads')
plt.ylabel('MB/s (higher is better)')
plt.title('STREAM bandwidth vs OpenMP threads')
plt.grid(True, linestyle=':')
plt.xticks(threads)
plt.legend()
plt.tight_layout()
plt.savefig('stream_bandwidth.png', dpi=150)
print('Wrote stream_bandwidth.png')
