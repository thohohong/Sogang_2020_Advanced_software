#define _USE_MATH_DEFINES
#include <math.h>

double _f0(double x) {
	double result = x * x * x - 2 * x * x + x - 3;

	return result;
}

double _fp0(double x) {
	double result = 3 * x * x - 4 * x + 1;
	return result;
}

double _f1_1(double x) {
	double result = x * x - 4 * x + 4 - log(x);
	return result;
}

double _fp1_1(double x) {
	double result = 2 * x - 4 - 1 / x;
	return result;
}

double _f1_2(double x) {
	double result = x + 1 - 2 * sin(M_PI * x);
	return result;
}

double _fp1_2(double x) {
	double result = 1 - 2 * M_PI * cos(M_PI * x);
	return result;
}

double _f2(double x) {
	double result = x * x * x * x - 11.0 * x * x * x + 42.35 * x * x - 66.55 * x + 35.1384;
	return result;
}

double _fp2(double x) {
	double result = 4 * x * x * x - 11.0 * 3 * x * x + 42.35 * 2 * x - 66.55;
	return result;
}


float _sp_f1_1(float x) {
	float result = x * x - 4 * x + 4 - log(x);
	return result;
}

float _sp_fp1_1(float x) {
	float result = 2 * x - 4 - 1 / x;
	return result;
}

float _sp_f1_2(float x) {
	float result = x + 1 - 2 * sin(M_PI * x);
	return result;
}

float _sp_fp1_2(float x) {
	float result = 1 - 2 * M_PI * cos(M_PI * x);
	return result;
}

float _sp_f4(float x) {
	float result = log(x) - 1;
	return result;
}

float _sp_fp4(float x) {
	float result = 1 / x;
	return result;
}

double _f4(double x) {
	double result = log(x) - 1;
	return result;
}

double _fp4(double x) {
	double result = 1 / x;
	return result;
}
