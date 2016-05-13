
#include "LRegression.h"

#include <cmath>


/// @brief 线性回归实现类
/// 线性函数为 h(x)  =  X * W
/// W为特征权重的列向量, X为特征的行向量
class CLinearRegression
{
public:
    CLinearRegression()
    {

    }

    ~CLinearRegression()
    {

    }

    /// @brief 训练模型
    bool TrainModel(
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

        this->AddConstantItem(problem.XMatrix, m_xMatrix);
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

    /// @brief 使用训练好的模型预测数据
    bool Predict(IN const LRegressionMatrix& sampleMatrix, OUT LRegressionMatrix& yVector) const
    {
        // 模型未训练
        if (m_wVector.RowLen < 1)
            return false;

        if (sampleMatrix.ColumnLen != m_xMatrix.ColumnLen-1)
            return false;

        if (sampleMatrix.RowLen < 1)
            return false;

        LRegressionMatrix X;
        this->AddConstantItem(sampleMatrix, X);

        yVector = X * m_wVector;

        return true;
    }

    /// @brief 训练后预测数据与训练数据的相关度(即模型的优劣程度)
    float GetCorrelation() const
    {
        // 模型未训练
        if (m_wVector.RowLen < 1)
            return 0.0f;

        LRegressionMatrix Y = m_xMatrix * m_wVector;

        unsigned int length = Y.RowLen;

        float sumA = 0.0f;
        float sumB = 0.0f;
        float sqrSumA = 0.0f;
        float sqrSumB = 0.0f;
        float proSum = 0.0f;
        for (unsigned int i = 0; i < length; i++)
        {
            const float& a = m_yVector[i][0];
            const float& b = Y[i][0];

            sumA += a;
            sumB += b;

            sqrSumA += a * a;
            sqrSumB += b * b;

            proSum += a * b;
        }

        // 计算皮尔逊相关系数
        float num = proSum - (sumA * sumB/length);
        float den = sqrtf((sqrSumA - sumA * sumA/length) * (sqrSumB - sumB * sumB/length));

        if (den == 0)
            return 0;

        return num/den;
    }

private:
    /// @brief 样本矩阵中增加常数项, 添加在最后一列, 值为1.0f
    /// @param[in] sampleMatrix 样本矩阵
    /// @param[in] newSampleMatrix 添加常数项后的样本矩阵
    void AddConstantItem(IN const LRegressionMatrix& sampleMatrix, OUT LRegressionMatrix& newSampleMatrix) const
    {
        // 每个样本中最后一项增加常数项的特征值:1.0
        newSampleMatrix.Reset(sampleMatrix.RowLen, sampleMatrix.ColumnLen + 1);
        for (unsigned int row = 0; row < sampleMatrix.RowLen; row++)
        {
            for (unsigned int col = 0; col < sampleMatrix.ColumnLen; col++)
            {
                newSampleMatrix[row][col] = sampleMatrix[row][col];
            }
            newSampleMatrix[row][sampleMatrix.ColumnLen] = 1.0f; 
        }
    }


private:
    LRegressionMatrix m_xMatrix; ///< 样本矩阵
    LRegressionMatrix m_yVector; ///< 目标向量(列向量)
    LRegressionMatrix m_wVector; ///<权重矩阵(列向量)
};

LLinearRegression::LLinearRegression()
    : m_pLinearRegression(0)
{
    m_pLinearRegression = new CLinearRegression();
}

LLinearRegression::~LLinearRegression()
{
    if (m_pLinearRegression != 0)
    {
        delete m_pLinearRegression;
        m_pLinearRegression = 0;
    }
}

bool LLinearRegression::TrainModel(
    IN const LRegressionProblem& problem, 
    IN float learningRate, 
    IN unsigned int trainTimes)
{
   return m_pLinearRegression->TrainModel(problem, learningRate, trainTimes);
}

bool LLinearRegression::Predict(IN const LRegressionMatrix& sampleMatrix, OUT LRegressionMatrix& yVector) const
{
    return m_pLinearRegression->Predict(sampleMatrix, yVector);
}

float LLinearRegression::GetCorrelation() const
{
    return m_pLinearRegression->GetCorrelation();
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