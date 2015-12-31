

#include <cstdio>
#include <cstdlib>

#include <Windows.h>

#include "LMachineLearning/LPerceptron.h"

/// @brief 产生随机整数
/// @param[in] min 随机整数的最小值
/// @param[in] max 随机整数的最大值
/// @return 随机整数
static int RandInt(int min, int max) 
{
    return rand()%(max - min + 1) + min;
}


int main()
{
    const unsigned int SAMPLE_NUM = 1000;

    // 定义训练输入矩阵和输出矩阵
    LPerceptronMatrix X(SAMPLE_NUM, 2);
    LPerceptronMatrix Y(SAMPLE_NUM, 1);
    for (unsigned int i = 0; i < SAMPLE_NUM; i++)
    {
        X[i][0] = (float)RandInt(-1000, 1000);
        X[i][1] = (float)RandInt(-1000, 1000);
        Y[i][0] = ((X[i][0] + X[i][1]) >= 0)? LPERCEPTRON_SUN : LPERCEPTRON_MOON;
    }

    LPerceptronProblem problem(X, Y);

    // 训练模型
    LPerceptron perceptron;
    perceptron.TrainModel(problem);

    // 使用测试样本测试
    LPerceptronMatrix testSample(1, 2);
    testSample[0][0] = -50.0f;
    testSample[0][1] = 0.0f;

    printf("Predict: %f\n", perceptron.Predict(testSample));

    system("pause");
    return 0;
}