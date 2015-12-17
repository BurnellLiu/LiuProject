

#include "LMachineLearning/LNMF.h"

#include <cstdio>

#include <Windows.h>

void PrintMatrix(const LNMFMatrix& m)
{
    for (unsigned int i = 0; i < m.RowLen; i++)
    {
        for (unsigned int j = 0; j < m.ColumnLen; j++)
        {
            printf("%2f  ", m[i][j]);
        }
        printf("\n");
    }
}


int main()
{
    float dataList[4] = 
    {
        1.0f, 2.0f,
        3.0f, 4.0f
    };
    LNMFMatrix V(2, 2, dataList);

    LNMFProblem problem(V, 2, 50);

    LNMF nmf;

    LNMFMatrix W;
    LNMFMatrix H;
    LNMFMatrix N;

    nmf.Factoring(problem, &W, &H);
    N = W * H;
    PrintMatrix(N);
    printf("\n");


    system("pause");

    return 0;
}