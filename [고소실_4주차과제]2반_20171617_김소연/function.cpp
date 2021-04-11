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

double _f_a(double x) {
	double A, B, C, D, E, h, l;
	double beta, alpha;
	double result;

	l = 89;
	h = 49;
	D = 55;
	beta = 11.5;

	A = l * sin(beta);
	B = l * cos(beta);
	C = (h + 0.5 * D) * sin(beta) - 0.5 * D * tan(beta);
	E = (h + 0.5 * D) * cos(beta) - 0.5 * D;

	result = A * sin(x) * cos(x) + B * sin(x) * sin(x) - C * cos(x) - E * sin(x);
	return result;
}

double _fp_a(double x) {
	double beta, alpha;
	double result;
	double l, h, D;

	l = 89;
	h = 49;
	D = 55;
	beta = 11.5;

	result = (-2 * l * sin(2 * x + beta) + (2 * h + D) * cos(x + beta) + D * tan(beta) * sin(x) - D * cos(x)) / -2;

	return result;
}
