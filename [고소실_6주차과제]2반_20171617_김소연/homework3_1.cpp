#include "my_solver.h"

#define SOLNUMS 4
#define MATCOLS SOLNUMS
#define MATROWS SOLNUMS
#define TOLERANCE 0.0000001

double** p;
double C;


extern "C"
{
	int hybrj1_(void fcn(int*, double*, double*, double*, int*, int*), int*, double*, double*, double*, int*, double*, int*, double*, int*);
	int hybrd1_(void fcn(int*, double*, double*, int*), int*, double*, double*, double*, int*, double*, int*);
}

void fcn_hw_3_1_2(int* n, double* x, double* fvec, int* iflag)
{
	/********************************/
	fvec[0] = pow(x[0] - p[0][0], 2.0) + pow(x[1] - p[0][1], 2.0) + pow(x[2] - p[0][2], 2.0) - C * C * pow(p[0][4] + x[3] - p[0][3], 2.0);
	fvec[1] = pow(x[0] - p[1][0], 2.0) + pow(x[1] - p[1][1], 2.0) + pow(x[2] - p[1][2], 2.0) - C * C * pow(p[1][4] + x[3] - p[1][3], 2.0);
	fvec[2] = pow(x[0] - p[2][0], 2.0) + pow(x[1] - p[2][1], 2.0) + pow(x[2] - p[2][2], 2.0) - C * C * pow(p[2][4] + x[3] - p[2][3], 2.0);
	fvec[3] = pow(x[0] - p[3][0], 2.0) + pow(x[1] - p[3][1], 2.0) + pow(x[2] - p[3][2], 2.0) - C * C * pow(p[3][4] + x[3] - p[3][3], 2.0);
	/********************************/
}

void fcn_hw_3_1(int* n, double* x, double* fvec, double* fjac, int* ldfjac, int* iflag) {
	if (*iflag == 1) {
		/********************************/
		fvec[0] = pow(x[0] - p[0][0], 2.0) + pow(x[1] - p[0][1], 2.0) + pow(x[2] - p[0][2], 2.0) - C * C * pow(p[0][4] + x[3] - p[0][3], 2.0);
		fvec[1] = pow(x[0] - p[1][0], 2.0) + pow(x[1] - p[1][1], 2.0) + pow(x[2] - p[1][2], 2.0) - C * C * pow(p[1][4] + x[3] - p[1][3], 2.0);
		fvec[2] = pow(x[0] - p[2][0], 2.0) + pow(x[1] - p[2][1], 2.0) + pow(x[2] - p[2][2], 2.0) - C * C * pow(p[2][4] + x[3] - p[2][3], 2.0);
		fvec[3] = pow(x[0] - p[3][0], 2.0) + pow(x[1] - p[3][1], 2.0) + pow(x[2] - p[3][2], 2.0) - C * C * pow(p[3][4] + x[3] - p[3][3], 2.0);
		/********************************/
	}
	// Jacobi matrix J(x)
	else if (*iflag == 2) {
		/********************************/
		int col = 0;
		int row = 0;


		for (int i = 0; i < 4; i++) {
			fjac[0 + i] = 2 * x[0] - 2 * p[i][0];
		}
		for (int i = 0; i < 4; i++) {
			fjac[4 + i] = 2 * x[1] - 2 * p[i][1];
		}
		for (int i = 0; i < 4; i++) {
			fjac[8 + i] = 2 * x[2] - 2 * p[i][2];
		}
		for (int i = 0; i < 4; i++) {
			fjac[12 + i] = -2 * C * C * (p[i][4] + x[3] - p[i][3]);
		}

		/********************************/
	}
}


void homework3_1() {
	int n = SOLNUMS;
	double x[SOLNUMS] = { 0.0, 0.0, 0.0, 0.0 };	//need to initilize x0
	double x_org[SOLNUMS] = { 0.0, 0.0, 0.0, 0.0 };
	double fvec[SOLNUMS], fjac[MATCOLS * MATROWS];
	double fvec2[SOLNUMS];
	int ldfjac = SOLNUMS;
	double tol = TOLERANCE;
	int info;
	double wa[(SOLNUMS * (SOLNUMS + 13)) / 2];
	int lwa = (SOLNUMS * (SOLNUMS + 13)) / 2;

	// allocation p
	p = (double**)malloc(sizeof(double*) * 4);
	for (int j = 0; j < 4; j++) p[j] = (double*)malloc(sizeof(double) * 5);


	for (int i = 0; i < 3; i++) {
		char readfile[256];
		char writefile1[256];
		char writefile2[256];

		sprintf(readfile, "GPS_signal_%d.txt", i);
		sprintf(writefile1, "GPS_position_3-1_%d.txt", i);
		sprintf(writefile2, "GPS_position_3-2_%d.txt", i);
		
		FILE* fp_r = fopen(readfile, "r");
		FILE* fp_w1 = fopen(writefile1, "w");
		FILE* fp_w2 = fopen(writefile2, "w");

		if (fp_r == NULL) {
			printf("FILE input Error.");
			return;
		}

		// Read p
		fscanf(fp_r, "%lf", &C);
		fscanf(fp_r, "%lf", &x_org[3]);

		for (int a = 0; a < 4; a++) {
			for (int b = 0; b < 5; b++) {
				fscanf(fp_r, "%lf", &p[a][b]);
			}
		}

		printf("(%d) Enter Initial X value (x0, x1, x2) : ", i);
		scanf("%lf %lf %lf", &x_org[0], &x_org[1], &x_org[2]);

		/* hybrj */ 
		for (int j = 0; j < 4; j++) x[j] = x_org[j];

		hybrj1_(fcn_hw_3_1, &n, x, fvec, fjac, &ldfjac, &tol, &info, wa, &lwa);
		
		int iflag = 1;
		fcn_hw_3_1(&n, x, fvec, fjac, &ldfjac, &iflag);

		for (int j = 0; j < 4; j++) {
			fprintf(fp_w1, "x%d = %.10lf\n", j + 1, x[j]);
		}
		for (int j = 0; j < 4; j++) {
			fprintf(fp_w1, "f%d(x1, x2, x3, x4) = %.10lf\n", j + 1, fvec[j]);
		}
		
		/* hybrd */
		for (int j = 0; j < 4; j++) {
			x[j] = x_org[j];
		}
		double wa2[(SOLNUMS * (3 * SOLNUMS + 13)) / 2];
		int lwa2 = (SOLNUMS * (3 * SOLNUMS + 13)) / 2;

		hybrd1_(fcn_hw_3_1_2, &n, x, fvec2, &tol, &info, wa2, &lwa2);

		iflag = 1;
		fcn_hw_3_1_2(&n, x, fvec, &iflag);

		for (int j = 0; j < 4; j++) {
			fprintf(fp_w2, "x%d = %.10lf\n", j + 1, x[j]);
		}
		for (int j = 0; j < 4; j++) {
			fprintf(fp_w2, "f%d(x1, x2, x3, x4) = %.10lf\n", j + 1, fvec[j]);
		}

		fclose(fp_r);
		fclose(fp_w1);
		fclose(fp_w2);

	}
	
	return;
}