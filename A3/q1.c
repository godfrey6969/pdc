#include <stdio.h>

void sub() {
	printf("Sub program called: \n");
	#pragma omp parallel
	{
		printf("Sub function parallel region: \n");
	}
}

void main() {
	printf("Main function called: \n");
	#pragma omp parallel
	{
		printf("Main parallel region: \n");
		printf("Sub function called from inside the Main parallel region: \n");
		sub();
	}
	sub();
}