#include <stdio.h>
#include <string.h>
#include <random>
#include <time.h>

#include <math.h>
#include <time.h>
#include <Windows.h>

__int64 start, freq, end;
#define CHECK_TIME_START QueryPerformanceFrequency((LARGE_INTEGER*)&freq); QueryPerformanceCounter((LARGE_INTEGER*)&start)
#define CHECK_TIME_END(a) QueryPerformanceCounter((LARGE_INTEGER*)&end); a = (float)((float)(end - start) / (freq / 1000.0f))
float compute_time;
float compute_time1, compute_time2;


#define MATDIM 1024
#define HW1_N 1000
#define HW3_N 100000
#define HW3_2_N 100
#define HW3_3_N 10000

float *hw1_x, hw1_E, hw1_var1, hw1_var2;
float hw2_a, hw2_b, hw2_c, hw2_naive_ans[2], hw2_pre_ans[2];

/* hw1 */
void init_hw1(int fixed);
void hw1_calc_e();
void hw1_calc_var1();
void hw1_calc_var2();
/* hw2 */
void hw2_naive();
void hw2_safe();
float hw2_verify(float x);
/* hw3 */
void hw3_code_motion();
void hw3_function_inlining();
void hw3_strength_reduction();
void hw3_strength_reduction2();
void hw3_bitwise_operation();
int calculate_sum(int a, int b);



void main(void)
{
	srand((unsigned)time(NULL));

	/* hw1 */
	puts("====== hw1 ======");
	init_hw1(0);
	hw1_calc_e();

	CHECK_TIME_START;
	hw1_calc_var1();
	CHECK_TIME_END(compute_time);
	compute_time1 = compute_time;
	printf("hw1_calc_var1 = %f ms\n", compute_time);
	printf("hw1_calc_var1 value = %.15f\n", hw1_var1);

	CHECK_TIME_START;
	hw1_calc_var2();
	CHECK_TIME_END(compute_time);
	compute_time2 = compute_time;
	printf("hw1_calc_var2 = %f ms\n", compute_time);
	printf("hw1_calc_var2 value = %.15f\n", hw1_var2);
	puts("");
	
	/* hw2 */
	puts("====== hw2 ======");
	printf("a, b, c : ");
	scanf("%f %f %f", &hw2_a, &hw2_b, &hw2_c);
	hw2_naive();
	hw2_safe();
	printf("naive result    : %.15f, %.15f\n", hw2_naive_ans[0], hw2_naive_ans[1]);
	printf("advanced result : %.15f, %.15f\n", hw2_pre_ans[0], hw2_pre_ans[1]);
	puts("");
	printf("Verifying naive ans    : %.15f, %.15f\n", hw2_verify(hw2_naive_ans[0]), hw2_verify(hw2_naive_ans[1]));
	printf("Verifying advanced ans : %.15f, %.15f\n", hw2_verify(hw2_pre_ans[0]), hw2_verify(hw2_pre_ans[1]));
	puts("");

	/* hw3 */
	puts("====== hw3 ======");
	puts("<< Method 1 : Code motion >>");
	hw3_code_motion();

	puts("");
	puts("");
	puts("<< Method 2 : function inlining >>");
	hw3_function_inlining();

	puts("");
	puts("");
	puts("<< Method 3 : strenth_reduction - division into production >>");
	hw3_strength_reduction();

	puts("");
	puts("");
	puts("<< Method 4 : strenth_reduction - division into shift >>");
	hw3_strength_reduction2();

	puts("");
	puts("");
	puts("<< Method 5 : bitwise operation >>");
	hw3_bitwise_operation();
}

void init_hw1(int fixed)
{
	float *ptr;
	hw1_x = (float *)malloc(sizeof(float)*HW1_N);

	if (fixed)
	{
		float tmp = HW1_N;
		for (int i = 0; i < HW1_N; i++)
		{
			if( i & 1) tmp -= 0.0001;
			hw1_x[i] = tmp;
		}
	}
	else
	{
		srand((unsigned)time(NULL));
		ptr = hw1_x;
		for (int i = 0; i < HW1_N; i++)
			*ptr++ = ((float)rand() / (float)RAND_MAX) * 2;
	}
}
void hw1_calc_e()
{
	hw1_E = 0.0;
	for (int i = 0; i < HW1_N; i++) {
		hw1_E += hw1_x[i];
	}
	hw1_E /= HW1_N;
}

void hw1_calc_var1()
{
	float sum1 = 0.0, sum2 = 0.0;

	for (int i = 0; i < HW1_N; i++) {
		sum1 += hw1_x[i] * hw1_x[i];
		sum2 += hw1_x[i];
	}
	hw1_var1 = (HW1_N * sum1 - sum2 * sum2) / (float)(HW1_N * (HW1_N - 1));
	//hw1_var1 = sqrt(hw1_var1);
}

void hw1_calc_var2()
{
	hw1_var2 = 0.0;

	for (int i = 0; i < HW1_N; i++) {
		hw1_var2 += (hw1_x[i] - hw1_E) * (hw1_x[i] - hw1_E);
	}
	hw1_var2 = hw1_var2 / (float)(HW1_N - 1.0);
	//hw1_var2 = sqrt(hw1_var2);
}


/* hw2 */
void hw2_naive()
{
	float d = sqrt(hw2_b * hw2_b - 4 * hw2_a * hw2_c);
	hw2_naive_ans[0] = (-1 * hw2_b + d) / (2 * hw2_a);
	hw2_naive_ans[1] = (-1 * hw2_b - d) / (2 * hw2_a);
}
void hw2_safe()
{	
	float d = sqrt(hw2_b * hw2_b - 4 * hw2_a * hw2_c);
	if (hw2_b < 0) {
		hw2_pre_ans[0] = (-1 * hw2_b + d) / (2 * hw2_a);
		hw2_pre_ans[1] = (-2 * hw2_c) / (hw2_b - d);
	}
	else{
		hw2_pre_ans[0] = (-2 * hw2_c) / (hw2_b + d);
		hw2_pre_ans[1] = (-1 * hw2_b - d) / (2 * hw2_a);
	}

}

float hw2_verify(float x)
{
	return hw2_a * x * x + hw2_b*x + hw2_c;
}

void hw3_code_motion() {
	srand((unsigned)time(NULL));
	
	printf("Calculate sum of x*x + i\n");
	printf("x is 3, i is integer from 1 to N\n\n");
	
	int x = 3;
	int sum = 0;

	CHECK_TIME_START;
	for (int i = 1; i <= HW3_N; i++) {
		sum += x * x + i;
	}
	CHECK_TIME_END(compute_time);
	printf("(Naive) result = %d\n", sum);
	printf("(Naive) consumed time = %f ms\n", compute_time);

	sum = 0;
	CHECK_TIME_START;
	int xx = x * x;
	for (int i = 1; i <= HW3_N; i++) {
		sum += xx + i;
	}
	CHECK_TIME_END(compute_time);
	printf("(Advanced) result = %d\n", sum);
	printf("(Advanced) consumed time = %f ms\n", compute_time);

}

void hw3_function_inlining() {
	srand((unsigned)time(NULL));

	printf("Caculate sum of (ai + bi)^2\n");
	printf("(i is integer from 0 to 1000)\n\n");

	int rand1[HW3_2_N];
	int rand2[HW3_2_N];

	for (int i = 0; i < HW3_2_N; i++) {
		rand1[i] = rand() % 100;
		rand2[i] = rand() % 100;
	}

	int sum = 0;
	CHECK_TIME_START;
	for (int i = 0; i < HW3_2_N; i++) {
		sum += calculate_sum(rand1[i], rand2[i]) * calculate_sum(rand1[i], rand2[i]);
	}

	CHECK_TIME_END(compute_time);
	printf("(Naive) result = %d\n", sum);
	printf("(Naive) consumed time = %f ms\n", compute_time);

	sum = 0;
	CHECK_TIME_START;
	for (int i = 0; i < HW3_2_N; i++) {
		sum += (rand1[i] + rand2[i]) * (rand1[i] + rand2[i]);
	}
	CHECK_TIME_END(compute_time);
	printf("(Advanced) result = %d\n", sum);
	printf("(Advanced) consumed time = %f ms\n", compute_time);
}

int calculate_sum(int a, int b) {
	return a + b;
}

// division into production
void hw3_strength_reduction() {
	srand((unsigned)time(NULL));

	printf("Calculate sum of xi / 3 \n");
	printf("xi is integer from 0 to 99 in array A[i]\n\n");

	int c = 3;
	float A[HW3_3_N];

	for (int i = 0; i < HW3_3_N; i++) {
		A[i] = (float)(rand() % 100);
	}

	float sum = 0.0;
	CHECK_TIME_START;
	for (int i = 0; i < HW3_3_N; i++) {
		sum += A[i] / c;
	}
	CHECK_TIME_END(compute_time);
	printf("(Naive) result = %f\n", sum);
	printf("(Naive) consumed time = %f ms\n", compute_time);

	sum = 0.0;
	CHECK_TIME_START;
	double tmp = 1.0 / c;
	for (int i = 0; i < HW3_3_N; i++) {
		sum += A[i] * tmp;
	}
	CHECK_TIME_END(compute_time);
	printf("(Advanced) result = %f\n", sum);
	printf("(Advanced) consumed time = %f ms\n", compute_time);


}

//division into shift
void hw3_strength_reduction2() {
	srand((unsigned)time(NULL));

	printf("Calculate sum of xi / 8 \n");
	printf("xi is integer from 0 to 99 in array A[i]\n\n");

	int c = 8;
	int A[HW3_3_N];

	for (int i = 0; i < HW3_3_N; i++) {
		A[i] = rand() % 100;
	}

	int sum = 0;
	CHECK_TIME_START;
	for (int i = 0; i < HW3_3_N; i++) {
		sum += A[i] / c;
	}
	CHECK_TIME_END(compute_time);
	printf("(Naive) result = %d\n", sum);
	printf("(Naive) consumed time = %f ms\n", compute_time);

	sum = 0;
	CHECK_TIME_START;
	for (int i = 0; i < HW3_3_N; i++) {
		sum += A[i] >> 3;
	}
	CHECK_TIME_END(compute_time);
	printf("(Advanced) result = %d\n", sum);
	printf("(Advanced) consumed time = %f ms\n", compute_time);
}

void hw3_bitwise_operation() {
	srand((unsigned)time(NULL));

	printf("count number of odd number in array A[]\n\n");

	int A[HW3_3_N];

	for (int i = 0; i < HW3_3_N; i++) {
		A[i] = rand() % 100;
	}

	int count = 0;
	CHECK_TIME_START;
	for (int i = 0; i < HW3_3_N; i++) {
		if (A[i] % 2 == 1) {
			count++;
		}
	}
	CHECK_TIME_END(compute_time);
	printf("(Naive) odd number count = %d\n", count);
	printf("(Naive) consumed time = %f ms\n", compute_time);

	count = 0;
	CHECK_TIME_START;
	for (int i = 0; i < HW3_3_N; i++) {
		if (A[i] & 1) {
			count++;
		}
	}
	CHECK_TIME_END(compute_time);
	printf("(Advanced) odd number count = %d\n", count);
	printf("(Advanced) consumed time = %f ms\n", compute_time);
}


