#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <omp.h>

static double getTime()
{
	const double kMicro = 1.0e-6;
	struct timeval TV;
	const int rc = gettimeofday(&TV, NULL);
	if (rc == -1) {
		printf("ERROR: Bad call to gettimeofday\n");
		return -1.0;
	}
	return ((double)TV.tv_sec) + kMicro * ((double)TV.tv_usec);
}

static double sequentialCompute(long iterations);
static double parallelCompute(long iterations, int numberOfThreads);
static double getDifference(double calculatedPi);

int main(int argc, char *argv[])
{
	double sequentialStart, sequentialEnd, sequentialTime;
	double parallelStart, parallelEnd, parallelTime;
	double sequentialPi, parallelPi;
	double sequentialDifference, parallelDifference;
	long iterations;
	int numberOfThreads;

	if (argc > 2) {
		iterations = strtol(argv[1], NULL, 10);
		numberOfThreads = strtol(argv[2], NULL, 10);
	} else {
		printf("\nWhen running this program, please include number of iterations and number of threads on command line.\n");
		return 0;
	}

	sequentialStart = getTime();
	sequentialPi = sequentialCompute(iterations);
	sequentialEnd = getTime();
	sequentialTime = sequentialEnd - sequentialStart;

	parallelStart = getTime();
	parallelPi = parallelCompute(iterations, numberOfThreads);
	parallelEnd = getTime();
	parallelTime = parallelEnd - parallelStart;

	sequentialDifference = getDifference(sequentialPi);
	parallelDifference = getDifference(parallelPi);

	printf("Sequential Calculation: %f\n", sequentialPi);
	printf("This is %f away from the correct value of PI.\n\n", sequentialDifference);
	printf("ParallelCalculation: %f\n", parallelPi);
	printf("This is %f away from the correct value of PI.\n\n", parallelDifference);
	printf("Number of iterations: %ld, Number of Threads: %d\n\n", iterations, numberOfThreads);

	double difference = parallelDifference - sequentialDifference;
	if (difference < .01 && difference > -.01)
		printf("Parallel calculation is VALID!\n");
	else
		printf("Parallel calculation is INVALID!\n");

	double speedup = ((double)sequentialTime) / ((double)parallelTime);
	printf("Sequential Time: %f, Parallel Time: %f, Speedup: %f\n", sequentialTime, parallelTime, speedup);
	return 0;
}

static double sequentialCompute(long iterations)
{
	double factor = 1.0;
	double sum = 0.0;
	for (long k = 0; k < iterations; ++k) {
		sum += factor / (2.0 * (double)k + 1.0);
		factor = -factor;
	}
	return 4.0 * sum;
}

static double getDifference(double calculatedPi)
{
	return calculatedPi - 3.14159265358979323846;
}

static double parallelCompute(long iterations, int numberOfThreads)
{
	if (numberOfThreads < 1)
		numberOfThreads = 1;
	omp_set_num_threads(numberOfThreads);
	double sum = 0.0;
	#pragma omp parallel for reduction(+:sum) schedule(static)
	for (long k = 0; k < iterations; ++k) {
		double sign = (k & 1) ? -1.0 : 1.0;
		sum += sign / (2.0 * (double)k + 1.0);
	}
	return 4.0 * sum;
}
