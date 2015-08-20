

#include "LPerceptron.h"


LPerceptron::LPerceptron()
{
    m_learningRate = 1.0f;
}

LPerceptron::~LPerceptron()
{

}

bool LPerceptron::SetLearningRate(IN float rate)
{
    if (rate <= 0 || rate > 1.0f)
        return false;

    this->m_learningRate = rate;

    return true;
}

bool LPerceptron::TrainModel(IN const LPerceptronProblem& problem)
{
    const LPerceptronMatrix& X = problem.XMatrix;
    const LPerceptronMatrix& Y = problem.YVector;
    LPerceptronMatrix& W = this->m_weightVector;
    float& B = this->m_b;
    const float Alpha = this->m_learningRate;

    // ������ ��������Ҫ��
    if (X.ColumnLen < 1)
        return false;
    if (X.RowLen < 2)
        return false;
    if (Y.ColumnLen != 1)
        return false;
    if (X.RowLen != Y.RowLen)
        return false;

    for (unsigned int n = 0; n < Y.RowLen; n++)
    {
        if (Y[n][0] != LPERCEPTRON_SUN &&
            Y[n][0] != LPERCEPTRON_MOON)
            return false;
    }


    // ��ʼ��Ȩ�������ͽؾ�
    W.Reset(X.ColumnLen, 1);
    for (unsigned int i =0; i < W.RowLen; i++)
    {
        W[i][0] = 0.0f;
    }
    B = 0.0f;


    bool bErrorClass = false; // ����Ƿ���ڴ������
    while (true)
    {
        // ����ÿһ��ѵ�������鿴�Ƿ񱻴������
        for (unsigned int i = 0; i < X.RowLen; i++)
        {
            float WXi = 0.0f;
            for (unsigned int n = 0; n < W.RowLen; n++)
            {
                WXi += W[n][0] * X[i][n];
            }

            // ������
            if (Y[i][0] * (WXi + B) <= 0)
            {
                bErrorClass = true;

                // ����W��B
                for (unsigned int n = 0; n < W.RowLen; n++)
                {
                    W[n][0] = W[n][0] + Alpha * Y[i][0] * X[i][n];
                }
                B = B + Alpha * Y[i][0];
            }

        }

        // ���û�д���������˳�ѭ��
        if (!bErrorClass)
        {
            break;
        }

        // ����д�����������
        if (bErrorClass)
        {
            bErrorClass = false;
            continue;
        }
    }
    
    return true;

}

float LPerceptron::Predict(IN const LPerceptronMatrix& sample)
{
    if (sample.RowLen != 1)
        return 0.0f;

    if (this->m_weightVector.RowLen < 1)
        return 0.0f;

    if (this->m_weightVector.RowLen != sample.ColumnLen)
        return 0.0f;

    LPerceptronMatrix WX = sample * this->m_weightVector;

    if (WX[0][0] + this->m_b >= 0)
        return LPERCEPTRON_SUN;
    else
        return LPERCEPTRON_MOON;
}