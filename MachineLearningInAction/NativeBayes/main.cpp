

#include <cstdio>

#include <Windows.h>

#include "LMachineLearning/LBayesClassifier.h"

int main()
{
    // ������������
    // ÿһ�д���һ����
    // ��һ��Ϊ���(Ӣ��), �ڶ���Ϊ����(��), ��3��Ϊ����(Ӣ��)
    int dataList[24] = 
    {
        600, 180, 12,
        592, 190, 11,
        558, 170, 12,
        592, 165, 10,
        500, 100, 6,
        550, 150, 8,
        542, 130, 7,
        575, 150, 9
    };
    LBayesMatrix sampleMatrix(8, 3, dataList);

    // �����������������
    // 1: ��
    // 0: Ů
    int classList[8] = 
    {
        1,
        1,
        1,
        1,
        0,
        0,
        0,
        0
    };
    LBayesMatrix classVector(8, 1, classList);

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