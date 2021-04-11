#include "my_solver.h"

#define SOLNUMS 3
#define MATCOLS SOLNUMS
#define MATROWS SOLNUMS
#define TOLERANCE 0.0000001

extern "C"
{
	int hybrd1_(void fcn(int*, double*, double*, int*), int*, double*, double*, double*, int*, double*, int*);
}


void fcn3_5(int* n, double* x, double* fvec, int* iflag)
{
	/********************************/

	fvec[0] = 10 * x[0] - 2 * pow(x[1], 2) + x[1] - 2 * x[2] - 5;
	fvec[1] = 8 * pow(x[1], 2), 4 * pow(x[2], 2) - 9;
	fvec[2] = 8 * x[1] * x[2];

	/********************************/
}

void practice3_5(void)
{
	int n = SOLNUMS;
	double x[SOLNUMS] = { 1.0, -1.0, 1.0 };	//need to initilize x0
	double fvec[SOLNUMS];
	double tol = TOLERANCE;
	int info;
	double wa[(SOLNUMS * (3 * SOLNUMS + 13)) / 2];
	int lwa = (SOLNUMS * (3 * SOLNUMS + 13)) / 2;

	FILE* fp_w = fopen("roots_3-5.txt", "w");
	if (fp_w == NULL) {
		printf("%s file open error...\n", "roots_3-5.txt");
		return;
	}

	/********************************/

	printf("============ x(0) = (1.0, -1.0, 1.0) =========== \n");
	fprintf(fp_w, "============ x(0) = (1.0, -1.0, 1.0) =========== \n");
	x[0] = 1.0; x[1] = -1.0, x[2] = 1.0;

	hybrd1_(fcn3_5, &n, x, fvec, &tol, &info, wa, &lwa);

	printf("x1 = %.10lf, x2 = %.10lf, x3 = %.10lf\n", x[0], x[1], x[2]);
	fprintf(fp_w, "x1 = %.10lf, x2 = %.10lf, x3 = %.10lf\n", x[0], x[1], x[2]);

	for (int i = 0; i < 3; i++) {
		printf("f%d(x1, x2, x3) = %.10lf\n", i, fvec[i]);
		fprintf(fp_w, "f%d(x1, x2, x3) = %.10lf\n", i, fvec[i]);
	}

	printf("info is %d\n", info);
	fprintf(fp_w, "info is %d\n", info);



	printf("============ x(0) = (1.0, 1.0, -1.0) =========== \n");
	fprintf(fp_w, "============ x(0) = (1.0, 1.0, -1.0) =========== \n");
	x[0] = 1.0; x[1] = 1.0, x[2] = -1.0;

	hybrd1_(fcn3_5, &n, x, fvec, &tol, &info, wa, &lwa);

	printf("x1 = %.10lf, x2 = %.10lf, x3 = %.10lf\n", x[0], x[1], x[2]);
	fprintf(fp_w, "x1 = %.10lf, x2 = %.10lf, x3 = %.10lf\n", x[0], x[1], x[2]);

	for (int i = 0; i < 3; i++) {
		printf("f%d(x1, x2, x3) = %.10lf\n", i, fvec[i]);
		fprintf(fp_w, "f%d(x1, x2, x3) = %.10lf\n", i, fvec[i]);
	}
	printf("info is %d\n", info);
	fprintf(fp_w, "info is %d\n", info);

	/********************************/

	fclose(fp_w);
}