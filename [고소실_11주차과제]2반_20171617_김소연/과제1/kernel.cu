#include "cuda_runtime.h"
#include "device_launch_parameters.h"

#include <stdio.h>
#include<stdio.h>
#include<stdlib.h>
#include <math.h>
#include <Windows.h>
#include <time.h>
#include <assert.h>

#define CUDA_CALL(x) { const cudaError_t a = (x); if(a != cudaSuccess) { printf("\nCuda Error: %s (err_num=%d) at line:%d\n", cudaGetErrorString(a), a, __LINE__); cudaDeviceReset(); assert(0);}}
typedef float TIMER_T;

#define USE_CPU_TIMER 1
#define USE_GPU_TIMER 1

#if USE_CPU_TIMER == 1
__int64 start, freq, end;
#define CHECK_TIME_START { QueryPerformanceFrequency((LARGE_INTEGER*)&freq); QueryPerformanceCounter((LARGE_INTEGER*)&start); }
#define CHECK_TIME_END(a) { QueryPerformanceCounter((LARGE_INTEGER*)&end); a = (float)((float)(end - start) / (freq / 1000.0f)); }
#else
#define CHECK_TIME_START
#define CHECK_TIME_END(a)
#endif

#if USE_GPU_TIMER == 1
cudaEvent_t cuda_timer_start, cuda_timer_stop;
#define CUDA_STREAM_0 (0)

void create_device_timer()
{
    CUDA_CALL(cudaEventCreate(&cuda_timer_start));
    CUDA_CALL(cudaEventCreate(&cuda_timer_stop));
}

void destroy_device_timer()
{
    CUDA_CALL(cudaEventDestroy(cuda_timer_start));
    CUDA_CALL(cudaEventDestroy(cuda_timer_stop));
}

inline void start_device_timer()
{
    cudaEventRecord(cuda_timer_start, CUDA_STREAM_0);
}

inline TIMER_T stop_device_timer()
{
    TIMER_T ms;
    cudaEventRecord(cuda_timer_stop, CUDA_STREAM_0);
    cudaEventSynchronize(cuda_timer_stop);

    cudaEventElapsedTime(&ms, cuda_timer_start, cuda_timer_stop);
    return ms;
}

#define CHECK_TIME_INIT_GPU() { create_device_timer(); }
#define CHECK_TIME_START_GPU() { start_device_timer(); }
#define CHECK_TIME_END_GPU(a) { a = stop_device_timer(); }
#define CHECK_TIME_DEST_GPU() { destroy_device_timer(); }
#else
#define CHECK_TIME_INIT_GPU()
#define CHECK_TIME_START_GPU()
#define CHECK_TIME_END_GPU(a)
#define CHECK_TIME_DEST_GPU()
#endif

#define BLOCK_SIZE 128
#define DATA_SIZE (1 << 26)
#define N_EQUATIONS 1<<20

TIMER_T compute_time = 0;
TIMER_T device_time = 0;

int N;
float* A;
float* B;
float* C;

float* X0_CPU;
float* X1_CPU;
float* FX0_CPU;
float* FX1_CPU;

float* X0_GPU;
float* X1_GPU;
float* FX0_GPU;
float* FX1_GPU;


cudaError_t find_roots_GPU(float* A, float* B, float* C, float* X0, float* X1, float* FX0, float* FX1, int n);

__global__ void find_roots_Kernel(float* A, float* B, float* C, float* X0, float* X1, float* FX0, float* FX1)
{
    /*Thread Idx*/
    int row;
    int col;
    int id = blockDim.x * blockIdx.x + threadIdx.x;
    /*Todo*/

    float a, b, c, d, x0, x1, tmp;
    a = A[id]; b = B[id]; c = C[id];
    d = sqrtf(b * b - 4.0f * a * c);
    tmp = 1.0f / (2.0f * a);
    X0[id] = x0 = (-b - d) * tmp;
    X1[id] = x1 = (-b + d) * tmp;
    FX0[id] = (a * x0 + b) * x0 + c;
    FX1[id] = (a * x1 + b) * x1 + c;

}


void find_roots_CPU(float* A, float* B, float* C, float* X0, float* X1, float* FX0, float* FX1, int n) {
    float a, b, c, d, x0, x1, tmp;

    for (int i = 0; i < n; i++) {
        a = A[i]; b = B[i]; c = C[i];
        d = sqrtf(b * b - 4.0f * a * c);
        tmp = 1.0f / (2.0f * a);
        X0[i] = x0 = (-b - d) * tmp;
        X1[i] = x1 = (-b + d) * tmp;
        FX0[i] = (a * x0 + b) * x0 + c;
        FX1[i] = (a * x1 + b) * x1 + c;
    }
}

void init_bin_file(int size) {
    srand((unsigned)time(NULL));
    printf("***Binary File init Start!!\n");
    FILE* fp_A = fopen("A.bin", "wb");
    FILE* fp_B = fopen("B.bin", "wb");
    FILE* fp_C = fopen("C.bin", "wb");

    float random_num;
    for (int i = 0; i < size; i++) {
        random_num = (float)rand() / RAND_MAX;
        fwrite(&random_num, sizeof(float), 1, fp_A);
    }
    for (int i = 0; i < size; i++) {
        random_num = (float)rand() / RAND_MAX;
        fwrite(&random_num, sizeof(float), 1, fp_B);
    }
    for (int i = 0; i < size; i++) {
        random_num = (float)rand() / RAND_MAX;
        fwrite(&random_num, sizeof(float), 1, fp_C);
    }

    fclose(fp_A);
    fclose(fp_B);
    fclose(fp_C);
    printf("***Binary File init End!!\n\n");
}

void read_bin_file()
{
    printf("***Binary File Read Start!!\n");
    FILE* fp_A = fopen("A.bin", "rb");
    FILE* fp_B = fopen("B.bin", "rb");
    FILE* fp_C = fopen("C.bin", "rb");

    A = (float*)malloc(sizeof(float) * N_EQUATIONS);
    B = (float*)malloc(sizeof(float) * N_EQUATIONS);
    C = (float*)malloc(sizeof(float) * N_EQUATIONS);

    X0_CPU = (float*)malloc(sizeof(float) * N_EQUATIONS);
    X1_CPU = (float*)malloc(sizeof(float) * N_EQUATIONS);
    FX0_CPU = (float*)malloc(sizeof(float) * N_EQUATIONS);
    FX1_CPU = (float*)malloc(sizeof(float) * N_EQUATIONS);

    X0_GPU = (float*)malloc(sizeof(float) * N_EQUATIONS);
    X1_GPU = (float*)malloc(sizeof(float) * N_EQUATIONS);
    FX0_GPU = (float*)malloc(sizeof(float) * N_EQUATIONS);
    FX1_GPU = (float*)malloc(sizeof(float) * N_EQUATIONS);

    fread(A, sizeof(float), N_EQUATIONS, fp_A);
    fread(B, sizeof(float), N_EQUATIONS, fp_B);
    fread(C, sizeof(float), N_EQUATIONS, fp_C);

    fclose(fp_A);
    fclose(fp_B);
    fclose(fp_C);

    printf("***Binary File Read End!!\n\n");
}

int main()
{
    init_bin_file(N_EQUATIONS);
    read_bin_file();

    printf("The problem size is %d.\n", N_EQUATIONS);
    int i;
    FILE* fp_X0, * fp_X1, * fp_FX0, * fp_FX1;

    //CPU 
    printf("***find_roots_CPU Start!!\n");
    CHECK_TIME_START;
    find_roots_CPU(A, B, C, X0_CPU, X1_CPU, FX0_CPU, FX1_CPU, N_EQUATIONS);
    CHECK_TIME_END(compute_time);
    printf("***find_roots_CPU End!!\n\n");

    printf("CPU time = %.6f\n", compute_time);
    //GPU

    printf("***find_roots_GPU Start!!\n");
    find_roots_GPU(A, B, C, X0_GPU, X1_GPU, FX0_GPU, FX1_GPU, N_EQUATIONS);
    printf("***find_roots_GPU End!!\n\n");

    printf("GPU time = %.6f\n", device_time);


    for (i = 0; i < N_EQUATIONS; i++)
    {
        if (X0_CPU[i] != X0_GPU[i])
        {
            printf("%u != %u\n", X0_CPU[i], X0_GPU[i]);
            break;
        }
    }


    if (i == N_EQUATIONS)
        printf("***Kernel execution Success!!\n\n");

    // Write the output array into the output file.
    fp_X0 = fopen("X0.bin", "wb");
    fp_X1 = fopen("X1.bin", "wb");
    fp_FX0 = fopen("FX0.bin", "wb");
    fp_FX1 = fopen("FX1.bin", "wb");

    for (i = 0; i < N; i++) {
        fwrite(&X0_GPU[i], sizeof(float), 1, fp_X0);
        fwrite(&X1_GPU[i], sizeof(float), 1, fp_X1);
        fwrite(&FX0_GPU[i], sizeof(float), 1, fp_FX0);
        fwrite(&FX1_GPU[i], sizeof(float), 1, fp_FX1);
    }

    fclose(fp_X0);
    fclose(fp_X1);
    fclose(fp_FX0);
    fclose(fp_FX1);

    printf("end!!\n\n");

    return 0;
}


cudaError_t find_roots_GPU(float* A, float* B, float* C, float* X0, float* X1, float* FX0, float* FX1, int n)
{


    cudaError_t cudaStatus;
    /*Todo*/

    cudaStatus = cudaSetDevice(0);


    float* d_A, * d_B, * d_C, * d_X0, * d_X1, * d_FX0, * d_FX1;

    size_t size = sizeof(float) * n;
    CUDA_CALL(cudaMalloc(&d_A, size))
        CUDA_CALL(cudaMemcpy(d_A, A, size, cudaMemcpyHostToDevice));

    CUDA_CALL(cudaMalloc(&d_B, size))
        CUDA_CALL(cudaMemcpy(d_B, B, size, cudaMemcpyHostToDevice));

    CUDA_CALL(cudaMalloc(&d_C, size))
        CUDA_CALL(cudaMemcpy(d_C, C, size, cudaMemcpyHostToDevice));

    CUDA_CALL(cudaMalloc(&d_X0, size))
        CUDA_CALL(cudaMalloc(&d_X1, size))
        CUDA_CALL(cudaMalloc(&d_FX0, size))
        CUDA_CALL(cudaMalloc(&d_FX1, size))
        dim3 dimBlock(BLOCK_SIZE, 1);
    dim3 dimGrid(n / dimBlock.x, 1);

    CHECK_TIME_INIT_GPU()
        CHECK_TIME_START_GPU()
        find_roots_Kernel << < dimGrid, dimBlock >> > (d_A, d_B, d_C, d_X0, d_X1, d_FX0, d_FX1);
    CHECK_TIME_END_GPU(device_time)
        CHECK_TIME_DEST_GPU();

    CUDA_CALL(cudaGetLastError())
        CUDA_CALL(cudaDeviceSynchronize())
        CUDA_CALL(cudaMemcpy(X0, d_X0, size, cudaMemcpyDeviceToHost))
        CUDA_CALL(cudaMemcpy(X1, d_X1, size, cudaMemcpyDeviceToHost))
        CUDA_CALL(cudaMemcpy(FX0, d_FX0, size, cudaMemcpyDeviceToHost))
        CUDA_CALL(cudaMemcpy(FX1, d_FX1, size, cudaMemcpyDeviceToHost))

        Error:

    cudaFree(d_A);
    cudaFree(d_B);
    cudaFree(d_C);
    cudaFree(d_X0);
    cudaFree(d_X1);
    cudaFree(d_FX0);
    cudaFree(d_FX1);



    return cudaStatus;
}