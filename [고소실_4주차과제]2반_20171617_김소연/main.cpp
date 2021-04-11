#include "my_solver.h"

extern void program1_1(FILE* fp); // Newton-Raphson Method
extern void program1_2(FILE* fp); // Secant Method
extern void program1_3(FILE* fp); // Bisection Method

extern void sp_program1_1(FILE* fp); // Newton-Raphson Method
extern void sp_program1_2(FILE* fp); // Secant Method

double (*_f)(double);
double (*_fp)(double);

float (*_sp_f)(float);
float (*_sp_fp)(float);

int main() {
	FILE* fp;
	fp = fopen("result.txt", "w");


	_f = _f2;
	_fp = _fp2;
	
	_sp_f = _sp_f1_2;
	_sp_fp = _sp_fp1_2;
		
	// for probelm 1-2
	/*
	for (int i = 0; i < 4; i++) {
		program1_3(fp);
	}
	*/
	program1_1(fp);
	program1_2(fp);
	program1_3(fp);

	//sp_program1_1(fp);
	//sp_program1_2(fp);


	fclose(fp);
}