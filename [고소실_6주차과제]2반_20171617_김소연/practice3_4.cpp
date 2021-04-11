#include "my_solver.h"

#define SOLNUMS 3
#define MATCOLS SOLNUMS
#define MATROWS SOLNUMS
#define TOLERANCE 0.0000001

extern "C"
{
	int hybrj1_(void fcn(int*, double*, double*, double*, int*, int*), int*, double*, double*, double*, int*, double*, int*, double*, int*);
}

void fcn3_4(int* n, double* x, double* fvec, double* fjac, int* ldfjac, int* iflag)
{
	// origin function F(x)
	if (*iflag == 1) {
		/********************************/

		fvec[0] = x[0] + x[1] + x[2] - 3.0;
		fvec[1] = x[0] * x[0] + x[1] * x[1] + x[2] * x[2] - 5.0;
		fvec[2] = exp(x[0]) + x[0] * x[1] + x[0] * x[2] - 1.0;

		/********************************/
	}
	// Jacobi matrix J(x)
	else if (*iflag == 2) {
		/********************************/

		fjac[0] = 1.0;						fjac[3] = 1.0;			fjac[6] = 1.0;
		fjac[1] = 2.0 * x[0];				fjac[4] = 2.0 * x[1];	fjac[7] = 2.0 * x[2];
		fjac[2] = exp(x[0]) + x[1] + x[2];	fjac[5] = x[0];			fjac[8] = x[2];

		/********************************/
	}
}

void practice3_4(void) {
	int n = SOLNUMS;
	double x[SOLNUMS] = { 0.1, 1.2, 2.5 };	//need to initilize x0
	double fvec[SOLNUMS], fjac[MATCOLS * MATROWS];
	int ldfjac = SOLNUMS;
	double tol = TOLERANCE;
	int info;
	double wa[(SOLNUMS * (SOLNUMS + 13)) / 2];
	int lwa = (SOLNUMS * (SOLNUMS + 13)) / 2;

	FILE* fp_w = fopen("roots_3-4.txt", "w");
	if (fp_w == NULL) {
		printf("%s file open error...\n", "roots_3-4.txt");
		return;
	}

	/********************************/

	printf("============ x(0) = (0.1, 1.2, 2.5) =========== \n");
	fprintf(fp_w, "============ x(0) = (0.1, 1.2, 2.5) =========== \n");
	x[0] = 0.1; x[1] = 1.2, x[2] = 2.5;

	hybrj1_(fcn3_4, &n, x, fvec, fjac, &ldfjac, &tol, &info, wa, &lwa);

	printf("x1 = %.10lf, x2 = %.10lf, x3 = %.10lf\n", x[0], x[1], x[2]);
	fprintf(fp_w, "x1 = %10lf, x2 = %.10lf, x3 = %.10lf\n", x[0], x[1], x[2]);

	for (int i = 0; i < 3; i++) {
		printf("f%d(x1, x2, x3) = %.10lf\n", i, fvec[i]);
		fprintf(fp_w, "f%d(x1, x2, x3) = %.10lf\n", i, fvec[i]);
	}

	if (!(info == 1 || info == 2)) {
		printf("info is %d, improper root\n", info);
		fprintf(fp_w, "info is %d, improper root\n", info);
	}
	else {
		printf("info is %d\n", info);
		fprintf(fp_w, "info is %d\n", info);
	}



	printf("============ x(0) = (1.0, 0.0, 1.0) =========== \n");
	fprintf(fp_w, "============ x(0) = (1.0, 0.0, 1.0) =========== \n");
	x[0] = 1.0; x[1] = 0.0, x[2] = 1.0;

	hybrj1_(fcn3_4, &n, x, fvec, fjac, &ldfjac, &tol, &info, wa, &lwa);

	printf("x1 = %.10lf, x2 = %.10lf, x3 = %.10lf\n", x[0], x[1], x[2]);
	fprintf(fp_w, "x1 = %.10lf, x2 = %.10lf, x3 = %.10lf\n", x[0], x[1], x[2]);

	for (int i = 0; i < 3; i++) {
		printf("f%d(x1, x2, x3) = %.10lf\n", i, fvec[i]);
		fprintf(fp_w, "f%d(x1, x2, x3) = %.10lf\n", i, fvec[i]);
	}

	if (!(info == 1 || info == 2)) {
		printf("info is %d, improper root\n", info);
		fprintf(fp_w, "info is %d, improper root\n", info);
	}
	else {
		printf("info is %d\n", info);
		fprintf(fp_w, "info is %d\n", info);
	}
	/********************************/

	fclose(fp_w);
}
