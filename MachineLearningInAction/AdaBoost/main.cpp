
#include <cstdio>

#include <Windows.h>

#include "LMachineLearning/LBoost.h"

int main()
{
    /* 
    ������������ͱ�ǩ����
    ���¶�������ݼ�, ֻ��һ��������������޷������
    ����������������������Ϳɽ����������
    */
    float sampleList[5 * 2] =
    {
        1.0f, 2.0f,
        2.0f, 1.0f,
        2.0f, 0.0f,
        1.5f, 0.5f,
        1.0f, 1.0f
    };

    float labelList[5 * 1] = 
    {
        LBOOST_MOON,
        LBOOST_MOON,
        LBOOST_MOON,
        LBOOST_SUN,
        LBOOST_SUN
    };


    LBoostMatrix sampleMatrix(5, 2, sampleList);
    LBoostMatrix labelVector(5, 1, labelList);
    LBoostProblem problem(sampleMatrix, labelVector);


    // ����������, ʹ�����������в���ѵ��
    LBoostTree boostTree;
    boostTree.TrainModel(problem);

    // ����ѵ��������
    LBoostMatrix resultVector;
    boostTree.Predict(sampleMatrix, &resultVector);
    unsigned errorCount = 0;
    for (unsigned int i = 0; i < resultVector.RowLen; i++)
    {
        if (resultVector[i][0] != labelVector[i][0])
            errorCount++;
    }

    float errorRate = (float)errorCount/resultVector.RowLen;

    printf("Train Error Rate: %f\n", errorRate);
    system("pause");
    return 0;
}