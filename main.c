#include <stdio.h>    /* for printf */
#include <stdlib.h>   /* for string to integer conversion, random numbers */
#include <time.h> /* for clock stuff */

#define DIFF(start, end) 1000000000 * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec
#define arrsize 1000

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
	int i, a[1];
	
	for (int trial = 0; trial < numtests; trial++) {
		i=0;
		
		clock_gettime(CLOCK_MONOTONIC, &start);
		for (; i < numtests; i++) 
			a[0]+= 1;
		clock_gettime(CLOCK_MONOTONIC, &end);
		
		float diff = DIFF(start, end);
		data[trial] = (float)diff / numtests;
	}
	
	return;
}

//non cache time
void noncache(float* data, int numtests){
	struct timespec start, end;
	int i, a[arrsize];
		
	for (int trial = 0; trial < numtests; trial++) {
		i=0;
		
		clock_gettime(CLOCK_MONOTONIC, &start);
		for (; i < numtests; i++)
			a[(i*255)%arrsize]+=1;
		clock_gettime(CLOCK_MONOTONIC, &end);
		
		float diff = DIFF(start, end);
		data[trial] = (float)diff / numtests;
	}
	
	return;
}

//block size
void blocksize(float* data, int numtests, int* size, int* blocks){
	struct timespec start, end;
	float diff=0.;
	float a[arrsize];
	
	for(int i=0; i<arrsize; i++) 
		a[i]=0;
	for(int j=0; j<numtests; j++){
		for(int i=0; i<arrsize; i++){
		
			clock_gettime(CLOCK_MONOTONIC, &start);
			a[i]+=diff;
			clock_gettime(CLOCK_MONOTONIC, &end);
		
			diff = DIFF(start, end);
		}
		//if numtests > sizeof cache, cache does not have to be manually cleared between cycles
		
	}
	
	float temp = mode(a, arrsize);
	printf("%f", temp);
	
	for(int i=0; i<arrsize; i++){
		if(a[i]>temp) (*blocks)++;
		if(a[i]<=temp) (*size)++;
	}

	return;
}

//cache size
void cachesize(float* data, int numtests, int* size, float target){
	struct timespec start, end;
	float diff;
	int a;
	for(int j=0; j<numtests; j++){
		//go forward to load all blocks in cache
		for(int i=0; i<numtests; i++){
			a=0;
			a+=data[i];
		}
		//go back to find delay
		for(int i=numtests-1; i>=0; i--){
			a=0;
		
			clock_gettime(CLOCK_MONOTONIC, &start);
			a+=data[i];
			clock_gettime(CLOCK_MONOTONIC, &end);
		
			diff = DIFF(start, end);
			if(j==0){
				data[i] = diff;
			}else data[i] += diff;
		}
	}
	

	int i = 1;
	while((data[numtests-i] < target) && (i >= 0)){
		(*size)++;
		printf("%d ", *size);
		i--;
	}

	return;
}

//main
int main(int argc, char** argv){
	int numtests = atoi(argv[1]);
	float* data = malloc(numtests * sizeof(float)); 
	
	printf("\nCache time: \n");
	cache(data, numtests);
	printvals(data, numtests);
	
	printf("\nNoncache time: \n");
	noncache(data, numtests);
	float ncv = median(data, numtests);
	printvals(data, numtests);

	printf("\nBlock & Cache size: \n");
	int bsize=0, blocks=1;
	blocksize(data, numtests, &bsize, &blocks);
	printf("block size =  %d, %d, %d, %d\n", bsize, blocks, bsize/blocks, bsize / blocks / numtests);

	int csize=0;
	cachesize(data, numtests, &csize, ncv);
	printf("cache size =  %d\n", csize);
	
	printf("\n");
	free(data);
	return 0;
}