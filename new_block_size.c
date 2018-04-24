#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define DIFF(start, end) 1000000000 * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec

void blocksize(float *data, int trials) {
	struct timespec start, end;
	int *memory = malloc(100000 * sizeof(int));
	int dummy = 0;

	for (int trial = 0; trial < trials; trial++) {
		int i = 0;
		clock_gettime(CLOCK_MONOTONIC, &start);
		for (; i < 100000; i++) {
			dummy += memory[trial];
		}
		clock_gettime(CLOCK_MONOTONIC, &end);

		int diff = DIFF(start, end);
		data[trial] += (float) diff / 100000;
		if (data[trial] > 10)
			printf("%6d  %.2f\n", trial, data[trial]);
	}

	free(memory);
}

#define INITIAL 1
#define FINAL 8

void betterblocksize(float *data, int trials) {
	struct timespec start, end;
	for (int trial = 0; trial < trials; trial++) {
		for (int n = INITIAL; n <= FINAL; n++) {
			int power = 2 << (n - 1);
			int max = 1000 * power;
			int *memory = malloc(max * sizeof(int));

			int dummy, i = 0;

			clock_gettime(CLOCK_MONOTONIC, &start);
			for (; i < max; i += power) {
				dummy = memory[i];
			}
			clock_gettime(CLOCK_MONOTONIC, &end);

			if (trial == 0) data[n - INITIAL] = DIFF(start, end);
			else data[n - INITIAL] += DIFF(start, end);
			//printf("n = %d, diff = %ld, cycles = %ld\n", n, DIFF(start, end));

			free(memory);
		}
	}

	for (int n = INITIAL; n < 9; n++) {
		data[n - INITIAL] = (float) data[n - INITIAL] / 1000 / trials;
	}
}

int main(int argc, char **argv) {
	// remember to use calloc, not malloc
	int trials = atoi(argv[1]);
	float *data = calloc(trials, sizeof(float));
	betterblocksize(data, trials);

	for (int i = INITIAL; i <= FINAL; i++)
		printf("2^%d (%d) -> %.2f\n", i, 2 << (i - 1), data[i - INITIAL]);

	free(data);
	return 0;
}