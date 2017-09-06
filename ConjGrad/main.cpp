#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fstream>
#include <iostream>
#include "timer.h"

double** read_matrix(int n);

void zero_vector(double* vector, int n);

double scalar_mult(double* b1, double* b2, int n);

int main(int argc, char* argv[])
{
	const int n = atoi(argv[1]);
	printf("n = %d\n", n);

	double alpha, betta, d1;

	double** a = read_matrix(n);
	double* b = static_cast<double *>(malloc(n * sizeof(double)));
	for (int i = 0; i < n; ++i)
	{
		b[i] = 1.0;
	}
	double* x = static_cast<double*>(malloc(n * sizeof(double)));
	double* x_n = static_cast<double*>(malloc(n * sizeof(double)));
	double* r = static_cast<double*>(malloc(n * sizeof(double)));
	double* r_n = static_cast<double*>(malloc(n * sizeof(double)));
	double* h = static_cast<double*>(malloc(n * sizeof(double)));
	double* h_n = static_cast<double*>(malloc(n * sizeof(double)));
	double* tmp = static_cast<double*>(malloc(n * sizeof(double)));

	zero_vector(x, n);
	zero_vector(x_n, n);
	zero_vector(r, n);
	zero_vector(r_n, n);
	zero_vector(h, n);
	zero_vector(h_n, n);
	zero_vector(tmp, n);

	StartTimer();

	// 1. Set x0
	for (int i = 0; i < n; i++)
	{
		x[i] = b[i] / a[i][i];
	}
	
	// 2. Calculate residual vector r0 = b - A * x0
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			tmp[i] += a[i][j] * x[j];
		}
		r[i] = b[i] - tmp[i];
	}
	
	// 3. Calulate direction vector h0 = r0
	memcpy(h, r, n * sizeof(double));

	double norm;
	int it = -1;
	do
	{
		norm = 0.0;
		zero_vector(tmp, n);

		//1 Вычисление alpha
		d1 = scalar_mult(r, r, n);

		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < n; j++)
			{
				tmp[i] += a[i][j] * h[j];
			}
		}

		alpha = d1 / scalar_mult(tmp, h, n);

		//2 Вычисление r_n
		for (int i = 0; i < n; i++)
		{
			r_n[i] = r[i] - alpha * tmp[i];
		}

		//3 Вычисление betta
		betta = scalar_mult(r_n, r_n, n) / d1;

		//4 Вычисление h_n
		for (int i = 0; i < n; i++)
		{
			h_n[i] = r_n[i] + betta * h[i];
		}

		//5 Вычисление x_n
		for (int i = 0; i < n; i++)
		{
			x_n[i] = x[i] + alpha * h[i];
		}

		for (int i = 0; i < n; i++)
		{
			double diff = fabs(x_n[i] - x[i]);
			if (diff > norm)
			{
				norm = diff;
			}
		}

		memcpy(x, x_n, n * sizeof(double));
		memcpy(h, h_n, n * sizeof(double));
		memcpy(r, r_n, n * sizeof(double));
		zero_vector(x_n, n);
		zero_vector(r_n, n);
		zero_vector(h_n, n);
	}
	while (norm > 1e-8 && ++it < 10000);

	const double elapsed = GetTimer();

	printf("Matrix Size: %d*%d Number of iterations(K): %d\n", n, n, it);
	printf("Total time = %10.8f [ms]\n", elapsed);

	for (int i = 0; i < n; i++)
	{
		free(a[i]);
	}
	free(a);
	free(b);
	free(x);
	free(x_n);
	free(r);
	free(r_n);
	free(h);
	free(h_n);
	free(tmp);

	getchar();
	return 0;
}

double** read_matrix(int n)
{
	double** a = static_cast<double **>(_mm_malloc(n * sizeof(double *), 64));
	for (int i = 0; i < n; ++i)
	{
		a[i] = static_cast<double *>(_mm_malloc(n * sizeof(double), 64));
	}

	std::ifstream f;
	switch (n)
	{
	case 512:
		f.open("C:/Users/HOME/Source/Repos/Jakobi/data/m_512.dat");
		break;
	case 1024:
		f.open("C:/Users/HOME/Source/Repos/Jakobi/data/m_1024.dat");
		break;
	case 2048:
		f.open("C:/Users/HOME/Source/Repos/Jakobi/data/m_2048.dat");
		break;
	case 4096:
		f.open("C:/Users/HOME/Source/Repos/Jakobi/data/m_4096.dat");
		break;
	case 8192:
		f.open("C:/Users/HOME/Source/Repos/Jakobi/data/m_8192.dat");
		break;
	default:
		f.open("default.dat");
		break;
	}

	if (!f.is_open())
	{
		printf("matrix file not found.");
		return nullptr;
	}

	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			f >> a[i][j];
		}
	}

	f.close();

	return a;
}

void zero_vector(double* vector, int n)
{
	for (int i = 0; i < n; i++)
	{
		vector[i] = 0.0;
	}
}

double scalar_mult(double* b1, double* b2, int n)
{
	double res = 0;
	for (int i = 0; i < n; i++)
	{
		res += b1[i] * b2[i];
	}
	return res;
}
