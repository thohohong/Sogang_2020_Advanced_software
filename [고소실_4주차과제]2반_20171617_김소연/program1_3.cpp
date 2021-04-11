#include "my_solver.h"

extern double (*_f)(double);
extern double (*_fp)(double);

/*********************************************
  Bisection Method
**********************************************/
void program1_3(FILE* fp) {
    double a, b, mid, value, gap;

    printf("\n=======================< Bisection >======================\n");
    fprintf(fp, "\n=======================< Bisection >======================\n");
    printf("Enter start point : ");
    scanf("%lf", &a);
    printf("Enter end point : ");
    scanf("%lf", &b);

    /* f(a) < 0, f(b) > 0 */
    if (_f(b) > _f(a)) {
        mid = a;
        a = b;
        b = mid;
    }
    printf(" i                  xn                f(xn)\n");
    fprintf(fp, " i                  xn                f(xn)\n");


    for (int i = 0;;i++) {
        mid = (a + b) / 2;
        printf("%2d    %20.18e    %12.10e\n", i, mid, _f(mid));
        fprintf(fp, "%2d    %20.18e    %12.10e\n", i, mid, _f(mid));

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

}