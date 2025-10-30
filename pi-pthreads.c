#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>

// Keep the same timing helper as serial
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

// Forward declarations
static double sequentialCompute(long iterations);
static double parallelCompute(long iterations, int numberOfThreads);
static double getDifference(double calculatedPi);

// Thread worker arguments
typedef struct ThreadArgs {
	long startIndex;      // inclusive
	long endIndex;        // exclusive
	double *localSums;    // per-thread accumulation array
	int threadIndex;      // index into localSums
} ThreadArgs;

static void *threadWork(void *arg)
{
	ThreadArgs *args = (ThreadArgs *)arg;
	long start = args->startIndex;
	long end = args->endIndex;
	int t = args->threadIndex;

	double localSum = 0.0;
	// Compute Leibniz series on a subrange
	for (long k = start; k < end; ++k) {
		double sign = (k & 1) ? -1.0 : 1.0;
		localSum += sign / (2.0 * (double)k + 1.0);
	}
	args->localSums[t] = localSum;
	return NULL;
}

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

// Pthreads parallel computation
static double parallelCompute(long iterations, int numberOfThreads)
{
	if (numberOfThreads < 1)
		numberOfThreads = 1;

	pthread_t *threads = (pthread_t *)malloc(sizeof(pthread_t) * (size_t)numberOfThreads);
	ThreadArgs *args = (ThreadArgs *)malloc(sizeof(ThreadArgs) * (size_t)numberOfThreads);
	double *localSums = (double *)calloc((size_t)numberOfThreads, sizeof(double));
	if (!threads || !args || !localSums) {
		printf("Allocation failed\n");
		free(threads); free(args); free(localSums);
		return 0.0;
	}

	long base = iterations / numberOfThreads;
	long rem = iterations % numberOfThreads;
	long cursor = 0;
	for (int t = 0; t < numberOfThreads; ++t) {
		long chunk = base + (t < rem ? 1 : 0);
		args[t].startIndex = cursor;
		args[t].endIndex = cursor + chunk;
		args[t].localSums = localSums;
		args[t].threadIndex = t;
		cursor += chunk;
		pthread_create(&threads[t], NULL, threadWork, &args[t]);
	}
	for (int t = 0; t < numberOfThreads; ++t) {
		pthread_join(threads[t], NULL);
	}
	double sum = 0.0;
	for (int t = 0; t < numberOfThreads; ++t) sum += localSums[t];

	free(threads);
	free(args);
	free(localSums);
	return 4.0 * sum;
}
