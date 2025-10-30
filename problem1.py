import csv
from math import log2
from pathlib import Path
import matplotlib.pyplot as plt

# Problem statement:
# T_serial = n^2 (microseconds)
# T_parallel = n^2 / p + log2(p)
# We compute speedup = T_serial / T_parallel and efficiency = speedup / p

n_values = [10 * (2 ** k) for k in range(0, 6)]  # 10,20,40,80,160,320
p_values = [2 ** k for k in range(0, 8)]         # 1,2,4,8,16,32,64,128

rows = [("n","p","T_serial","T_parallel","speedup","efficiency")]
for n in n_values:
	for p in p_values:
		T_serial = n * n
		T_parallel = (n * n) / p + (0.0 if p == 1 else log2(p))
		speedup = T_serial / T_parallel
		eff = speedup / p
		rows.append((n, p, T_serial, T_parallel, speedup, eff))

out_csv = Path("problem1_results.csv")
with out_csv.open("w", newline="") as f:
	writer = csv.writer(f)
	writer.writerows(rows)
print(f"Wrote {out_csv}")

# Plot 1: Speedup vs p for each fixed n
plt.figure(figsize=(7,5))
for n in n_values:
	y = []
	for p in p_values:
		T_serial = n * n
		T_parallel = (n * n) / p + (0.0 if p == 1 else log2(p))
		y.append(T_serial / T_parallel)
	plt.plot(p_values, y, marker='o', label=f"n={n}")
plt.xscale('log', base=2)
plt.xticks(p_values, p_values)
plt.xlabel('Threads p (log2 scale)')
plt.ylabel('Speedup')
plt.title('Problem 1a: Speedup vs p for fixed n')
plt.grid(True, linestyle=':')
plt.legend()
plt.tight_layout()
plt.savefig('p1_speedup_vs_p.png', dpi=150)
print("Wrote p1_speedup_vs_p.png")

# Plot 2: Efficiency vs p for each fixed n
plt.figure(figsize=(7,5))
for n in n_values:
	y = []
	for p in p_values:
		T_serial = n * n
		T_parallel = (n * n) / p + (0.0 if p == 1 else log2(p))
		speedup = T_serial / T_parallel
		y.append(speedup / p)
	plt.plot(p_values, y, marker='o', label=f"n={n}")
plt.xscale('log', base=2)
plt.xticks(p_values, p_values)
plt.xlabel('Threads p (log2 scale)')
plt.ylabel('Efficiency')
plt.title('Problem 1a: Efficiency vs p for fixed n')
plt.grid(True, linestyle=':')
plt.legend()
plt.tight_layout()
plt.savefig('p1_efficiency_vs_p.png', dpi=150)
print("Wrote p1_efficiency_vs_p.png")

# Plot 3: Speedup vs n for a few fixed p values
plt.figure(figsize=(7,5))
for p in [1,2,4,8,16,32,64,128]:
	y = []
	for n in n_values:
		T_serial = n * n
		T_parallel = (n * n) / p + (0.0 if p == 1 else log2(p))
		y.append(T_serial / T_parallel)
	plt.plot(n_values, y, marker='o', label=f"p={p}")
plt.xticks(n_values, n_values)
plt.xlabel('Problem size n')
plt.ylabel('Speedup')
plt.title('Problem 1a: Speedup vs n for fixed p')
plt.grid(True, linestyle=':')
plt.legend()
plt.tight_layout()
plt.savefig('p1_speedup_vs_n.png', dpi=150)
print("Wrote p1_speedup_vs_n.png")

# Textual conclusions for (a) are to be added to questions.md
# Part (b) conclusions will be added to questions.md as prose.
