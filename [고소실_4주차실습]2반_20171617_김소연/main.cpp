#include "my_solver.h"

extern void program1_1(FILE* fp); // Newton-Raphson Method
extern void program1_2(FILE* fp); // Secant Method

extern void sp_program1_1(FILE* fp); // Newton-Raphson Method
extern void sp_program1_2(FILE* fp); // Secant Method

double (*_f)(double);
double (*_fp)(double);

float (*_sp_f)(float);
float (*_sp_fp)(float);

int main() {
	FILE* fp;
	fp = fopen("result.txt", "w");


	_f = _f1_1;
	_fp = _fp1_1;
	
	_sp_f = _sp_f4;
	_sp_fp = _sp_fp4;
	
	// for probelm 1-2
	/*
	for (int i = 0; i < 4; i++) {
		program1_1(fp);
	}
	*/
	program1_1(fp);
	program1_2(fp);

	//sp_program1_1(fp);
	//sp_program1_2(fp);


	fclose(fp);
}
