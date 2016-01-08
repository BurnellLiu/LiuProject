
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
        for (unsigned int col = 0; col < m_xMatrix.ColumnLen-1; col++)
        {
            m_xMatrix[row][col] = problem.XMatrix[row][col];
        }
        m_xMatrix[row][m_xMatrix.ColumnLen-1] = 1.0f; 
    }

    m_yVector = problem.YVector;

    // 初始化权重向量
    m_wVector.Reset(m_xMatrix.ColumnLen, 1, 0.0f);
  
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

LLogisticRegression::LLogisticRegression()
{

}

LLogisticRegression::~LLogisticRegression()
{

}

bool LLogisticRegression::TrainModel(IN const LRegressionProblem& problem, IN float learningRate, IN unsigned int trainTimes)
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

    for (unsigned int i = 0; i < problem.YVector.RowLen; i++)
    {
        if (problem.YVector[i][0] != REGRESSION_ONE &&
            problem.YVector[i][0] != REGRESSION_ZERO)
            return false;
    }

    // 每个样本中最后一项增加常数项的特征值:1.0
    m_xMatrix.Reset(problem.XMatrix.RowLen, problem.XMatrix.ColumnLen + 1);
    for (unsigned int row = 0; row < m_xMatrix.RowLen; row++)
    {
        for (unsigned int col = 0; col < m_xMatrix.ColumnLen-1; col++)
        {
            m_xMatrix[row][col] = problem.XMatrix[row][col];
        }
        m_xMatrix[row][m_xMatrix.ColumnLen-1] = 1.0f; 
    }

    m_yVector = problem.YVector;

    // 初始化权重向量
    m_wVector.Reset(m_xMatrix.ColumnLen, 1, 0.0f);

    const LRegressionMatrix& X = m_xMatrix;
    const LRegressionMatrix& Y = m_yVector;
    LRegressionMatrix& W = m_wVector;
    float A = learningRate;
    LRegressionMatrix XT = X.T();

    /*
    h(x)  =  1/(1 + e^(X * W)) 则
    wj = wj - α * ∑((y - h(x)) * xj)
    如果h(x)  =  1/(1 + e^(-1 * X * W)) 则
    wj = wj + α * ∑((y - h(x)) * xj)
    */

    LRegressionMatrix XW(X.RowLen, 1);
    LRegressionMatrix HX(X.RowLen, 1);
    LRegressionMatrix O;

    for (unsigned int i = 0; i < trainTimes; i++)
    {
        XW = X * W;
        for (unsigned int m = 0; m < XW.RowLen; m++)
        {
            HX[m][0] = 1.0f/(1.0f + exp(XW[m][0]));
        }
        O = XT * (Y - HX);
        W = W - O.ScalarMul(A);
    }

    return true;
}

bool LLogisticRegression::GetWeightVector(OUT LRegressionMatrix& weightVector)
{
    if (m_wVector.RowLen < 1)
        return false;

    weightVector = m_wVector;

    return true;
}

float LLogisticRegression::GetLikelihood()
{
    if (m_wVector.RowLen < 1)
        return -1.0f;

    float like = 1.0f;

    LRegressionMatrix XW(m_xMatrix.RowLen, 1);
    LRegressionMatrix HX(m_xMatrix.RowLen, 1);
    XW = m_xMatrix * m_wVector;
    for (unsigned int m = 0; m < XW.RowLen; m++)
    {
        HX[m][0] = 1.0f/(1.0f + exp(XW[m][0]));
        if (m_yVector[m][0] == REGRESSION_ZERO)
        {
            HX[m][0] = 1 - HX[m][0];
        }

        like = like * HX[m][0];
    }

    return like;

}