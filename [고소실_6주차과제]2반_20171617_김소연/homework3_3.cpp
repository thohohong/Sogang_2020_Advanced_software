#include "my_solver.h"


#define SOLNUMS 2
#define MATCOLS SOLNUMS
#define MATROWS SOLNUMS
#define TOLERANCE 0.0000001

extern "C"
{
	int hybrd1_(void fcn(int*, double*, double*, int*), int*, double*, double*, double*, int*, double*, int*);
}


void fcn_hw_3_3(int* n, double* x, double* fvec, int* iflag)
{
	/********************************/

	fvec[0] = sin(x[0] * x[1] + M_PI / 6) + sqrt(x[0] * x[0] * x[1] * x[1] + 1);
	fvec[0] /= cos(x[0] - x[1]);
	fvec[0] += 2.8;

	fvec[1] = x[0] * exp(x[0] * x[1] + M_PI / 6) - sin(x[0] - x[1]);
	fvec[1] /= sqrt(x[0] * x[0] * x[1] * x[1] + 1);
	fvec[1] -= 1.66;

	/********************************/
}

void homework3_3(void)
{
	int n = SOLNUMS;
	double x[SOLNUMS] = { 20.0, 0.0 };	//need to initilize x0
	double fvec[SOLNUMS];
	double tol = TOLERANCE;
	int info;
	double wa[(SOLNUMS * (3 * SOLNUMS + 13)) / 2];
	int lwa = (SOLNUMS * (3 * SOLNUMS + 13)) / 2;

	FILE* fp_w = fopen("roots_found_3_3.txt", "w");
	if (fp_w == NULL) {
		printf("%s file open error...\n", "roots_found_3_3.txt");
		return;
	}

	hybrd1_(fcn_hw_3_3, &n, x, fvec, &tol, &info, wa, &lwa);


	fprintf(fp_w, "x = %.10lf\n", x[0]);
	fprintf(fp_w, "y = %.10lf\n", x[1]);
	
	for (int j = 0; j < 2; j++) {
		fprintf(fp_w, "f%d(x, y) = %.10lf\n", j + 1, fvec[j]);
	}

	fprintf(fp_w, "info is %d\n", info);

	fclose(fp_w);
}