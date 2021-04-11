#include <stdio.h>
#include <random>
#include <time.h>
#include <math.h>
#include <time.h>
#include <Windows.h>

#define N 50
double Taylor_series(double x, int n);
double Taylor_series_ex(double x, int n);
__int64 start, freq, end;
#define CHECK_TIME_START QueryPerformanceFrequency((LARGE_INTEGER*)&freq); QueryPerformanceCounter((LARGE_INTEGER*)&start)
#define CHECK_TIME_END(a) QueryPerformanceCounter((LARGE_INTEGER*)&end); a = (float)((float)(end - start) / (freq / 1000.0f))
float compute_time;

void main(void)
{
	CHECK_TIME_START;
	printf("*** f<-8.3> = %.6e \n", Taylor_series(-8.3, N));
	CHECK_TIME_END(compute_time);
	printf("Taylor_series : %f ms\n\n", compute_time);

	CHECK_TIME_START;
	printf("*** f<-8.3> = %.6e \n", Taylor_series_ex(-8.3, N));
	CHECK_TIME_END(compute_time);
	printf("Taylor_series_ex : %f ms\n\n", compute_time);

	printf("*** f<-8.3> = %.6e \n", pow(2.71828182846, -8.3));
}



//double로 구현
double Taylor_series(double x, int n)
{
	int i, j;
	double result = 1.0;
	double series = 1;

	for (i = 1; i < n; i++) {
		series *= i;
		result += pow(x, i) / series;
	}
	
	return result;
}

//float으로 구현
double Taylor_series_ex(double x, int n)
{
	int i, j;
	float result = 1.0;
	
	for (i = n - 1; i > 0; i--) {
		result *= (float)(x / i);
		result += 1;
	}

	return result;
}
