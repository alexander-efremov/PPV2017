#include <cstdio>
#include <fstream>
#include <cfloat>
#include <cmath>
#include <iostream>
#include "timer.h"

double** read_matrix(int n, char* filepath);

double* read_vector(int n, char* filepath);

int main(int argc, char* argv[])
{
    if (argc == 2)
    {
        return 0;
    }

    const int n = atoi(argv[1]);
    char filepath[256];
    strcpy(filepath, argv[2]);
    int getch = 0;
    if (argc > 3)
    {
        getch = atoi(argv[3]);
    }

    printf("Matrix size: %d*%d\n", n, n);

    double** a = read_matrix(n, filepath);
    double* b = read_vector(n, filepath);
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

double* read_vector(int n, char* filepath)
{
    double* b = static_cast<double *>(_mm_malloc(n * sizeof(double), 64));
    char filename[300];
    strcpy(filename, filepath);
    std::ifstream f;
    switch (n)
    {
    case 1024:
        strcat(filename, "/m_1024.dat");
        f.open(filename);
        break;
    case 2048:
        strcat(filename, "/m_2048.dat");
        f.open(filename);
        break;
    case 4096:
        strcat(filename, "/m_4096.dat");
        f.open(filename);
        break;
    case 8192:
        strcat(filename, "/m_8192.dat");
        f.open(filename);
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

double** read_matrix(int n, char* filepath)
{
    double** a = static_cast<double **>(_mm_malloc(n * sizeof(double *), 64));
    for (int i = 0; i < n; ++i)
    {
        a[i] = static_cast<double *>(_mm_malloc(n * sizeof(double), 64));
    }
    char filename[300];
    strcpy(filename, filepath);
    std::ifstream f;
    switch (n)
    {
    case 1024:
        strcat(filename, "/v_1024.dat");
        f.open(filename);
        break;
    case 2048:
        strcat(filename, "/v_2048.dat");
        f.open(filename);
        break;
    case 4096:
        strcat(filename, "/v_4096.dat");
        f.open(filename);
        break;
    case 8192:
        strcat(filename, "/v_8192.dat");
        f.open(filename);
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
