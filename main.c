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

//stats mean
float mean(float* data, int numtests){
	float a = 0;
	
	for(int i=0; i<numtests; i++){
		a += data[i];
	}
	
	return (float)(a/numtests);
}

//stats median
float median(float* data, int numtests){
	return data[numtests/2];
}

//stats mode
float mode(float* data, int numtests){
	float temp = data[0];
	int nums = 0;
	int curmax = 0;
	int curmaxloc = 0;
	
	for(int i=0; i<numtests; i++){
		if(data[i]==temp) nums++;
		else{
			if(nums>curmax){
				curmaxloc = (i-1);
				curmax = nums;
			}
			nums = 0;
			temp = data[i];
		}
	}
	
	return data[curmaxloc];
}

//output results
void printvals(float* data, int numtests){
	qsort(data, numtests, sizeof(float), compare_float);
	printf("mean = %f\n", mean(data, numtests));
	printf("median = %f\n", median(data, numtests));
	printf("mode = %f\n", mode(data, numtests));
}

//cache time
void cache(float* data, int numtests){
	struct timespec start, end;
	int i, a;
	
	for (int trial = 0; trial < numtests; trial++) {
		i=0; a=1;
		
		clock_gettime(CLOCK_MONOTONIC, &start);
		for (; i < numtests; i++) 
			a+=a;
		clock_gettime(CLOCK_MONOTONIC, &end);
		
		float diff = DIFF(start, end);
		data[trial] = (float)diff / numtests;
	}
	
	return;
}

//non cache time
void noncache(float* data, int numtests){
	struct timespec start, end;
	int i,a;
		
	for (int trial = 0; trial < numtests; trial++) {
		i=0; a=1;
		
		clock_gettime(CLOCK_MONOTONIC, &start);
		for (; i < numtests; i++)
			a+=data[(i*888)%numtests]; //add 888 for spacing
		clock_gettime(CLOCK_MONOTONIC, &end);
		
		float diff = DIFF(start, end);
		data[trial] = (float)diff / numtests;
	}
	
	return;
}

//block size
void blocksize(float* data, int numtests, int* size, int* blocks){
	struct timespec start, end;
	float diff;
	int a;
	for(int j=0; j<10000; j++){
		for(int i=0; i<numtests; i++){
			a=0;
		
			clock_gettime(CLOCK_MONOTONIC, &start);
			a+=data[i];
			clock_gettime(CLOCK_MONOTONIC, &end);
		
			diff = DIFF(start, end);
			if(j==0){
				data[i] = (float)diff/numtests;
			}else data[i] += (float)diff/numtests;
		}
	}
	
	float temp = mean(data, numtests);

	for(int i=0; i<numtests; i++){
		if(data[i]>temp) (*blocks)++;
		if(data[i]<temp) (*size)++;
	}

	return;
}

//cache size
//LOOOL

//main
int main(int argc, char** argv){
	int numtests = atoi(argv[1]);
	float* data = malloc(numtests * sizeof(float)); 
	
	printf("\nCache time: \n");
	cache(data, numtests);
	printvals(data, numtests);
	
	printf("\nNoncache time: \n");
	noncache(data, numtests);
	printvals(data, numtests);

	printf("\nBlock and cache size: \n");
	int size=0, blocks=0;
	blocksize(data, numtests, &size, &blocks);
	printf("block size =  %d\n", size);
	printf("cache size = %d\n", size * blocks);
	
	//printvals(data, numtests);
	
	free(data);
	return 0;
}