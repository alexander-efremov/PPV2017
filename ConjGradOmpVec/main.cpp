#include <cstdio>
#include <fstream>
#include <cfloat>
#include <cmath>
#include <iostream>

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

    if (getch)
    {
        getchar();
    }
    return 0;
}