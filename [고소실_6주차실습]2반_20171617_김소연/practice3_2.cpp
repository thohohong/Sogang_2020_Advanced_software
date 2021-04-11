#include "my_solver.h"

extern "C"
{
	int rpoly_(double*, int*, double*, double*, long int*);
}

void practice3_2() {
	char readfile[256];
	char writefile[256];

	int NCOEF, DEGREE;

	double* poly;
	double* zeror, * zeroi;
	long int fail;
	int i;

	for (int t = 1; t <= 6; t++) {

		sprintf(readfile, "polynomial_3-2_%d.txt", t);
		sprintf(writefile, "roots_3-2_%d.txt", t);

		FILE* fp_r = fopen(readfile, "r");
		if (fp_r == NULL) {
			printf("%s file open error...\n", readfile);
			return;
		}

		FILE* fp_w = fopen(writefile, "w");
		if (fp_w == NULL) {
			printf("%s file open error...\n", writefile);
			return;
		}

		fscanf(fp_r, "%d", &DEGREE);
		NCOEF = DEGREE + 1;

		poly = (double*)malloc(sizeof(double) * NCOEF);
		zeror = (double*)malloc(sizeof(double) * DEGREE);
		zeroi = (double*)malloc(sizeof(double) * DEGREE);


		/********************************/
		int degree = DEGREE;

		for (i = 0; i < NCOEF; i++) {
			fscanf(fp_r, "%lf", &poly[i]);
		}

		printf("====== polynomial_3_2_%d.txt ======\n", t);
		rpoly_(poly, &degree, zeror, zeroi, &fail);
		if (fail) {
			printf("wrong\n");
			free(zeroi);
			free(zeror);
			free(poly);

			if (fp_r != NULL) fclose(fp_r);
			if (fp_w != NULL) fclose(fp_w);
			continue;
		}

		for (i = 0; i < degree; i++) {
			if (zeroi[i] == 0.0) {
				double f_x = 0;
				for (int j = 0; j <= degree; j++) {
					f_x += pow(zeror[i], degree - j) * poly[j];
				}

				printf("x = %.10f + %.10fi, f(x) = %.10lf\n", zeror[i], zeroi[i], f_x);
				fprintf(fp_w, "x = %.10f, f(x) = %.10lf\n", zeror[i], zeroi[i], f_x);
			}
			else {
				printf("x = %.10f + %.10fi\n", zeror[i], zeroi[i]);
				fprintf(fp_w, "x = %.10f + %.10fi\n", zeror[i], zeroi[i]);
			}
		}
		/********************************/


		free(zeroi);
		free(zeror);
		free(poly);

		if (fp_r != NULL) fclose(fp_r);
		if (fp_w != NULL) fclose(fp_w);
	}
}