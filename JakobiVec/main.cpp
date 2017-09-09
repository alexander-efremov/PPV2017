#include <cstdio>
#include <fstream>
#include <cfloat>
#include <cmath>
#include <iostream>
#include "timer.h"

double** read_matrix(int n);

double* read_vector(int n);

int main(int argc, char* argv[])
{
    if (argc ==1)
    {
        return 0;
    }

    const int n = atoi(argv[1]);
    int getch = 0;
    if (argc > 2)
    {
        getch = atoi(argv[2]);
    }

    printf("Matrix Size: %d*%d\n", n, n);

    double** a = read_matrix(n);
    double* b = read_vector(n);

    double* x = static_cast<double *>(_mm_malloc(n * sizeof(double), 64));
    double* x_n = static_cast<double *>(_mm_malloc(n * sizeof(double), 64));

    double norm, diff;

    StartTimer();

#pragma simd
#pragma vector aligned
    for (int i = 0; i < n; ++i)
    {
        x[i] = b[i] / a[i][i];
    }

    int it = 0;
    do
    {
#pragma simd
#pragma vector aligned
        for (int i = 0; i < n; ++i)
        {
            x_n[i] = 0.0;
        }
        
        norm = DBL_MIN;

        for (int i = 0; i < n; ++i)
        {
            x_n[i] = b[i];
#pragma simd
#pragma vector aligned
            for (int j = 0; j < n; ++j)
            {
                x_n[i] -= a[i][j] * x[j];
            }
            x_n[i] += a[i][i] * x[i];
            x_n[i] /= a[i][i];
        }

        for (int k = 0; k < n; ++k)
        {
            diff = fabs(x_n[k] - x[k]);
            if (diff > norm)
            {
                norm = diff;
            }
            x[k] = x_n[k];
        }
        memcpy(x, x_n, n * sizeof(double));
    }
    while (norm > 1e-8 && it++ < 10000);
    
    const double elapsed = GetTimer();

    printf("Number of iterations(K): %d\n", it);
    printf("Total time = %10.8f ms\n", elapsed);

    for (int i = 0; i < n; ++i)
    {
        _mm_free(a[i]);
    }
    _mm_free(a);
    _mm_free(b);
    _mm_free(x);
    _mm_free(x_n);

    if (getch)
    {
        getchar();
    }
    return 0;
}

double* read_vector(int n)
{
    double* b = static_cast<double *>(_mm_malloc(n * sizeof(double), 64));
    std::ifstream f;
    switch (n)
    {
    case 1024:
        f.open("C:/Users/HOME/Source/Repos/Jakobi/data/v_1024.dat");
        break;
    case 2048:
        f.open("C:/Users/HOME/Source/Repos/Jakobi/data/v_2048.dat");
        break;
    case 4096:
        f.open("C:/Users/HOME/Source/Repos/Jakobi/data/v_4096.dat");
        break;
    case 8192:
        f.open("C:/Users/HOME/Source/Repos/Jakobi/data/v_8192.dat");
        break;
    default:
        f.open("default.dat");
        break;
    }

    for (int i = 0; i < n; i++)
    {
        f >> b[i];
    }

    f.close();

    return b;
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
