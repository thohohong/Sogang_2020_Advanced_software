#ifndef __MY_SOLVER_H__
#define __MY_SOLVER_H__

#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>

#define DELTA 0.000001 //�� ������ �Լ����� �۾ƾ� ��
#define Nmax 50 //�ݺ����� �����ؾ��ϴ� �ּ����� Ƚ��
#define EPSILON 0.00001 //

double _f0(double x);
double _fp0(double x);

double _f1_1(double x);
double _fp1_1(double x);

double _f1_2(double x);
double _fp1_2(double x);

double _f2(double x);
double _fp2(double x);

float _sp_f1_1(float x);
float _sp_fp1_1(float x);

float _sp_f1_2(float x);
float _sp_fp1_2(float x);

float _sp_f4(float x);
float _sp_fp4(float x);

double _f4(double x);
double _fp4(double x);
#endif  // __MY_SOLVER_H__
