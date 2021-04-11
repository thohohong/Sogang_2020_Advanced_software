#include "my_solver.h"

#define SOLNUMS 2
#define MATCOLS SOLNUMS
#define MATROWS SOLNUMS
#define TOLERANCE 0.0000001

extern "C"
{
	int hybrd1_(void fcn(int*, double*, double*, int*), int*, double*, double*, double*, int*, double*, int*);
}


void fcn3_7(int* n, double* x, double* fvec, int* iflag)
{
	/********************************/
	fvec[0] = 2 * pow(x[0], 3) - 12 * x[0] - x[1] - 1;
	fvec[1] = 3 * pow(x[1], 2) - 6 * x[1] - x[0] - 3;
	/********************************/
}

void practice3_7(void)
{
	int n = SOLNUMS;
	double x[SOLNUMS] = { -4.0, 5.0 };	//need to initilize x0
	double fvec[SOLNUMS];
	double tol = TOLERANCE;
	int info;
	double wa[(SOLNUMS * (3 * SOLNUMS + 13)) / 2];
	int lwa = (SOLNUMS * (3 * SOLNUMS + 13)) / 2;

	FILE* fp_w = fopen("roots_3-7.txt", "w");
	if (fp_w == NULL) {
		printf("%s file open error...\n", "roots_3-7.txt");
		return;
	}

	/********************************/

	for (double i = -4.0; i <= 4.0; i++) {
		for (double j = -5.0; j <= 5.0; j++) {
			printf("============ x0 = %.2lf, y0 = %.2lf =========== \n", i, j);
			fprintf(fp_w, "============ x0 = %.2lf, y0 = %.2lf =========== \n", i, j);
			x[0] = i; x[1] = j;

			hybrd1_(fcn3_7, &n, x, fvec, &tol, &info, wa, &lwa);

			printf("x = %.6lf, y = %.6lf\n", x[0], x[1]);
			fprintf(fp_w, "x = %.6lf, y = %.6lf\n", x[0], x[1]);

			for (int i = 0; i < 2; i++) {
				printf("f%d(x, y) = %.6lf\n", i, fvec[i]);
				fprintf(fp_w, "f%d(x, y) = %.6lf\n", i, fvec[i]);
			}

			printf("info is %d\n", info);
			fprintf(fp_w, "info is %d\n", info);
		}
	}

	/********************************/

	fclose(fp_w);
}