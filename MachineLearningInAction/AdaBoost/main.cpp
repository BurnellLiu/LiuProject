
#include <cstdio>

#include <Windows.h>

#include "LMachineLearning/LBoost.h"

int main()
{
    LBoostMatrix sampleMatrix(5, 2);
    sampleMatrix[0][0] = 1.0f;
    sampleMatrix[0][1] = 2.1f;

    sampleMatrix[1][0] = 2.0f;
    sampleMatrix[1][1] = 1.1f;

    sampleMatrix[2][0] = 1.3f;
    sampleMatrix[2][1] = 1.0f;

    sampleMatrix[3][0] = 1.0f;
    sampleMatrix[3][1] = 1.0f;

    sampleMatrix[4][0] = 2.0f;
    sampleMatrix[4][1] = 1.0f;

    LBoostMatrix labelVector(5, 1);
    labelVector[0][0] = LBOOST_SUN;
    labelVector[1][0] = LBOOST_SUN;
    labelVector[2][0] = LBOOST_MOON;
    labelVector[3][0] = LBOOST_MOON;
    labelVector[4][0] = LBOOST_SUN;

    LBoostTree boostTree;
    LBoostProblem problem(sampleMatrix, labelVector);
    boostTree.TrainModel(problem);

    LBoostMatrix sample(1, 2);
    sample[0][0] = 0.0f;
    sample[0][1] = 0.0f;
    printf("%f\n", boostTree.Predict(sample));
    system("pause");
    return 0;
}