

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

    // 检查参数 符不符合要求
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


    // 初始化权重向量和截距
    W.Reset(X.ColumnLen, 1);
    for (unsigned int i =0; i < W.RowLen; i++)
    {
        W[i][0] = 0.0f;
    }
    B = 0.0f;


    bool bErrorClass = false; // 标记是否存在错误分类
    while (true)
    {
        // 检验每一个训练样本查看是否被错误分类
        for (unsigned int i = 0; i < X.RowLen; i++)
        {
            float WXi = 0.0f;
            for (unsigned int n = 0; n < W.RowLen; n++)
            {
                WXi += W[n][0] * X[i][n];
            }

            // 误分类点
            if (Y[i][0] * (WXi + B) <= 0)
            {
                bErrorClass = true;

                // 更新W和B
                for (unsigned int n = 0; n < W.RowLen; n++)
                {
                    W[n][0] = W[n][0] + Alpha * Y[i][0] * X[i][n];
                }
                B = B + Alpha * Y[i][0];
            }

        }

        // 如果没有错误分类则退出循环
        if (!bErrorClass)
        {
            break;
        }

        // 如果有错误分类则继续
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