#include <stdio.h>    /* for printf */
#include <stdlib.h>   /* for string to integer conversion, random numbers */
#include <time.h> /* for clock stuff */
#include <math.h>

#define BIGENOUGH 10000000/4
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
			a[0]++;
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
		int* a = calloc(BIGENOUGH, sizeof(int)); 
		
		timespec_get(&start, TIME_UTC);
		for (; i < numtests; i++)
			a[(i*888)%BIGENOUGH]++; //add 888 for spacing
		timespec_get(&end, TIME_UTC);
		
		float diff = DIFF(start, end);
		data[trial] = (float)diff / numtests;
		
		free(a);
	}
	
	return;
}

//block size
void blocksize(float* data, int numtests, int ncv){
	struct timespec start, end;
	int sizes = 9;
	int atests[sizes];
	
	for(int i=0; i<sizes; i++){
		atests[i] = (int)((float)BIGENOUGH/pow(2,i));
	}

	for(int j=0; j<numtests; j++){
		for(int i=0; i<sizes; i++){
			int* a = calloc(BIGENOUGH, sizeof(int)); 
			int l=0;

			timespec_get(&start, TIME_UTC);
			for (; l < atests[i]; l++); 
				a[(int)(l*pow(2,i))]++;
			timespec_get(&end, TIME_UTC);
			
			float diff = DIFF(start, end);
			if(j==0){
				data[i] = diff;
			}else data[i] += diff;
			
			free(a);
		}
	}
	
	float results[] = {0., 0.};
	int max[2];
	
	for(int i=0; i<sizes; i++){
		//printf("Dur per add at estimated size %d tests %d = %f\n", (int)(pow(2,i)), atests[i], data[i]/atests[i]/numtests);
		if(data[i]/atests[i]/numtests > results[0]){
			results[0] = data[i]/atests[i]/numtests;
			max[0] = i;
		}if(i>=1){
			if(((float)data[i]/atests[i]/numtests)/(data[i-1]/atests[i-1]/numtests > results[1])){
				results[1] = ((float)data[i]/atests[i]/numtests)/(data[i-1]/atests[i-1]/numtests);
				max[1] = i;
			}
		}
	}
	/*
	for(int i=1; i<sizes; i++){
		//printf("Dur %% change from previous at size %d = %f\n", (int)(pow(2,i)), ((float)data[i]/atests[i]/numtests)/(data[i-1]/atests[i-1]/numtests));
		
	}
	*/
	//printf("Largest avg duration per add found at: size %d, duration %.2f\n", (int)(pow(2,max[0])), results[0]);
	//printf("largest %% change in duration found at: size %d, change %.2f\n", (int)(pow(2,max[1])), results[1]);
	printf("block size (within a factor of 2) = %d\n", (int)(pow(2,max[1]-1)));
	return;
}

void cachesize(float *data, int numtests) {
	struct timespec start, end;
	int resolution = 100;
	printf("[          ]"); fflush(stdout);
	for (int i=0; i<BIGENOUGH; i+=BIGENOUGH/resolution) {
		for (int trial = 0; trial < numtests; trial++) {
			int *a = calloc(BIGENOUGH, sizeof(int));

			//goto end
			for(int j=0; j<BIGENOUGH; j++){
				a[j]++;
			}
			
			//go back to i*10 location 
			timespec_get(&start, TIME_UTC);
				a[i]++;
			timespec_get(&end, TIME_UTC);

			//store time at data[i]
			if (trial == 0) data[i/(BIGENOUGH/resolution)] = DIFF(start, end);
			else data[i/(BIGENOUGH/resolution)] += DIFF(start, end);
			
			//printf("%d", i/(BIGENOUGH/resolution));

			free(a);
		}
		if(i==0) printf("\r[");
		if((i/(BIGENOUGH/resolution))%10 ==0){
			printf("=");
			fflush(stdout);
		}
		
		//printf("%.2f @ %d @ %d\n", data[i/(BIGENOUGH/resolution)], i, i/(BIGENOUGH/resolution));
	}
	
	/*for (int i=0; i<resolution; i++){
		data[i]/=numtests;
	}*/
	
	printf("]\r\n");
	float target = median(data, resolution);
	//printf("max = %.2f\n ", max);
	
	printvals(data, resolution);
	for(int muta=5; muta >=0; muta--){
		for (int i=1; i<=resolution; i++){
			//printf("%.2f, ", data[i]);
			//printf("\n");
			if(data[resolution-i] < (1+(muta/10)) * target){
				//printf("%d, %.2f", BIGENOUGH, (float)i*(BIGENOUGH/resolution));
				printf("cache size in Mbytes = %.2f at diffscale %.2f and resolution %.2f with value %.2f\n", ((float)(BIGENOUGH) - ((resolution-i)*(BIGENOUGH/resolution)))*sizeof(int)/1000000, (float)muta/10, (float)(BIGENOUGH/resolution)*sizeof(int)/1000000, data[i]);
				//return;
				break;
			}
		}
		//printf("Cache size not found at confidence %.2f.\nRetrying at reduced level.\n", (float)muta/5);
	}
	return;
}

//main
int main(){//(int argc, char** argv){
	int numtests = 20000; //atoi(argv[1]);
	float* data = calloc(BIGENOUGH, sizeof(float)); 
	/*
	printf("\nCache time: \n");
	cache(data, numtests);
	printvals(data, numtests);
	
	printf("\nNoncache time: \n");
	noncache(data, numtests/2);
	float ncv = median(data, numtests/2);
	printvals(data, numtests/2);

	printf("\nBlock size: \n");
	blocksize(data, numtests/50, ncv);
	*/
	printf("\nCache size: \n");
	cachesize(data, numtests/1000);
	
	printf("\n");
	free(data);
	return 0;
}