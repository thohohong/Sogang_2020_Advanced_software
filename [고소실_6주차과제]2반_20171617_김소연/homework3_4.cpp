#include "my_solver.h"

#define index_2d(row, col, dim) row*dim+col

extern "C"
{
    int gespp_(int*, float*, int*, int*, float*);
    int solve_(int*, float*, int*, int*, float*, float*);
};

void homework3_4() {
    int i, ia, j, n;
    int* l;
    float* a, * b, * x, * s;
    //float a[16] b[4], x[4], s[4];

    FILE* fp_r = fopen("linear_system_3-4.txt", "r");
    FILE* fp_w = fopen("solution_3-4.txt", "w");

    fscanf(fp_r, "%d", &n);

    ia = n;

    a = (float*)malloc(sizeof(float) * n * n);
    b = (float*)malloc(sizeof(float) * n);
    x = (float*)malloc(sizeof(float) * n);
    s = (float*)malloc(sizeof(float) * n);
    l = (int*)malloc(sizeof(int) * n);

    for (int i = 0; i < n * n; i++) {
        fscanf(fp_r, "%f", &a[i]);
    }
    for (int i = 0; i < n; i++) {
        fscanf(fp_r, "%f", &b[i]);
    }

    gespp_(&n, a, &ia, l, s);
    solve_(&n, a, &ia, l, b, x);

    fprintf(fp_w, "%d\n", n);
    for (int i = 0; i < n; i++) {
        fprintf(fp_w, "%.6f\n", x[i]);
    }

    float sum = 0;
    float local_sum = 0;
    float b_sum = 0;

    for (int i = 0; i < n; i++) {
        local_sum = 0;
        for (int j = 0; j < n; j++) {
            local_sum += a[index_2d(i, j, n)] * x[j];
        }
        local_sum -= b[i];
        sum += pow(local_sum, 2.0);
    }
    sum = sqrt(sum);

    for (int i = 0; i < n; i++) {
        b_sum += pow(b[i], 2.0);
    }
    b_sum = sqrt(b_sum);

    sum /= b_sum;

    printf("%f\n", sum);
    fprintf(fp_w, "%f\n", sum);
}
