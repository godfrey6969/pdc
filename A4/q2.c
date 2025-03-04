#include <stdio.h>
#include <omp.h>
#include <stdlib.h>

int privateVar;
#pragma omp threadprivate(privateVar)
int cmpA() {
	return 10;
}
int cmpB() {
	return 12;
}
void main() {
	int N = 100;
	int arr1[N], arr2[N];
	for (int i = 0; i < N; i++) {
		arr1[i] = rand() % 100 + 1;
		arr2[i] = rand() % 100 + 1;	
	}
	int serial = 0, parallel = 0, shared = 0, result = 0;
	double st_time_ser = omp_get_wtime();
	for (int i = 0; i < N; i++) {
		serial += (arr1[i] * arr2[i]);
	}
	double en_time_ser = omp_get_wtime();
	
	double st_time_par = omp_get_wtime();
	#pragma omp parallel reduction(+:parallel)
	{
		privateVar = omp_get_thread_num() + 1;
		#pragma omp single 
		{
			shared = 12;
			printf("Shared variable initialized by thread %d\n", omp_get_thread_num());
		}
		#pragma omp for
		for(int i = 0; i < N; i++) {
			parallel += (arr1[i] * arr2[i]);
		}
		#pragma omp sections
		{
			#pragma omp section
			{
				int resA = cmpA() * privateVar * shared;
				result += resA;
			}
			#pragma omp section
			{
				int resB = cmpB() * privateVar * shared;
				result += resB;
			}
		}
	}
	double en_time_par = omp_get_wtime();
	printf("\nDot product serial: %d\n", serial);
	printf("Dot product parallel: %d\n", parallel);
	printf("Final result: %d\n", result);
	
	printf("Time taken(serial): %f\n", en_time_ser - st_time_ser);
	printf("Time taken(serial): %f", en_time_par - st_time_par);
}
