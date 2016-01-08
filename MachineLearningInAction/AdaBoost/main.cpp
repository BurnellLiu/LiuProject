
#include <cstdio>

#include <Windows.h>

#include "LMachineLearning/LBoost.h"

int main()
{
    /* 
    定义样本矩阵和标签向量
    以下定义的数据集, 只用一个单层决策树是无法处理的
    多个单层决策树组成提升树就可解决如下问题
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


    // 定义提升树, 使用样本集进行并且训练
    LBoostTree boostTree;
    boostTree.TrainModel(problem);

    // 计算训练错误率
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