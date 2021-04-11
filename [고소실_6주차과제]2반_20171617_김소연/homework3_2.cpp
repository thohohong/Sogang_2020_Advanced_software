#include "my_solver.h"


#define SOLNUMS 4
#define MATCOLS SOLNUMS
#define MATROWS SOLNUMS
#define TOLERANCE 0.0000001

extern "C"
{
	int hybrd1_(void fcn(int*, double*, double*, int*), int*, double*, double*, double*, int*, double*, int*);
}


void fcn_hw_3_2(int* n, double* x, double* fvec, int* iflag)
{
	/********************************/

	fvec[0] = x[0] + 10 * x[1] + 9;
	fvec[1] = sqrt(5) * (x[3] - x[2]) - 2 * sqrt(5);
	fvec[2] = pow(x[1] - 2 * x[3], 2.0) - 9;
	fvec[3] = sqrt(10) * pow(x[0] - x[2], 2.0) - 2 * sqrt(10);

	/********************************/
}

void homework3_2(void)
{
	int n = SOLNUMS;
	double x[SOLNUMS] = { 0.9, -0.9, 1.25, -1.25 };	//need to initilize x0
	double fvec[SOLNUMS];
	double tol = TOLERANCE;
	int info;
	double wa[(SOLNUMS * (3 * SOLNUMS + 13)) / 2];
	int lwa = (SOLNUMS * (3 * SOLNUMS + 13)) / 2;

	FILE* fp_w = fopen("roots_found_3_2.txt", "w");
	if (fp_w == NULL) {
		printf("%s file open error...\n", "roots_found_3_2.txt");
		return;
	}

	hybrd1_(fcn_hw_3_2, &n, x, fvec, &tol, &info, wa, &lwa);

	int iflag = 1;
	fcn_hw_3_2(&n, x, fvec, &iflag);

	fprintf(fp_w, "x = %.10lf\n", x[0]);
	fprintf(fp_w, "y = %.10lf\n", x[1]);
	fprintf(fp_w, "w = %.10lf\n", x[2]);
	fprintf(fp_w, "z = %.10lf\n", x[3]);

	for (int j = 0; j < 4; j++) {
		fprintf(fp_w, "f%d(x, y, w, z) = %.10lf\n", j + 1, fvec[j]);
	}

	fclose(fp_w);
}