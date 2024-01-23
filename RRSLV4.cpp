#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

void drugi_zad(int num_threads) {
	int size = 10000;
	printf("Dodijeljeno niti: %d\n", num_threads);
	omp_set_num_threads(num_threads);
	double startTime = omp_get_wtime();
	int i, j, k= 0;
	double** a;
	double** b;
	double** c;

	a = (double**)malloc(size * sizeof(double*));
	b = (double**)malloc(size * sizeof(double*));
	c = (double**)malloc(size * sizeof(double*));

#pragma omp parallel for
	for (i = 0; i < size; i++) {
		*(a + i) = (double*)malloc(size * sizeof(double));
		*(b + i) = (double*)malloc(size * sizeof(double));
		*(c + i) = (double*)malloc(size * sizeof(double));
	}

#pragma omp parallel for private(j)
	for (i = 0; i < size; i++) {
		for (j = 0; j < size; j++) {
			*(*(a + i) + j) = i + j;
			*(*(b + i) + j) = j - i;
			*(*(c + i) + j) = 0;
		}
	}

#pragma omp parallel for private(j,k) 
	for (i = 0; i < size; i++) {
		for (j = 0; j < size; j++) {
			for (k = 0; k < size; k++) {
				c[i][j] += a[i][k] * b[k][j];
			}
		}
	}

	double endTime = omp_get_wtime();
	double total = (endTime - startTime);
	printf("time took: %f s\n", total);
	printf("time took: %f s\n", total * num_threads);
	double mflop = 2 * (double)size * (double)size * (double)size / total * 1000000;
	printf("mflop %f\n", mflop);

	for (int i = 0; i < size; i++)
	{
		free(a[i]);
		free(b[i]);
		free(c[i]);
	}

	free(a);
	free(b);
	free(c);
}

int main(int argc, char* argv[]) {
	drugi_zad(12);
	return 0;
}
