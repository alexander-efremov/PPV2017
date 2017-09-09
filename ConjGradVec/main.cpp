#include <cstdio>
#include <fstream>
#include <cfloat>
#include <cmath>
#include <iostream>
#include "timer.h"

double** read_matrix(int n, char* filepath);

double* read_vector(int n, char* filepath);

double scalar_mult(double* b1, double* b2, int n);

int main(int argc, char* argv[])
{
    if (argc <= 2)
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

    double alpha, beta, d1, d2;

    double** a = read_matrix(n, filepath);
    double* b = read_vector(n, filepath);
    double* x = static_cast<double*>(_mm_malloc(n * sizeof(double), 64));
    double* x_n = static_cast<double*>(_mm_malloc(n * sizeof(double), 64));
    double* r = static_cast<double*>(_mm_malloc(n * sizeof(double), 64));
    double* r_n = static_cast<double*>(_mm_malloc(n * sizeof(double), 64));
    double* h = static_cast<double*>(_mm_malloc(n * sizeof(double), 64));
    double* h_n = static_cast<double*>(_mm_malloc(n * sizeof(double), 64));
    double* tmp = static_cast<double*>(_mm_malloc(n * sizeof(double), 64));

    StartTimer();

    // 1. Set x0
#pragma simd
    for (int i = 0; i < n; ++i)
    {
        x[i] = b[i] / a[i][i];
    }

    // 2. Calculate residual vector r0 = b - A * x0
    for (int i = 0; i < n; ++i)
    {
#pragma simd
        for (int j = 0; j < n; ++j)
        {
            tmp[i] += a[i][j] * x[j];
        }
    }

    // r0 (r0 = b - A*x0)
#pragma simd    
    for (int i = 0; i < n; ++i)
    {
        r[i] = b[i] - tmp[i];
    }

    // 3. Calculate direction vector h0 = r0
    memcpy(h, r, n * sizeof(double));

    double norm;
    int it = 0;
    do
    {
        norm = DBL_MIN;

        for (int i = 0; i < n; ++i)
        {
            tmp[i] = 0.0;
        }

        // 1 Calculate alpha
        d1 = scalar_mult(r, r, n);

        for (int i = 0; i < n; ++i)
        {
#pragma simd
            for (int j = 0; j < n; ++j)
            {
                tmp[i] += a[i][j] * h[j];
            }
        }

        d2 = scalar_mult(tmp, h, n);
        alpha = d1 / d2;

        // 2 Calculate r_n
#pragma simd
        for (int i = 0; i < n; ++i)
        {
            r_n[i] = r[i] - alpha * tmp[i];
        }

        // 3 Calculate beta
        d2 = scalar_mult(r_n, r_n, n);
        beta = d2 / d1;

        // 4 Calculate h_n
#pragma simd
        for (int i = 0; i < n; ++i)
        {
            h_n[i] = r_n[i] + beta * h[i];
        }

        // 5 Calculate x_n
#pragma simd
        for (int i = 0; i < n; ++i)
        {
            x_n[i] = x[i] + alpha * h[i];
        }

        for (int i = 0; i < n; ++i)
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
    } while (norm > 1e-8 && it++ < 10000);

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
    _mm_free(r);
    _mm_free(r_n);
    _mm_free(h);
    _mm_free(h_n);
    _mm_free(tmp);

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

    for (int i = 0; i < n; ++i)
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

double scalar_mult(double *__restrict b1, double *__restrict b2, int n)
{
    double res = 0.0;
#pragma simd
#pragma vector aligned
    for (int i = 0; i < n; ++i)
    {
        res += b1[i] * b2[i];
    }
    return res;
}