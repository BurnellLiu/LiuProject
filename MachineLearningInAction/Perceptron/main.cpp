

#include <cstdio>

#include <Windows.h>

#include "LMachineLearning/LPerceptron.h"


int main()
{
    LPerceptronMatrix X(3, 2);
    X[0][0] = 3.0f;
    X[0][1] = 3.0f;

    X[1][0] = 4.0f;
    X[1][1] = 3.0f;

    X[2][0] = 1.0f;
    X[2][1] = 1.0f;

    LPerceptronMatrix Y(3, 1);
    Y[0][0] = LPERCEPTRON_SUN;
    Y[1][0] = LPERCEPTRON_SUN;
    Y[2][0] = LPERCEPTRON_MOON;

    LPerceptronProblem problem(X, Y);

    LPerceptron perceptron;
    perceptron.TrainModel(problem);

    LPerceptronMatrix sample(1, 2);
    sample[0][0] = 2.0f;
    sample[0][1] = 2.0f;

    printf("Predict: %f\n", perceptron.Predict(sample));

    system("pause");
    return 0;
}