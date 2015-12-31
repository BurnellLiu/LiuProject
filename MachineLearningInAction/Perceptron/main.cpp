

#include <cstdio>
#include <cstdlib>

#include <Windows.h>

#include "LMachineLearning/LPerceptron.h"

/// @brief �����������
/// @param[in] min �����������Сֵ
/// @param[in] max ������������ֵ
/// @return �������
static int RandInt(int min, int max) 
{
    return rand()%(max - min + 1) + min;
}


int main()
{
    const unsigned int SAMPLE_NUM = 1000;

    // ����ѵ�����������������
    LPerceptronMatrix X(SAMPLE_NUM, 2);
    LPerceptronMatrix Y(SAMPLE_NUM, 1);
    for (unsigned int i = 0; i < SAMPLE_NUM; i++)
    {
        X[i][0] = (float)RandInt(-1000, 1000);
        X[i][1] = (float)RandInt(-1000, 1000);
        Y[i][0] = ((X[i][0] + X[i][1]) >= 0)? LPERCEPTRON_SUN : LPERCEPTRON_MOON;
    }

    LPerceptronProblem problem(X, Y);

    // ѵ��ģ��
    LPerceptron perceptron;
    perceptron.TrainModel(problem);

    // ʹ�ò�����������
    LPerceptronMatrix testSample(1, 2);
    testSample[0][0] = -50.0f;
    testSample[0][1] = 0.0f;

    printf("Predict: %f\n", perceptron.Predict(testSample));

    system("pause");
    return 0;
}