
#include "LDataStruct/LMatrix.h"
#include "LMachineLearning/LRegression.h"

#include <cstdio>
#include <Windows.h>

int main()
{
    LMatrix<float> x(3, 1);
    x[0][0] = 1.0f;
    x[1][0] = 2.0f;
    x[2][0] = 3.0f;

    LMatrix<float> y(3, 1);
    y[0][0] = 1.0f;
    y[1][0] = 0.0f;
    y[2][0] = 0.0f;

    LRegressionProblem problem(x, y);
    LLogisticRegression logistic;
    logistic.TrainModel(problem, 1.0f, 500);
    printf("%f\n",logistic.GetLikelihood());
    LRegressionMatrix weight;
    logistic.GetWeightVector(weight);
    for (unsigned int i = 0; i < weight.RowLen; i++)
    {
        printf("%f  ", weight[i][0]);
    }
    printf("\n");

    system("pause");

    return 0;
}