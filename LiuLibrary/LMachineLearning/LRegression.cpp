
#include "LRegression.h"

#include <cmath>

namespace Regression
{
    /// @brief 样本矩阵中增加常数项, 添加在最后一列, 值为1.0f
    /// @param[in] sampleMatrix 样本矩阵
    /// @param[in] newSampleMatrix 添加常数项后的样本矩阵
    void SampleAddConstantItem(IN const LRegressionMatrix& sampleMatrix, OUT LRegressionMatrix& newSampleMatrix)
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
}

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
    bool TrainModel(IN const LRegressionProblem& problem)
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
        if (problem.LearningRate <= 0.0f)
            return false;

        Regression::SampleAddConstantItem(problem.XMatrix, m_xMatrix);
        m_yVector = problem.YVector;

        // 初始化权重向量
        m_wVector.Reset(m_xMatrix.ColumnLen, 1, 0.0f);

        const LRegressionMatrix& X = m_xMatrix;
        const LRegressionMatrix& Y = m_yVector;
        LRegressionMatrix& W = m_wVector;
        float A = problem.LearningRate;

        LRegressionMatrix XT = X.T();

        LRegressionMatrix XW;
        LRegressionMatrix O;

        for (unsigned int i = 0; i < problem.TrainTimes; i++)
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
        Regression::SampleAddConstantItem(sampleMatrix, X);

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

bool LLinearRegression::TrainModel(IN const LRegressionProblem& problem)
{
    return m_pLinearRegression->TrainModel(problem);
}

bool LLinearRegression::Predict(IN const LRegressionMatrix& sampleMatrix, OUT LRegressionMatrix& yVector) const
{
    return m_pLinearRegression->Predict(sampleMatrix, yVector);
}

float LLinearRegression::GetCorrelation() const
{
    return m_pLinearRegression->GetCorrelation();
}

/// @brief 逻辑回归(分类)实现类
/// 逻辑函数为 h(x)  =  1/(1 + e^(X * W)) 
/// W为特征权重的列向量, X为特征的行向量
/// 原始问题中的目标向量中的值只能为0.0f或1.0f
/// P(1) = h(x), P(0) = 1-h(x)
class CLogisticRegression
{
public:
    /// @brief 构造函数
    CLogisticRegression()
    {

    }

    /// @brief 析构函数
    ~CLogisticRegression()
    {

    }

    /// @brief 训练模型
    bool TrainModel(IN const LRegressionProblem& problem)
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
        if (problem.LearningRate <= 0.0f)
            return false;

        for (unsigned int i = 0; i < problem.YVector.RowLen; i++)
        {
            if (problem.YVector[i][0] != REGRESSION_ONE &&
                problem.YVector[i][0] != REGRESSION_ZERO)
                return false;
        }

        Regression::SampleAddConstantItem(problem.XMatrix, m_xMatrix);

        m_yVector = problem.YVector;

        // 初始化权重向量
        m_wVector.Reset(m_xMatrix.ColumnLen, 1, 0.0f);

        const LRegressionMatrix& X = m_xMatrix;
        const LRegressionMatrix& Y = m_yVector;
        LRegressionMatrix& W = m_wVector;
        float A = problem.LearningRate;
        LRegressionMatrix XT = X.T();

        /*
        h(x)  =  1/(1 + e^(X * W)) 则
        wj = wj - α * ∑((y - h(x)) * xj)
        如果h(x)  =  1/(1 + e^(X * W)) 则
        wj = wj + α * ∑((y - h(x)) * xj)
        */

        LRegressionMatrix XW(X.RowLen, 1);
        LRegressionMatrix HX(X.RowLen, 1);
        LRegressionMatrix O;

        for (unsigned int i = 0; i < problem.TrainTimes; i++)
        {
            XW = X * W;
            for (unsigned int m = 0; m < XW.RowLen; m++)
            {
                this->Sigmoid(XW[m][0], HX[m][0]);
            }
            O = XT * (Y - HX);
            W = W - O.ScalarMul(A);
        }

        return true;
    }

    /// @brief 使用训练好的模型预测数据
    bool Predict(IN const LRegressionMatrix& sampleMatrix, OUT LRegressionMatrix& yVector) const
    {
        if (m_wVector.RowLen < 1)
            return false;

        if (sampleMatrix.ColumnLen != m_xMatrix.ColumnLen-1)
            return false;

        if (sampleMatrix.RowLen < 1)
            return false;

        LRegressionMatrix X;
        Regression::SampleAddConstantItem(sampleMatrix, X);

        yVector.Reset(X.RowLen, 1);

        LRegressionMatrix XW(X.RowLen, 1);
        XW = X * m_wVector;
        for (unsigned int m = 0; m < XW.RowLen; m++)
        {
            this->Sigmoid(XW[m][0], XW[m][0]);

            if (XW[m][0] >= 0.5f)
                yVector[m][0] = REGRESSION_ONE;
            else
                yVector[m][0] = REGRESSION_ZERO;
        }

        return true;

    }

    /// @brief 获取当前训练模型的正确率
    float GetAccuracy() const
    {
        if (m_wVector.RowLen < 1)
            return -1.0f;

        LRegressionMatrix XW(m_xMatrix.RowLen, 1);
        LRegressionMatrix HX(m_xMatrix.RowLen, 1);
        LRegressionMatrix Y(m_xMatrix.RowLen, 1);
        XW = m_xMatrix * m_wVector;

        unsigned int rightCount = 0;
        for (unsigned int m = 0; m < XW.RowLen; m++)
        {
            this->Sigmoid(XW[m][0], HX[m][0]);

            if (HX[m][0] >= 0.5f)
                Y[m][0] = REGRESSION_ONE;
            else
                Y[m][0] = REGRESSION_ZERO;

            if (Y[m][0] == m_yVector[m][0])
                rightCount++;
        }

        return rightCount * 1.0f/Y.RowLen;
    }

private:
    /// @brief S型函数
    /// @param[in] input 输入值
    /// @param[out] output 存储输出值
    void Sigmoid(float input, float& output) const
    {
        output = 1.0f/(1.0f + exp(input));
    }

private:
    LRegressionMatrix m_xMatrix; ///< 样本矩阵
    LRegressionMatrix m_yVector; ///< 目标向量(列向量)
    LRegressionMatrix m_wVector; ///<权重矩阵(列向量)
};

LLogisticRegression::LLogisticRegression()
    : m_pLogisticRegression(0)
{
    m_pLogisticRegression = new CLogisticRegression(); 
}

LLogisticRegression::~LLogisticRegression()
{
    if (m_pLogisticRegression != 0)
    {
        delete m_pLogisticRegression;
        m_pLogisticRegression = 0;
    }
}

bool LLogisticRegression::TrainModel(IN const LRegressionProblem& problem)
{
    return m_pLogisticRegression->TrainModel(problem);
}

bool LLogisticRegression::Predict(IN const LRegressionMatrix& sampleMatrix, OUT LRegressionMatrix& yVector) const
{
    return m_pLogisticRegression->Predict(sampleMatrix, yVector);
}

float LLogisticRegression::GetAccuracy() const
{
    return m_pLogisticRegression->GetAccuracy();
}