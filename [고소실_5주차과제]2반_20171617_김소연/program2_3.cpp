#include "my_solver.h"
#include <math.h>
#include <time.h>


double** data;
double sampling_num;
double u;
int bisection_flag = 0;
double a_, b_;

extern double (*_f)(double);
extern double (*_fp)(double);

// HOMEWORK
void program_homework1()
{
	int num;
	double result;
	double u;
	double lambda;
	double* data;
	unsigned int iseed = (unsigned int)time(NULL);
	srand(iseed);
		
	printf("Enter number of random number : ");
	scanf("%d", &num);
	
	printf("Enter lambda : ");
	scanf("%lf", &lambda);

	data = (double*)malloc(sizeof(double) * num);

	/* make random number on the basis of exp */
	for (int i = 0; i < num; i++) {
		u = rand() / (double)(RAND_MAX + 1);
		result = -1 * log(1 - u) / lambda;
		printf("%.15lf\n", result);
		data[i] = result;
	}

	/* Check E(x), V(x) */
	double E = 0;
	double V = 0;

	for (int i = 0; i < num; i++) {
		E += data[i];
	}
	E /= num;
	
	for (int i = 0; i < num; i++) {
		V += (data[i] - E) * (data[i] - E);
	}
	V /= num;

	/* print result */
	printf("============== Expected data ==============\n");
	printf("E(X) = %.6lf, V(X) = %.6lf\n", 1 / lambda, 1 / (lambda * lambda));
	printf("============== Actual Data ================\n");
	printf("E(X) = %.6lf, V(X) = %.6lf\n", E, V);
}





double _f2(double x) {
	double result;

	result = integral_(data, 0.0, x, sampling_num) - u;

	return result;
}

double _fp2(double x) {
	double result = 0;
	double s;

	for (int i = 0; i < sampling_num - 1; i++) {
		if (x > data[i][0] && x < data[i + 1][0]) {
			s = (x - data[i][0]) / (data[i + 1][0] - data[i][0]);
			result = (1 - s) * data[i][1] + s * data[i + 1][1];
		}
	}
	return result;
}

double Bisection(double a, double b) {
	double result;
	double mid, value, gap;


	/* f(a) < 0, f(b) > 0 */
	if (_f(b) > _f(a)) {
		mid = a;
		a = b;
		b = mid;
	}

	for (int i = 0;; i++) {
		mid = (a + b) / 2;

		value = _f(mid);
		value = value > 0 ? value : value * -1;

		/* break point (a) */
		if (value < DELTA) {
			break;
		}

		/* break point (b) */
		if (i >= Nmax) {
			break;
		}

		/* break special */
		if (bisection_flag > 0 && i == bisection_flag) {
			bisection_flag = 0;
			a_ = a;
			b_ = b;
			break;
		}
			
		gap = b - a;
		gap = gap > 0 ? gap : gap * -1;
		/* break point (c) */
		if (gap < EPSILON) {
			break;
		}

		if (_f(a) * _f(mid) < 0) {
			b = mid;
		}
		else {
			a = mid;
		}
	}

	result = mid;

	return mid;
}
/*********************************************
  Newton-Rapson Method
**********************************************/

double Newton(double x) {
	double next_x;

	double f_x;
	double fp_x;
	double x_size;

	for (int i = 0;; i++) {
		f_x = _f(x);
		f_x = f_x > 0 ? f_x : f_x * -1;

		fp_x = _fp(x);
		fp_x = fp_x > 0 ? fp_x : fp_x * -1;
		
		next_x = x - (_f(x) / _fp(x));

		f_x = _f(next_x);
		f_x = f_x > 0 ? f_x : f_x * -1;
		/* break point (a) */
		if (f_x < DELTA) {
			x = next_x;
			break;
		}

		/* break point (b) */
		if (i >= Nmax - 1) {
			x = next_x;
			break;
		}

		x_size = next_x - x;
		x_size = x_size > 0 ? x_size : x_size * -1;
		/* break point (c) */
		if (x_size < EPSILON) {
			x = next_x;
			break;
		}

		x = next_x;
	}

	return x;
}

/*********************************************
  Secant Method
**********************************************/
double Secant(double prev_x, double x) {
	double next_x;
	
	double f_x;
	double fp_x;
	double x_size;
	
	for (int i = 0; ; i++) {
		f_x = _f(x);
		f_x = f_x > 0 ? f_x : f_x * -1;

		fp_x = _fp(x);
		fp_x = fp_x > 0 ? fp_x : fp_x * -1;

		next_x = x - _f(x) * (x - prev_x) / (_f(x) - _f(prev_x));

		f_x = _f(next_x);
		f_x = f_x > 0 ? f_x : f_x * -1;
		/* break point (a) */
		if (f_x < DELTA) {
			prev_x = x;
			x = next_x;
			break;
		}

		/* break point (b) */
		if (i >= Nmax - 1) {
			prev_x = x;
			x = next_x;
			break;
		}

		x_size = next_x - x;
		x_size = x_size > 0 ? x_size : x_size * -1;
		/* break point (c) */
		if (x_size < EPSILON) {
			prev_x = x;
			x = next_x;
			break;
		}

		prev_x = x;
		x = next_x;
	}

	return x;
}



void program2_3() {
	int num_histogram;
	int num_random;
	double cur;
	double* r_data;
	double** p_data;
	int* histogram;
	
	int sampling_num;
	double h;

	FILE* fp_p = fopen("pdf_table.txt", "r");
	FILE *fp_r = fopen("random_event_table.txt", "r");
	FILE* fp_w = fopen("histogram.txt", "w");
	
	fscanf(fp_p, "%d %lf", &sampling_num, &h);

	p_data = (double**)malloc(sizeof(double*) * sampling_num);
	for (int i = 0; i < sampling_num; i++) {
		p_data[i] = (double*)malloc(sizeof(double) * 2);
		fscanf(fp_p, "%lf %lf", &p_data[i][0], &p_data[i][1]);
	}
	
	printf("Enter number of histogram label : ");
	scanf("%d", &num_histogram);
	histogram = (int*)calloc(num_histogram, sizeof(int));

	fscanf(fp_r, "%d", &num_random);
	r_data = (double*)malloc(sizeof(double) * num_random);


	for (int i = 0; i < num_random; i++) {
		fscanf(fp_r, "%lf", &r_data[i]);
	}

	for (int i = 0; i < num_random; i++) {
		for (int j = 0; j < num_histogram; j++) {
			if (r_data[i] - (double)j / num_histogram < 1.0 / num_histogram) {
				histogram[j]++;
				break;
			}
		}
	}

	fprintf(fp_w, "================= range : number (ratio) ==================== \n");
	printf("================= range : number (ratio) ==================== \n");
	for (int i = 0; i < num_histogram; i++) {
		fprintf(fp_w, "%.6lf ~ %.6lf : %5d (%.6lf)\n", (double)i / num_histogram, ((double)i + 1) / num_histogram, histogram[i], (double)histogram[i] / num_random);
		printf("%.6lf ~ %.6lf : %5d (%.6lf)\n", (double)i / num_histogram, ((double)i + 1) / num_histogram, histogram[i], (double)histogram[i] / num_random);
	}
	
	fprintf(fp_w, "================= range : integral of p(x) ================== \n");
	printf("================= range : integral of p(x) ================== \n");
	for (int i = 0; i < num_histogram; i++) {
		double integral_result = integral_(p_data, (double)i / num_histogram, ((double)i + 1) / num_histogram, sampling_num);
		fprintf(fp_w, "%.6lf ~ %.6lf : %.6lf\n", (double)i / num_histogram, ((double)i + 1) / num_histogram, integral_result);
		printf("%.6lf ~ %.6lf : %.6lf\n", (double)i / num_histogram, ((double)i + 1) / num_histogram, integral_result);
	}
	fclose(fp_p);
	fclose(fp_r);
	fclose(fp_w);
}


// HOMEWORK
void program2_2_a()
{
	__int64 start, freq, end;
	float resultTime = 0;

	CHECK_TIME_START;

	// something to do...
	FILE* fp_r, * fp_w;

	fp_r = fopen("pdf_table.txt", "r");
	fp_w = fopen("random_event_table.txt", "w");

	/***************************************************/
	int num;
	double result;
	double h;
	double x, y;

	unsigned int iseed = (unsigned int)time(NULL);
	srand(iseed);

	_f = _f2;

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
		result = Bisection(0.0, 1.0);
		fprintf(fp_w, "%.15lf\n", result);
		printf("%.15lf\n", result);
	}

	/***************************************************/

	if (fp_r != NULL) fclose(fp_r);
	if (fp_w != NULL) fclose(fp_w);

	CHECK_TIME_END(resultTime);

	printf("The program2_2_a run time is %f(ms)..\n", resultTime * 1000.0);
}

void program2_2_b()
{
	__int64 start, freq, end;
	float resultTime = 0;

	CHECK_TIME_START;

	// something to do...
	FILE* fp_r, * fp_w;

	fp_r = fopen("pdf_table.txt", "r");
	fp_w = fopen("random_event_table.txt", "w");

	/***************************************************/
	int num;
	double result;
	double h;
	double x, y;

	unsigned int iseed = (unsigned int)time(NULL);
	srand(iseed);

	_f = _f2;
	_fp = _fp2;

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
		bisection_flag = 5;
		result = Bisection(0.0, 1.0);
		//result = Newton(result);
		result = Secant(a_, b_);
		fprintf(fp_w, "%.15lf\n", result);
		printf("%.15lf\n", result);
	}

	/***************************************************/

	if (fp_r != NULL) fclose(fp_r);
	if (fp_w != NULL) fclose(fp_w);

	CHECK_TIME_END(resultTime);

	printf("The program2_2_b run time is %f(ms)..\n", resultTime * 1000.0);
}