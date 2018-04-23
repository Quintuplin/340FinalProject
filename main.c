#pragma GCC push_options
#pragma GCC optimize ("O0")

#include <stdio.h>    /* for printf */
#include <stdlib.h>   /* for string to integer conversion, random numbers */
#include <time.h> /* for clock stuff */

#define DIFF(start, end) 1000000000 * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec

//compare vals for qsort (stackoverflow)
int compare_float( const void* a, const void* b )
{
    if( *(int*)a == *(int*)b ) return 0;
    return *(int*)a < *(int*)b ? -1 : 1;
}
/*
//stats normalize
void normalise(float **times, int numtests){
	qsort(*times, numtests, sizeof(float), compare_float);
	float max = (float)times[0];
	float min = (float)times[numtests-1];
	
	float nmax = 1000; float nmin = 0;
	
	for(int i=0; i<numtests; i++){
		float temp = nmin + (*(times+i) - min)(nmax-nmin)/(max-min);
		*times[i] = temp;
	}
	
	return;
}*/

//stats mean
float mean(float times[], int numtests){
	float a = 0;
	
	for(int i=0; i<numtests; i++){
		a += times[i];
		//printf("%f ", a);
	}
	
	return (float)(a/numtests);
}

//stats median
float median(float times[], int numtests){
	qsort(times, numtests, sizeof(float), compare_float);
	
	return times[numtests/2];
}

//stats mode
float mode(float times[], int numtests){
	qsort(times, numtests, sizeof(float), compare_float);
	
	float temp = times[0];
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
float basetime(int numtests){
	struct timespec start, end;
	float* times = malloc(numtests * sizeof(float)); 
	float diff;
	
	for(int i=0; i<numtests; i++){

		clock_gettime(CLOCK_MONOTONIC, &start);
		clock_gettime(CLOCK_MONOTONIC, &end);
		
		diff = ((1000000000 * (end.tv_sec - start.tv_sec)) + end.tv_nsec) -  start.tv_nsec;
					
		if (diff >= 1000000)printf("%f ", diff); //detect for massive error condition

		times[i] = diff;
		//printf("c%f ", diff);
	}
	
	float temp = mean(times, numtests);
	free(times);
	return temp;
}

/*//cache time
float cachetime(int numtests){
	struct timespec start, end;
	float* times = malloc(numtests * sizeof(float)); 
	float diff;
	int a = 1;
	
	for(int i=0; i<numtests; i++){
		a = 1;
		clock_gettime(CLOCK_MONOTONIC, &start);
		a;
		clock_gettime(CLOCK_MONOTONIC, &end);
		
		diff = ((1000000000 * (end.tv_sec - start.tv_sec)) + end.tv_nsec) -  start.tv_nsec;
					
		if (diff >= 1000000)printf("%f ", diff); //detect for massive error condition

		times[i] = diff;
		//printf("c%f ", diff);
	}
	
	float temp = mean(times, numtests);
	free(times);
	return temp;
}
*/

//better cache time
float bettercache(int numtests){
	float *data = malloc(numtests * sizeof(float));
	for (int trial = 0; trial < numtests; trial++) {
		struct timespec start, end;
		int a = 1, i = 0;
		clock_gettime(CLOCK_MONOTONIC, &start);
		for (; i < numtests; i++) 
			a;
		clock_gettime(CLOCK_MONOTONIC, &end);
		float diff = DIFF(start, end);
		data[trial] = (float)diff / numtests;
		//printf("%2d ", diff / numtests); // remove eventually
	}
	
	printf("Better cache median = %f\n", median(data, numtests));
	printf("Better cache mode = %f\n", mode(data, numtests));
	float temp = mean(data, numtests);
	free(data);
	return temp;
}

//better non cache time
float betternoncache(int numtests){
	float *data = malloc(numtests * sizeof(float));
	for (int trial = 0; trial < numtests; trial++) {
		struct timespec start, end;
		int i = 0;
		clock_gettime(CLOCK_MONOTONIC, &start);
		for (; i < numtests; i++)
			data[(i+numtests/2)%numtests];
		clock_gettime(CLOCK_MONOTONIC, &end);
		float diff = DIFF(start, end);
		data[trial] = (float)diff / numtests;
		//printf("%2d ", diff / numtests); // remove eventually
	}
	
	printf("Better noncache median = %f\n", median(data, numtests));
	printf("Better noncache mode = %f\n", mode(data, numtests));
	float temp = mean(data, numtests);
	free(data);
	return temp;
}
//non cache time
float noncachetime(int numtests){
	struct timespec start, end;
	
	float* times = malloc(numtests * sizeof(float)); 
	float diff;
	
	for(int i=0; i<numtests; i++){
		clock_gettime(CLOCK_MONOTONIC, &start);
		void* a = malloc(sizeof(int)); //shouf be uncacheable??
		clock_gettime(CLOCK_MONOTONIC, &end);
		free(a);
		
		diff = ((1000000000 * (end.tv_sec - start.tv_sec)) + end.tv_nsec) -  start.tv_nsec;

		if (diff >= 1000000)printf("%f ", diff); //detect for massive error condition
		
		times[i] = diff;
		//printf("v%f ", diff);
	}
	
	float temp = mean(times, numtests);
	free(times);
	return temp;
}

//block size
float blocksize(int numtests){
	struct timespec start, end;
	
	float* times = malloc(numtests * sizeof(float)); 
	float diff;
	
	for(int i=0; i<numtests; i++){
		clock_gettime(CLOCK_MONOTONIC, &start);
		times[i];
		clock_gettime(CLOCK_MONOTONIC, &end);
		
		diff = ((1000000000 * (end.tv_sec - start.tv_sec)) + end.tv_nsec) -  start.tv_nsec;
		if (diff >= 1000000)printf("%f ", diff); //detect for massive error condition
		times[i] = diff;
	}
	
	float temp = mean(times, numtests);
	float* len = malloc(numtests * sizeof(float)); 
	int j=0;
	
	for(int i=0; i<numtests; i++){
		if(times[i]<temp) len[j]++;
		else j++;
	}
	
	free(times);
	
	float temp2 = mode(len, j);
	free(len);
	
	return temp2;
}
//cache size

//main
int main(int argc, char** argv){
	int numtests = atoi(argv[1]);
	
	float temp = (float)basetime(numtests);
	
	//printf("Cache time = %f\n", cachetime(numtests) - temp);
	printf("\n");
	printf("Better cache time mean = %f\n", bettercache(numtests));
	printf("\n");
	printf("Better noncache time mean = %f\n", betternoncache(numtests));
	printf("\n");
	printf("Non-cache time = %f\n", noncachetime(numtests) - temp);
	printf("\n");
	printf("Block size = %f\n", blocksize(numtests));
	
	return 0;
}

#pragma GCC pop_options