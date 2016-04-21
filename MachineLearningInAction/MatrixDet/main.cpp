

#include <cstdio>

#include <Windows.h>

#include "LDataStruct/LMatrix.h"

int main()
{

    LMatrix<int> matrix(2, 2, 0);
    matrix.Reset(4, 4, 3);
    const LMatrix<int>& b = matrix;
    printf("%d\n", b.Data[0][0]);

    system("pause");
    return 0;
}