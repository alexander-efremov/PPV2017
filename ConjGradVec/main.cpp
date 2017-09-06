#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <float.h>
#include <string.h>
#include <fstream>
#include <iostream>
using namespace std;

#define size (int)1024
int numProc;

void NullVector(double *Matrix)
{
	int i;
	for (i = 0; i < size; i++)
		Matrix[i] = 0;
}

void NullMatrix(double **Matrix)
{
	int i, j;
	for (i = 0; i < size; i++)
		for (j = 0; j < size; j++)
			Matrix[i][j] = 0;
}

bool DiagDominationCheck(double **Matrix)
{
	int i, j; double k = 0; bool answer = false;
	for (i = 0; i < size; i++)
	{
		for (j = 0; j < size; j++)
		{
			if (i != j)
			{
				k = k + fabs(Matrix[i][j]);
			}
		}
		if (fabs(Matrix[i][i])>fabs(k))
			answer = true;
		else
			answer = false;
		k = 0;
	}
	return answer;
}

void DiagDomination(double **Matrix)
{
	bool check = false;
	while (check != true)
	{
		int i;
		for (i = 0; i < size; i++)
		{
			Matrix[i][i] = Matrix[i][i] * 100;
		}
		check = DiagDominationCheck(Matrix);
	}
	printf("\nMatrix was rebuilt to diagonal dominance!\n");
}

double ScalarMult(double *V1, double *V2)
{
	int i;
	double scm = 0;
	for (i = 0; i < size; i++)
	{
		scm += V1[i] * V2[i];
	}
	return scm;
}

int main(int argc, char* argv[])
{
	double startTime0, endTime0, startTime1, endTime1;

	 
	int i, j;
	double alpha, betta, d1, d2;
	double EPS = 1e-8; //�������
					   //-----------------��������� ������---------------------------
	double **A = (double**)_mm_malloc(size * sizeof(double*), 64);
	for (i = 0; i < size; i++)
	{
		A[i] = (double*)_mm_malloc(size * sizeof(double), 64);
	}
	double *B = (double*)_mm_malloc(size * sizeof(double), 64);
	double *X = (double*)_mm_malloc(size * sizeof(double), 64);
	double *X_NEW = (double*)_mm_malloc(size * sizeof(double), 64);
	double *r = (double*)_mm_malloc(size * sizeof(double), 64);
	double *r_new = (double*)_mm_malloc(size * sizeof(double), 64);
	double *h = (double*)_mm_malloc(size * sizeof(double), 64);
	double *h_new = (double*)_mm_malloc(size * sizeof(double), 64);
	double *tmp = (double*)_mm_malloc(size * sizeof(double), 64);
	//------------------------------------------------------------
	NullMatrix(A);
	NullVector(B);
	//---������ ������� �� �����---
	ifstream A_F("C:/Users/HOME/Source/Repos/Jakobi/data/m_1024.dat");
	for (i = 0; i < size; i++)
	{
		for (j = 0; j < size; j++)
		{
			A_F >> A[i][j];
		}
	}
	A_F.close();
	//---������ ������� �� �����---
	ifstream B_F("C:/Users/HOME/Source/Repos/Jakobi/data/v_1024.dat");
	for (i = 0; i < size; i++)
	{
		B_F >> B[i];
	}
	B_F.close();
	//-----------------------------
	//�������� ������� �� ������� ������������� ������������
	bool Matrixcheck = DiagDominationCheck(A);
	if (Matrixcheck == false)
	{
		printf("\nMatrix has NOT diagonal dominance!\n");
		DiagDomination(A);
	}
	else
		printf("\nMatrix has diagonal dominance!\n");
	//----------------------CLEAR CODE-----------------
	NullVector(X);
	NullVector(X_NEW);
	NullVector(r);
	NullVector(r_new);
	NullVector(h);
	NullVector(h_new);
	NullVector(tmp);

	//��������������� ����
	//1 ����� X0
	for (i = 0; i < size; i++)
		X[i] = B[i] / A[i][i];
	//2 �������� ������ ������� r0 = b - A*x0
	for (i = 0; i < size; i++)
	{
		for (j = 0; j < size; j++)
		{
			tmp[i] += A[i][j] * X[j];
		}
		r[i] = B[i] - tmp[i];
	}
	//3 �������� ������ ����������� h0 = r0
	memcpy(h, r, size * sizeof(double));
	//������������ �����
	double norm = 1; //�����
	int N_iter = 0; //������� ��������
	while (norm > EPS && N_iter < 10000)
	{
		norm = 0;
		NullVector(tmp);
		//1 ���������� alpha
		d1 = 0; d2 = 0;
		d1 = ScalarMult(r, r);

		for (i = 0; i < size; i++)
		{
			for (j = 0; j < size; j++)
			{
				tmp[i] += A[i][j] * h[j];
			}
		}

		d2 = ScalarMult(tmp, h);
		alpha = d1 / d2;

		//2 ���������� r_new
		for (i = 0; i < size; i++)
		{
			r_new[i] = r[i] - alpha*tmp[i];
		}
		//3 ���������� betta
		d2 = 0;
		d2 = ScalarMult(r_new, r_new);
		betta = d2 / d1;
		//4 ���������� h_new
		for (i = 0; i < size; i++)
		{
			h_new[i] = r_new[i] + betta*h[i];
		}
		//5 ���������� X_NEW
		for (i = 0; i < size; i++)
		{
			X_NEW[i] = X[i] + alpha*h[i];
		}
		//--------------------------------
		for (i = 0; i < size; i++)
		{
			double diff = fabs(X_NEW[i] - X[i]);
			if (diff > norm)
				norm = diff;
		}
		memcpy(X, X_NEW, size * sizeof(double));
		memcpy(h, h_new, size * sizeof(double));
		memcpy(r, r_new, size * sizeof(double));
		NullVector(X_NEW);
		NullVector(r_new);
		NullVector(h_new);
		alpha = 0;
		betta = 0;
		d1 = 0;
		d2 = 0;
		N_iter++;
	}
	//---------------------------------------------------
	printf("\nNumber of iterations(K):%d\n", N_iter);
	printf("Matrix Size: %d*%d\n", size, size);
	//--print block--
	printf("It's done!\n");
	//������ ������
	for (i = 0; i < size; i++)
	{
		_mm_free(A[i]);
	}
	_mm_free(A);
	_mm_free(B);
	_mm_free(X);
	_mm_free(X_NEW);
	_mm_free(r);
	_mm_free(r_new);
	_mm_free(h);
	_mm_free(h_new);
	_mm_free(tmp);
	getchar();
	//---------------------------------
	return 0;
}