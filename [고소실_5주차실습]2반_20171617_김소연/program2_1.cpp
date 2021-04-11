#include "my_solver.h"

double integral_(double** data, double a, double b, double size) {
	double result = 0;
	double h = data[1][0] - data[0][0];
	int i = 0;

	while (data[i][0] < a) {
		i++;
	}
	
	while (i < size - 1 && data[i + 1][0] <= b) {
		result += h * (data[i][1] + data[i + 1][1]) / 2;
		i++;
	}
	if (data[i][0] != b) {
		result += (data[i][1] + (data[i + 1][1] - data[i][1]) / h * (b - data[i][0])) * (b - data[i][0]);
	}

	return result;
}

void program2_1()
{
	FILE* fp_r, *fp_w;
	__int64 start, freq, end;
	float resultTime = 0;

	fp_r = fopen("sampling_table.txt", "r");
	if (fp_r == NULL) {
		printf("input file not found....\n");
		exit(0);
	}

	fp_w = fopen("pdf_table.txt", "w");

	/***************************************************/
	int sampling_num;
	double h;
	double pre_x, pre_y;
	double cur_x, cur_y;
	double integral = 0;
	double re_integral = 0;
	double nomalize_h;
	double** data;

	fscanf(fp_r, "%d %lf", &sampling_num, &h);
	fscanf(fp_r, "%lf %lf", &pre_x, &pre_y);

	nomalize_h = 1 / ((double)sampling_num - 1);

	/* integral */
	for (int i = 0; i < sampling_num - 1; i++) {
		fscanf(fp_r, "%lf %lf", &cur_x, &cur_y);
		integral += (pre_y + cur_y) * nomalize_h / 2;
		pre_y = cur_y;
		pre_x = cur_x;
	}
	fclose(fp_r);
	fp_r = fopen("sampling_table.txt", "r");

	/* allocate data */
	data = (double**)malloc(sizeof(double*) * sampling_num);
	
	for (int i = 0; i < sampling_num; i++) {
		data[i] = (double*)malloc(sizeof(double) * 2);
	}

	/* nomalize & print */
	fscanf(fp_r, "%d %lf", &sampling_num, &h);
	fprintf(fp_w, "%d %lf\n", sampling_num, nomalize_h);

	for (int i = 0; i < sampling_num; i++) {
		fscanf(fp_r, "%lf %lf", &cur_x, &cur_y);
		
		cur_x = i * nomalize_h;
		cur_y = cur_y / integral;
		
		fprintf(fp_w, "%lf %lf\n", i * nomalize_h, cur_y);

		data[i][0] = cur_x;
		data[i][1] = cur_y;

		pre_x = cur_x;
		pre_y = cur_y;		
	}
	
	
	re_integral = integral_(data, 0.0, 1.0, 100);
	printf("*** Integrating the pdf from 0.0 to 1.0 = %.6lf\n", re_integral);

	re_integral = integral_(data, 0.0, 0.25, 100);
	printf("*** Integrating the pdf from 0.0 to 0.25 = %.6lf\n", re_integral);

	re_integral = integral_(data, 0.25, 0.5, 100);
	printf("*** Integrating the pdf from 0.25 to 0.5 = %.6lf\n", re_integral);

	re_integral = integral_(data, 0.5, 0.75, 100);
	printf("*** Integrating the pdf from 0.5 to 0.75 = %.6lf\n", re_integral);

	re_integral = integral_(data, 0.75, 1.0, 100);
	printf("*** Integrating the pdf from 0.75 to 1.0 = %.6lf\n", re_integral);
	
	/***************************************************/

	if (fp_r != NULL) fclose(fp_r);
	if (fp_w != NULL) fclose(fp_w);
}
