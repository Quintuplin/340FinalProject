#pragma GCC push_options
#pragma GCC optimize ("O0")

#include <stdio.h>    /* for printf */
#include <stdlib.h>   /* for string to integer conversion, random numbers */
#include <time.h> /* for clock stuff */

#define lui long unsigned int

//compare vals for qsort (stackoverflow)
int compare_long( const void* a, const void* b )
{
    if( *(lui*)a == *(lui*)b ) return 0;
    return *(lui*)a < *(lui*)b ? -1 : 1;
}

//stats median
lui median(lui times[], int numtests){
	qsort(times, numtests, sizeof(lui), compare_long);
	
	return times[numtests/2];
}

//stats mean
lui mean(lui times[], int numtests){
	long lui a = 0;
	
	for(int i=0; i<numtests; i++){
		a += times[i];
	}
	
	return (lui)(a/numtests);
}

//base time
lui basetime(int numtests){
	struct timespec start, end;
	lui* times = malloc(numtests * sizeof(lui)); 
	lui diff;
	
	for(int i=0; i<numtests; i++){

		clock_gettime(CLOCK_MONOTONIC, &start);
		clock_gettime(CLOCK_MONOTONIC, &end);
		
		diff = ((1000000000 * (end.tv_sec - start.tv_sec)) + end.tv_nsec) -  start.tv_nsec;
					
		if (diff >= 1000000)printf("%lu ", diff); //detect for massive error condition

		times[i] = diff;
		//printf("c%lu ", diff);
	}
	
	lui temp = mean(times, numtests);
	free(times);
	return temp;
}

//cache time
lui cachetime(int numtests){
	struct timespec start, end;
	lui* times = malloc(numtests * sizeof(lui)); 
	lui diff;
	int a = 1;
	
	for(int i=0; i<numtests; i++){
		a = 1;
		clock_gettime(CLOCK_MONOTONIC, &start);
		a;
		clock_gettime(CLOCK_MONOTONIC, &end);
		
		diff = ((1000000000 * (end.tv_sec - start.tv_sec)) + end.tv_nsec) -  start.tv_nsec;
					
		if (diff >= 1000000)printf("%lu ", diff); //detect for massive error condition

		times[i] = diff;
		//printf("c%lu ", diff);
	}
	
	lui temp = mean(times, numtests);
	free(times);
	return temp;
}

//non cache time
lui noncachetime(int numtests){
	struct timespec start, end;
	
	lui* times = malloc(numtests * sizeof(lui)); 
	lui diff;
	
	for(int i=0; i<numtests; i++){
		clock_gettime(CLOCK_MONOTONIC, &start);
		void* a = malloc(sizeof(int)); //should be uncacheable??
		clock_gettime(CLOCK_MONOTONIC, &end);
		free(a);
		
		diff = ((1000000000 * (end.tv_sec - start.tv_sec)) + end.tv_nsec) -  start.tv_nsec;

		if (diff >= 1000000)printf("%lu ", diff); //detect for massive error condition
		
		times[i] = diff;
		//printf("v%lu ", diff);
	}
	
	lui temp = mean(times, numtests);
	free(times);
	return temp;
}

//block size

//cache size

//main
int main(int argc, char** argv){
	int numtests = atoi(argv[1]);
	
	lui temp = (lui)basetime(numtests);
	
	printf("Cache time = %ld\n", (lui)cachetime(numtests) - temp);
	printf("\n");
	printf("Non-cache time = %ld\n", (lui)noncachetime(numtests) - temp);
	
	return 0;
}

#pragma GCC pop_options