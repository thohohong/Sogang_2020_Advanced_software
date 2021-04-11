#include "my_solver.h"
#include <time.h>
#include <math.h>
#include <stdlib.h>

double integral_2(double** data, double a, double b, double size) {
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

double _f(double** data, double sampling_num, double x, double u) {
    double result;
    double h;
    
    result = integral_2(data, 0.0, x, sampling_num) - u;

    return result;
}


double Bisection(double** data, double sampling_num, double a, double b, double u) {
	double result;
    double mid, value, gap;

    
    /* f(a) < 0, f(b) > 0 */
    if (_f(data, sampling_num, b, u) > _f(data, sampling_num, a, u)) {
        mid = a;
        a = b;
        b = mid;
    }
    
    for (int i = 0;; i++) {
        mid = (a + b) / 2;
    
        value = _f(data, sampling_num, mid, u);
        value = value > 0 ? value : value * -1;

        /* break point (a) */
        if (value < DELTA) {
            break;
        }

        /* break point (b) */
        if (i >= Nmax) {
            break;
        }

        gap = b - a;
        gap = gap > 0 ? gap : gap * -1;
        /* break point (c) */
        if (gap < EPSILON) {
            break;
        }

        if (_f(data, sampling_num, a, u) * _f(data, sampling_num, mid, u) < 0) {
            b = mid;
        }
        else {
            a = mid;
        }
    }
    
    result = mid;

	return mid;
}

void program2_2()
{
	FILE* fp_r, *fp_w;

	fp_r = fopen("pdf_table.txt", "r");
	fp_w = fopen("random_event_table.txt", "w");

	/***************************************************/
	int num;
    double u;
    double result;
    double sampling_num, h;
    double** data;
    double x, y;

	unsigned int iseed = (unsigned int)time(NULL);
	srand(iseed);

	
	printf("Enter number of random number : ");
	scanf("%d", &num);
    fprintf(fp_w, "%d\n", num);

    fscanf(fp_r, "%lf %lf", &sampling_num, &h);

    data = (double**)malloc(sizeof(double*) * sampling_num);

    for (int i = 0; i < sampling_num; i++) {
        data[i] = (double*)malloc(sizeof(double) * 2);
        fscanf(fp_r, "%lf %lf", &x, &y);
        data[i][0] = x;
        data[i][1] = y;
    }

    for (int i = 0; i < num; i++) {
        u = rand() / (double)(RAND_MAX + 1);
        result = Bisection(data, sampling_num, 0.0, 1.0, u);
        fprintf(fp_w, "%.15lf\n", result);
        printf("%.15lf\n", result);
    }

	/***************************************************/

	if (fp_r != NULL) fclose(fp_r);
	if (fp_w != NULL) fclose(fp_w);
}
