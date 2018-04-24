#include <stdio.h>    /* for printf */
#include <stdlib.h>   /* for string to integer conversion, random numbers */
#include <time.h> /* for clock stuff */
#include <math.h>

#define FUCKHUEG 1000000000 
#define DIFF(start, end) FUCKHUEG * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec
   

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
	printf("mean = %.2f\n", mean(data, numtests));
	printf("median = %.2f\n", median(data, numtests));
	printf("mode = %.2f\n", mode(data, numtests));
}

//cache time
void cache(float* data, int numtests){
	struct timespec start, end;
	int i, a[10];
	
	for (int trial = 0; trial < numtests; trial++) {
		i=0; a[0]=1;
		
		timespec_get(&start, TIME_UTC);
		for (; i < numtests; i++) 
			a[0]+=a[0];
		timespec_get(&end, TIME_UTC);
		
		float diff = DIFF(start, end);
		data[trial] = (float)diff / numtests;
	}
	
	return;
}

//non cache time
void noncache(float* data, int numtests){
	struct timespec start, end;
	int i;
		
	for (int trial = 0; trial < numtests; trial++) {
		i=0; 
		float* a = malloc(FUCKHUEG/10000); 
		
		timespec_get(&start, TIME_UTC);
		for (; i < numtests; i++)
			a[0]+=a[(i*888)%FUCKHUEG/10000]; //add 888 for spacing
		timespec_get(&end, TIME_UTC);
		
		float diff = DIFF(start, end);
		data[trial] = (float)diff / numtests;
		
		free(a);
	}
	
	return;
}

//block size
void blocksize(float* data, int numtests){
	struct timespec start, end;
	int n[] = {2, 4, 8, 16, 32, 64, 128, 256};
	
	int atests[] = {0, 0, 0, 0, 0, 0, 0};
	
	for(int i=0; i<8; i++)
		atests[i] = numtests/n[i];

	for(int j=0; j<numtests; j++){
		
		for(int i=0; i<8; i++){
			float* a = malloc(FUCKHUEG/100); 
			int l=0;

			timespec_get(&start, TIME_UTC);
			for (; l < atests[i]; l++); 
				a[l*n[i]]+=1;
			timespec_get(&end, TIME_UTC);
			
			float diff = DIFF(start, end);
			if(j==0){
				data[i] = diff;
			}else data[i] += diff;
			
			free(a);
		}
	}
	
	for(int i=0; i<8; i++){
		printf("Dur per add at estimated size %d tests %d = %f\n", n[i], atests[i], (float)data[i]/atests[i]/numtests);
	}
	
	for(int i=1; i<8; i++){
		printf("Dur %% change = %f\n", ((float)data[i]/atests[i]/numtests) / (data[i-1]/atests[i-1]/numtests));
	}

	return;
}

/*//cache size
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
		
			timespec_get(&start, TIME_UTC);
			a+=data[1];
			timespec_get(&end, TIME_UTC);
		
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
*/

void cachesize(){
	for(int i=0; i>=0; i+= 10000){
		int a[i];
		a[i-1]=1;
		printf("Cache is %lu bytes\n", i*sizeof(int));
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
	//float ncv = median(data, numtests);
	printvals(data, numtests);

	printf("\nBlock & Cache size: \n");
	blocksize(data, numtests);

	int csize=0;
	//cachesize(data, numtests/10, &csize, ncv);
	//cachesize();
	printf("cache size =  %d\n", csize);
	
	free(data);
	return 0;
}