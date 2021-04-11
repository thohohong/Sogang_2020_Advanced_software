#include "my_solver.h"

#define SOLNUMS 2
#define MATCOLS SOLNUMS
#define MATROWS SOLNUMS
#define TOLERANCE 0.0000001

extern "C"
{
	int hybrj1_(void fcn(int*, double*, double*, double*, int*, int*), int*, double*, double*, double*, int*, double*, int*, double*, int*);
}

void fcn3_6(int* n, double* x, double* fvec, double* fjac, int* ldfjac, int* iflag) {
	// origin function F(x)
	if (*iflag == 1) {
		/********************************/

		fvec[0] = 3 * x[0] * x[0] - 2 * x[1] * x[1];
		fvec[1] = x[0] * x[0] - 2 * x[0] + x[1] * x[1] + 2 * x[1] - 8;

		/********************************/
	}
	// Jacobi matrix J(x)
	else if (*iflag == 2) {
		/********************************/

		fjac[0] = 6 * x[0];				fjac[2] = -4 * x[1];
		fjac[1] = 2 * x[0] - 2;			fjac[3] = 2 * x[1] + 2;	

		/********************************/
	}
}

void practice3_6(void)
{
	int n = SOLNUMS;
	double x[SOLNUMS] = { -4.0, 5.0 };	//need to initilize x0
	double fvec[SOLNUMS], fjac[MATCOLS * MATROWS];
	int ldfjac = SOLNUMS;
	double tol = TOLERANCE;
	int info;
	double wa[(SOLNUMS * (SOLNUMS + 13)) / 2];
	int lwa = (SOLNUMS * (SOLNUMS + 13)) / 2;

	FILE* fp_w = fopen("roots_3-6.txt", "w");
	if (fp_w == NULL) {
		printf("%s file open error...\n", "roots_3-6.txt");
		return;
	}

	/********************************/

	
	for (double i = -4.0; i <= 4.0; i++) {
		for (double j = -5.0; j <= 5.0; j++) {
			printf("============ x0 = %.2lf, y0 = %.2lf =========== \n", i, j);
			fprintf(fp_w, "============ x0 = %.2lf, y0 = %.2lf =========== \n", i, j);
			x[0] = i; x[1] = j;

			hybrj1_(fcn3_6, &n, x, fvec, fjac, &ldfjac, &tol, &info, wa, &lwa);

			printf("x = %.6lf, y = %.6lf\n", x[0], x[1]);
			fprintf(fp_w, "x = %.6lf, y = %.6lf\n", x[0], x[1]);

			for (int i = 0; i < 2; i++) {
				printf("f%d(x, y) = %.6lf\n", i, fvec[i]);
				fprintf(fp_w, "f%d(x, y) = %.6lf\n", i, fvec[i]);
			}

			if (!(info == 1 || info == 2)) {
				printf("info is %d, improper root\n", info);
				fprintf(fp_w, "info is %d, improper root\n", info);
			}
			else {
				printf("info is %d\n", info);
				fprintf(fp_w, "info is %d\n", info);
			}
		}
	}

	/********************************/

	fclose(fp_w);
}