

#include <cstdio>

#include <Windows.h>

#include "LMachineLearning/LBayesClassifier.h"


int main()
{
    // ������������
    // ÿһ�д���һ����
    // ��һ��Ϊ���(Ӣ��), �ڶ���Ϊ����(��), ��3��Ϊ����(Ӣ��)
    LBayesMatrix sampleMatrix(8, 3);
    sampleMatrix[0][0] = 600; sampleMatrix[0][1] = 180; sampleMatrix[0][2] = 12;
    sampleMatrix[1][0] = 592; sampleMatrix[1][1] = 190; sampleMatrix[1][2] = 11;
    sampleMatrix[2][0] = 558; sampleMatrix[2][1] = 170; sampleMatrix[2][2] = 12;
    sampleMatrix[3][0] = 592; sampleMatrix[3][1] = 165; sampleMatrix[3][2] = 10;
    sampleMatrix[4][0] = 500; sampleMatrix[4][1] = 100; sampleMatrix[4][2] = 6;
    sampleMatrix[5][0] = 550; sampleMatrix[5][1] = 150; sampleMatrix[5][2] = 8;
    sampleMatrix[6][0] = 542; sampleMatrix[6][1] = 130; sampleMatrix[6][2] = 7;
    sampleMatrix[7][0] = 575; sampleMatrix[7][1] = 150; sampleMatrix[7][2] = 9;

    // �����������������
    // 1: ��
    // 0: Ů
    LBayesMatrix classVector(8, 1);
    classVector[0][0] = 1;
    classVector[1][0] = 1;
    classVector[2][0] = 1;
    classVector[3][0] = 1;
    classVector[4][0] = 0;
    classVector[5][0] = 0;
    classVector[6][0] = 0;
    classVector[7][0] = 0;

    // ���屴Ҷ˹ԭʼ����
    LBayesProblem problem(sampleMatrix, classVector, BAYES_FEATURE_CONTINUS);

    // ���屴Ҷ˹������, ����ѵ��
    LBayesClassifier classifier;
    classifier.TrainModel(problem);

    
    // ����������, ��Ԥ�������������
    LBayesMatrix newSample(1, 3);
    newSample[0][0] = 600; newSample[0][1] = 130; newSample[0][2] = 8;
    
    int predictValue = -1; 
    classifier.Predict(newSample, &predictValue);

    printf("%d\n", predictValue);
    system("pause");
    return 0;
}