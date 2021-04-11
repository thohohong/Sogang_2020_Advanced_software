#include "my_solver.h"

extern double (*_f)(double);
extern double (*_fp)(double);

/*********************************************
  Newton-Rapson Method
**********************************************/
void program1_1(FILE* fp) {
    double next_x;
    double x;

    double f_x;
    double fp_x;
    double x_size;

    int i;
    printf("===================< Newton-Raphson - double >==================\n");
    fprintf(fp, "===================< Newton-Raphson - double >==================\n");

    printf("Enter x0 : ");
    scanf("%lf", &x);


    printf(" i                  xn                |f(xn)|             |f'(xn)|\n");
    fprintf(fp, " i                  xn                |f(xn)|             |f'(xn)|\n");

    for(i = 0;; i++) {
        f_x = _f(x);
        f_x = f_x > 0 ? f_x : f_x * -1;

        fp_x = _fp(x);
        fp_x = fp_x > 0 ? fp_x : fp_x * -1;
        printf("%2d    %20.18e    %12.10e   %12.10e\n", i, x, f_x, fp_x);
        fprintf(fp, "%2d    %20.18e    %12.10e   %12.10e\n", i, x, f_x, fp_x);

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

    f_x = _f(x);
    f_x = f_x > 0 ? f_x : f_x * -1;

    fp_x = _fp(x);
    fp_x = fp_x > 0 ? fp_x : fp_x * -1;

    printf("%2d    %20.18e    %12.10e   %12.10e\n", ++i, x, f_x, fp_x);
    fprintf(fp, "%2d    %20.18e    %12.10e   %12.10e\n", i, x, f_x, fp_x);
}
