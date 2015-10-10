
#include "LRegression.h"

#include <cmath>

LLinearRegression::LLinearRegression()
{

}

LLinearRegression::~LLinearRegression()
{

}

bool LLinearRegression::TrainModel(
    IN const LRegressionProblem& problem, 
    IN float learningRate, 
    IN unsigned int trainTimes)
{
    // 检查参数
    if (problem.XMatrix.RowLen < 2)
        return false;
    if (problem.XMatrix.ColumnLen < 1)
        return false;
    if (problem.YVector.ColumnLen != 1)
        return false;
    if (problem.YVector.RowLen != problem.XMatrix.RowLen)
        return false;
    if (learningRate <= 0.0f)
        return false;

    // 每个样本中最后一项增加常数项的特征值:1.0
    m_xMatrix.Reset(problem.XMatrix.RowLen, problem.XMatrix.ColumnLen + 1);
    for (unsigned int row = 0; row < m_xMatrix.RowLen; row++)
    {
        for (unsigned int col = 0; col < m_xMatrix.ColumnLen; col++)
        {
            m_xMatrix[row][col] = problem.XMatrix[row][col];
        }
        m_xMatrix[row][m_xMatrix.ColumnLen-1] = 1.0f;
    }

    m_yVector = problem.YVector;

    // 初始化权重向量
    m_wVector.Reset(m_xMatrix.ColumnLen, 1);
    for (unsigned int i = 0; i < m_wVector.RowLen; i++)
    {
        m_wVector[i][0] = 0.0f;
    }

    const LRegressionMatrix& X = m_xMatrix;
    const LRegressionMatrix& Y = m_yVector;
    LRegressionMatrix& W = m_wVector;
    float A = learningRate;

    LRegressionMatrix XT = X.T();

    LRegressionMatrix XW;
    LRegressionMatrix O;

    for (unsigned int i = 0; i < trainTimes; i++)
    {
        XW = X * W;
        O = XT * (XW - Y);
        W = W - O.ScalarMul(A);
    }

    return true;
}

bool LLinearRegression::GetWeightVector(OUT LRegressionMatrix& weightVector)
{
    if (m_wVector.RowLen < 1)
        return false;

    weightVector = m_wVector;

    return true;
}

float LLinearRegression::GetErrorValue()
{
    if (m_wVector.RowLen < 1)
        return -1.0f;

    LRegressionMatrix dif = m_xMatrix * m_wVector - m_yVector;
    LRegressionMatrix square = dif.T() * dif;
    float squareValue = square[0][0];
    squareValue = sqrt(squareValue);
    return squareValue;
}
