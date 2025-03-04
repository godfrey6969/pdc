#include <stdio.h>
#include <omp.h>

int compA() {
	return 4;
}
int compB() {
	return 5;
}

void main() {
	int final = 0;
	int shared = 0;
	
	#pragma omp parallel
	{
		#pragma omp single 
		{
			shared = 100;
			printf("Initializing shared variable by single construct \n");
			printf("Thread: %d\n", omp_get_thread_num());
		}
		#pragma omp sections reduction(+:final)
		{
			#pragma omp section
			{
				int resA = compA() * shared;
				printf("First section computed by thread: %d\n", omp_get_thread_num());
				printf("Computation: %d\n", resA);
				final += resA;
			}
			#pragma omp section
			{
				int resB = compB() * shared;
				printf("Second section computed by thread: %d\n", omp_get_thread_num());
				printf("Computation: %d\n", resB);
				final += resB;
			}
		}
	}
	printf("Final resut: %d\n", final);
	return;
}
