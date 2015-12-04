

#include <cstdio>

#include <Windows.h>

#include "LMachineLearning/LBayesClassify.h"


int main()
{
    LBayesMatrix sampleSet(8, 3);
    sampleSet[0][0] = 600; sampleSet[0][1] = 180; sampleSet[0][2] = 12;
    sampleSet[1][0] = 592; sampleSet[1][1] = 190; sampleSet[1][2] = 11;
    sampleSet[2][0] = 558; sampleSet[2][1] = 170; sampleSet[2][2] = 12;
    sampleSet[3][0] = 592; sampleSet[3][1] = 165; sampleSet[3][2] = 10;
    sampleSet[4][0] = 500; sampleSet[4][1] = 100; sampleSet[4][2] = 6;
    sampleSet[5][0] = 550; sampleSet[5][1] = 150; sampleSet[5][2] = 8;
    sampleSet[6][0] = 542; sampleSet[6][1] = 130; sampleSet[6][2] = 7;
    sampleSet[7][0] = 575; sampleSet[7][1] = 150; sampleSet[7][2] = 9;
    LBayesMatrix classVector(8, 1);
    classVector[0][0] = 1;
    classVector[1][0] = 1;
    classVector[2][0] = 1;
    classVector[3][0] = 1;
    classVector[4][0] = 0;
    classVector[5][0] = 0;
    classVector[6][0] = 0;
    classVector[7][0] = 0;

    LBayesProblem problem(sampleSet, classVector, BAYES_FEATURE_NONE_DISCRETE);
    LBayesClassify classify;
    classify.TrainModel(problem);

    int classValue = 9;
    LBayesMatrix sample(1, 3);
    sample[0][0] = 600; sample[0][1] = 130; sample[0][2] = 8;
    

    classify.Predict(sample, &classValue);
    printf("%d\n", classValue);
    system("pause");
    return 0;
}