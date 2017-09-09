#include <cstdio>
#include <fstream>
#include <cmath>
#include <iostream>

int main(int argc, char* argv[])
{
    if (argc <= 3)
    {
        return 0;
    }

    const int n = atoi(argv[1]);
    int getch = 0;
    if (argc > 4)
    {
        getch = atoi(argv[4]);
    }

    printf("Matrix size: %d*%d\n", n, n);

    if (getch)
    {
        getchar();
    }
    return 0;
}