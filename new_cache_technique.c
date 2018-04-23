#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define DIFF(start, end) 1000000000 * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec
#define ACCESSES 10000

int main(int argc, char **argv) {
	int trials = atoi(argv[1]);
	int *data = malloc(trials * sizeof(int));
	for (int trial = 0; trial < trials; trial++) {
		struct timespec start, end;
		int a = 1, i = 0;
		clock_gettime(CLOCK_MONOTONIC, &start);
		for (; i < ACCESSES; i++) a;
		clock_gettime(CLOCK_MONOTONIC, &end);
		int diff = DIFF(start, end);
		data[trial] = diff / ACCESSES;
		printf("%2d ", diff / ACCESSES); // remove eventually
	}
	free(data);
	return 0;
}
