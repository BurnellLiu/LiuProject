

#include <cstdio>

#include <Windows.h>

#include "LMachineLearning/LBayesClassify.h"


int main()
{
    LBayesMatrix sampleSet(2, 2);
    sampleSet[0][0] = 1; sampleSet[0][1] = 1;
    sampleSet[1][0] = 1; sampleSet[1][1] = 0;
    LBayesMatrix classVector(2, 1);
    classVector[0][0] = 1;
    classVector[1][0] = 0;

    LBayesProblem problem(sampleSet, classVector, BAYES_FEATURE_NONE_DISCAETE);
    LBayesClassify classify;
    classify.TrainModel(problem);

    int classValue = 9;
    LBayesMatrix sample(1, 2);
    sample[0][0] = 0;
    sample[0][1] = 0;

    classify.Predict(sample, &classValue);
    printf("%d\n", classValue);
    system("pause");
    return 0;
}