#include <cstdio>
#include <fstream>
#include <cfloat>
#include <cmath>
#include <iostream>
#include "timer.h"

int main(int argc, char* argv[])
{
    if (argc <= 2)
    {
        return 0;
    }

    const int n = atoi(argv[1]);
    int getch = 0;
    if (argc > 3)
    {
        getch = atoi(argv[3]);
    }

    printf("Matrix size: %d*%d\n", n, n);

    if (getch)
    {
        getchar();
    }
    return 0;
}