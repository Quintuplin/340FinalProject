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
/*
//stats normalize
void normalise(lui **times, int numtests){
	qsort(*times, numtests, sizeof(lui), compare_long);
	lui max = (lui)times[0];
	lui min = (lui)times[numtests-1];
	
	lui nmax = 1000; lui nmin = 0;
	
	for(int i=0; i<numtests; i++){
		lui temp = nmin + (*(times+i) - min)(nmax-nmin)/(max-min);
		*times[i] = temp;
	}
	
	return;
}*/

//stats mean
lui mean(lui times[], int numtests){
	long lui a = 0;
	
	for(int i=0; i<numtests; i++){
		a += times[i];
	}
	
	return (lui)(a/numtests);
}

//stats median
lui median(lui times[], int numtests){
	qsort(times, numtests, sizeof(lui), compare_long);
	
	return times[numtests/2];
}

//stats mode
lui mode(lui times[], int numtests){
	qsort(times, numtests, sizeof(lui), compare_long);
	
	lui temp = times[0];
	int nums = 0;
	int curmax = 0;
	int curmaxloc = 0;
	
	for(int i=0; i<numtests; i++){
		if(times[i]==temp) nums++;
		else{
			if(nums>curmax){
				curmaxloc = (i-1);
				curmax = nums;
			}
			nums = 0;
			temp = times[i];
		}
	}
	
	return times[curmaxloc];
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
lui blocksize(int numtests){
	struct timespec start, end;
	
	lui* times = malloc(numtests * sizeof(lui)); 
	lui diff;
	
	for(int i=0; i<numtests; i++){
		clock_gettime(CLOCK_MONOTONIC, &start);
		times[i];
		clock_gettime(CLOCK_MONOTONIC, &end);
		
		diff = ((1000000000 * (end.tv_sec - start.tv_sec)) + end.tv_nsec) -  start.tv_nsec;
		if (diff >= 1000000)printf("%lu ", diff); //detect for massive error condition
		times[i] = diff;
	}
	
	lui temp = mean(times, numtests);
	lui* len = malloc(numtests * sizeof(lui)); 
	int j=0;
	
	for(int i=0; i<numtests; i++){
		if(times[i]<temp) len[j]++;
		else j++;
	}
	
	free(times);
	
	lui temp2 = mode(len, j);
	free(len);
	
	return temp2;
}
//cache size

//main
int main(int argc, char** argv){
	int numtests = atoi(argv[1]);
	
	lui temp = (lui)basetime(numtests);
	
	printf("Cache time = %ld\n", cachetime(numtests) - temp);
	printf("\n");
	printf("Non-cache time = %ld\n", noncachetime(numtests) - temp);
	printf("\n");
	printf("Block size = %ld\n", blocksize(numtests));
	
	return 0;
}

#pragma GCC pop_options