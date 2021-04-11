#include "my_solver.h"

extern float (*_sp_f)(float);
extern float (*_sp_fp)(float);

/*********************************************
  Secant Method
**********************************************/
void sp_program1_2(FILE* fp) {
    float next_x;
    float x;
    float prev_x;

    float f_x;
    float fp_x;
    float x_size;
    int i;

    printf("\n=======================< Secant - single >======================\n");
    fprintf(fp, "\n=======================< Secant - single >======================\n");

    printf("Enter x0 : ");
    scanf("%f", &prev_x);

    printf("Enter x1 : ");
    scanf("%f", &x);

    printf(" i                  xn                |f(xn)|             |f'(xn)|\n");
    fprintf(fp, " i                  xn                |f(xn)|             |f'(xn)|\n");

    for (i = 0; ; i++) {
        f_x = _sp_f(x);
        f_x = f_x > 0 ? f_x : f_x * -1;

        fp_x = _sp_fp(x);
        fp_x = fp_x > 0 ? fp_x : fp_x * -1;
        printf("%2d    %20.18e    %12.10e   %12.10e\n", i, x, f_x, fp_x);
        fprintf(fp, "%2d    %20.18e    %12.10e   %12.10e\n", i, x, f_x, fp_x);

        next_x = x - _sp_f(x) * (x - prev_x) / (_sp_f(x) - _sp_f(prev_x));

        f_x = _sp_f(next_x);
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

    f_x = _sp_f(x);
    f_x = f_x > 0 ? f_x : f_x * -1;

    fp_x = _sp_fp(x);
    fp_x = fp_x > 0 ? fp_x : fp_x * -1;

    printf("%2d    %20.18e    %12.10e   %12.10e\n", ++i, x, f_x, fp_x);
    fprintf(fp, "%2d    %20.18e    %12.10e   %12.10e\n", i, x, f_x, fp_x);

}
