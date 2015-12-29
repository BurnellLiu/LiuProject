

#include <cstdio>

#include <Windows.h>

#include "LMachineLearning/LPerceptron.h"


int main()
{
    // ����ѵ�����������������
    float inputList[8] = 
    {
        1.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 1.0f,
        0.0f, 0.0f
    };

    LPerceptronMatrix X(4, 2, inputList);

    LPerceptronMatrix Y(4, 1);
    Y[0][0] = LPERCEPTRON_SUN;
    Y[1][0] = LPERCEPTRON_SUN;
    Y[2][0] = LPERCEPTRON_SUN;
    Y[3][0] = LPERCEPTRON_MOON;

    LPerceptronProblem problem(X, Y);

    // ѵ��ģ��
    LPerceptron perceptron;
    perceptron.TrainModel(problem);

    // ʹ�ò�����������
    LPerceptronMatrix testSample(1, 2);
    testSample[0][0] = 0.0f;
    testSample[0][1] = 0.0f;

    printf("Predict: %f\n", perceptron.Predict(testSample));

    system("pause");
    return 0;
}