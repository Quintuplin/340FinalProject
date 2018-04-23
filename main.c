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
void normalise(float **data, int numtests){
	qsort(*data, numtests, sizeof(float), compare_float);
	float max = (float)data[0];
	float min = (float)data[numtests-1];
	
	float nmax = 1000; float nmin = 0;
	
	for(int i=0; i<numtests; i++){
		float temp = nmin + (*(data+i) - min)(nmax-nmin)/(max-min);
		*data[i] = temp;
	}
	
	return;
}*/

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

/*//base time
float basetime(int numtests){
	struct timespec start, end;
	float* data = malloc(numtests * sizeof(float)); 
	float diff;
	
	for(int i=0; i<numtests; i++){

		clock_gettime(CLOCK_MONOTONIC, &start);
		clock_gettime(CLOCK_MONOTONIC, &end);
		
		diff = ((1000000000 * (end.tv_sec - start.tv_sec)) + end.tv_nsec) -  start.tv_nsec;
					
		if (diff >= 1000000)printf("%f ", diff); //detect for massive error condition

		data[i] = diff;
		//printf("c%f ", diff);
	}
	
	float temp = mean(data, numtests);
	free(data);
	return temp;
}
*/

/*//cache time
float cachetime(int numtests){
	struct timespec start, end;
	float* data = malloc(numtests * sizeof(float)); 
	float diff;
	int a = 1;
	
	for(int i=0; i<numtests; i++){
		a = 1;
		clock_gettime(CLOCK_MONOTONIC, &start);
		a;
		clock_gettime(CLOCK_MONOTONIC, &end);
		
		diff = ((1000000000 * (end.tv_sec - start.tv_sec)) + end.tv_nsec) -  start.tv_nsec;
					
		if (diff >= 1000000)printf("%f ", diff); //detect for massive error condition

		data[i] = diff;
		//printf("c%f ", diff);
	}
	
	float temp = mean(data, numtests);
	free(data);
	return temp;
}
*/

//better cache time
void bettercache(float* data, int numtests){
	//float *data = malloc(numtests * sizeof(float));
	for (int trial = 0; trial < numtests; trial++) {
		struct timespec start, end;
		int i = 0; int a=0;
		clock_gettime(CLOCK_MONOTONIC, &start);
		for (; i < numtests; i++) 
			//a++;
			a+=a;
		clock_gettime(CLOCK_MONOTONIC, &end);
		float diff = DIFF(start, end);
		data[trial] = (float)diff / numtests;
		//printf("%2d ", diff / numtests); // remove eventually
	}
	
	//printf("Better cache median = %f\n", median(data, numtests));
	//printf("Better cache mode = %f\n", mode(data, numtests));
	//float temp = mean(data, numtests);
	//free(data);
	return;
}

//better non cache time
void betternoncache(float* data, int numtests){
	//float *data = malloc(numtests * sizeof(float));
	for (int trial = 0; trial < numtests; trial++) {
		struct timespec start, end;
		int i = 0; int a=0;
		clock_gettime(CLOCK_MONOTONIC, &start);
		for (; i < numtests; i++)
			a+=data[(i*888)%numtests]; //add 888 so that it's not clean blocks of potentially predictable 1000s
		clock_gettime(CLOCK_MONOTONIC, &end);
		float diff = DIFF(start, end);
		data[trial] = (float)diff / numtests;
		//printf("%2d ", diff / numtests); // remove eventually
	}
	
	//printf("Better noncache median = %f\n", median(data, numtests));
	//printf("Better noncache mode = %f\n", mode(data, numtests));
	//float temp = mean(data, numtests);
	//free(data);
	return;
}

/*//non cache time
float noncachetime(int numtests){
	struct timespec start, end;
	
	float* data = malloc(numtests * sizeof(float)); 
	float diff;
	
	for(int i=0; i<numtests; i++){
		clock_gettime(CLOCK_MONOTONIC, &start);
		void* a = malloc(sizeof(int)); //shouf be uncacheable??
		clock_gettime(CLOCK_MONOTONIC, &end);
		free(a);
		
		diff = ((1000000000 * (end.tv_sec - start.tv_sec)) + end.tv_nsec) -  start.tv_nsec;

		if (diff >= 1000000)printf("%f ", diff); //detect for massive error condition
		
		data[i] = diff;
		//printf("v%f ", diff);
	}
	
	float temp = mean(data, numtests);
	free(data);
	return temp;
}
*/

//block size
float blocksize(int numtests){
	struct timespec start, end;
	
	float* data = malloc(numtests * sizeof(float)); 
	float diff;
	
	for(int i=0; i<numtests; i++){
		int a=0;
		clock_gettime(CLOCK_MONOTONIC, &start);
		a+=data[i];
		clock_gettime(CLOCK_MONOTONIC, &end);
		
		diff = ((1000000000 * (end.tv_sec - start.tv_sec)) + end.tv_nsec) -  start.tv_nsec;
		if (diff >= 1000000)printf("%f ", diff); //detect for massive error condition
		data[i] = diff;
	}
	
	float temp = mean(data, numtests);
	float* len = malloc(numtests * sizeof(float)); 
	int j=0;
	
	for(int i=0; i<numtests; i++){
		if(data[i]<temp) len[j]++;
		else j++;
	}
	
	free(data);
	
	float temp2 = mode(len, j);
	free(len);
	
	return temp2;
}
//cache size

//main
int main(int argc, char** argv){
	int numtests = atoi(argv[1]);
	
	float* data = malloc(numtests * sizeof(float)); 
	
	printf("\nCache time: \n");
	bettercache(data, numtests);
	printvals(data, numtests);
	
	printf("\nNoncache time: \n");
	betternoncache(data, numtests);
	printvals(data, numtests);
	
	/*
	//float temp = (float)basetime(numtests);
	//printf("Cache time = %f\n", cachetime(numtests) - temp);
	//printf("\n");
	printf("Better cache time mean = %f\n", bettercache(numtests));
	printf("\n");
	printf("Better noncache time mean = %f\n", betternoncache(numtests));
	printf("\n");
	//printf("Non-cache time = %f\n", noncachetime(numtests) - temp);
	//printf("\n");
	*/
	
	printf("\nBlock size = %f\n", blocksize(numtests));
	
	free(data);
	return 0;
}

#pragma GCC pop_options