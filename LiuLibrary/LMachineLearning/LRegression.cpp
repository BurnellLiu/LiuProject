
#include "LRegression.h"

#include <cmath>


namespace Regression
{
    /// @brief �������������ӳ�����, ��������һ��, ֵΪ1.0f
    /// @param[in] sampleMatrix ��������
    /// @param[in] newSampleMatrix ��ӳ���������������
    void SampleAddConstantItem(IN const LRegressionMatrix& sampleMatrix, OUT LRegressionMatrix& newSampleMatrix)
    {
        // ÿ�����������һ�����ӳ����������ֵ:1.0
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

/// @brief ���Իع�ʵ����
/// ���Ժ���Ϊ h(x)  =  X * W
/// WΪ����Ȩ�ص�������, XΪ������������
class CLinearRegression
{
public:
    CLinearRegression()
    {

    }

    ~CLinearRegression()
    {

    }

    /// @brief ѵ��ģ��
    bool TrainModel(IN const LRegressionProblem& problem)
    {
        // ������
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

        // ��ʼ��Ȩ������
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

    /// @brief ʹ��ѵ���õ�ģ��Ԥ������
    bool Predict(IN const LRegressionMatrix& sampleMatrix, OUT LRegressionMatrix& yVector) const
    {
        // ģ��δѵ��
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

    /// @brief ѵ����Ԥ��������ѵ�����ݵ���ض�(��ģ�͵����ӳ̶�)
    float GetCorrelation() const
    {
        // ģ��δѵ��
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

        // ����Ƥ��ѷ���ϵ��
        float num = proSum - (sumA * sumB/length);
        float den = sqrtf((sqrSumA - sumA * sumA/length) * (sqrSumB - sumB * sumB/length));

        if (den == 0)
            return 0;

        return num/den;
    }

private:
    LRegressionMatrix m_xMatrix; ///< ��������
    LRegressionMatrix m_yVector; ///< Ŀ������(������)
    LRegressionMatrix m_wVector; ///<Ȩ�ؾ���(������)
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

/// @brief �߼��ع�(����)ʵ����
/// �߼�����Ϊ h(x)  =  1/(1 + e^(X * W)) 
/// WΪ����Ȩ�ص�������, XΪ������������
/// ԭʼ�����е�Ŀ�������е�ֵֻ��Ϊ0.0f��1.0f
/// P(1) = h(x), P(0) = 1-h(x)
class CLogisticRegression
{
public:
    /// @brief ���캯��
    CLogisticRegression()
    {

    }

    /// @brief ��������
    ~CLogisticRegression()
    {

    }

    /// @brief ѵ��ģ��
    bool TrainModel(IN const LRegressionProblem& problem)
    {
        // ������
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

        // ��ʼ��Ȩ������
        m_wVector.Reset(m_xMatrix.ColumnLen, 1, 0.0f);

        const LRegressionMatrix& X = m_xMatrix;
        const LRegressionMatrix& Y = m_yVector;
        LRegressionMatrix& W = m_wVector;
        float A = problem.LearningRate;
        LRegressionMatrix XT = X.T();

        /*
        ���h(x)  =  1/(1 + e^(X * W)) ��
        wj = wj - �� * ��((y - h(x)) * xj)
        ���h(x)  =  1/(1 + e^(-X * W)) ��
        wj = wj + �� * ��((y - h(x)) * xj)
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

    /// @brief ʹ��ѵ���õ�ģ��Ԥ������
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

    /// @brief ��ȡ��ǰѵ��ģ�͵���ȷ��
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
    /// @brief S�ͺ���
    /// @param[in] input ����ֵ
    /// @param[out] output �洢���ֵ
    void Sigmoid(float input, float& output) const
    {
        output = 1.0f/(1.0f + exp(input));
    }

private:
    LRegressionMatrix m_xMatrix; ///< ��������
    LRegressionMatrix m_yVector; ///< Ŀ������(������)
    LRegressionMatrix m_wVector; ///<Ȩ�ؾ���(������)
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

void PrintMatrix(const LMatrix<float>& matrix);

class CSoftmaxRegression
{
public:
    CSoftmaxRegression(IN unsigned int m, IN unsigned int n, IN unsigned int k)
    {
        m_M = m;
        m_N = n;
        m_K = k;

        m_wMatrix.Reset(n + 1, k, 0.0f);
    }

    ~CSoftmaxRegression()
    {

    }

    /// @brief ѵ��ģ��
    bool TrainModel(IN const LRegressionMatrix& xMatrix, IN const LRegressionMatrix& yMatrix, IN float alpha)
    {
        // ������
        if (m_M < 2 || m_N < 1 || m_K < 2)
            return false;

        if (xMatrix.RowLen < 1)
            return false;
        if (xMatrix.ColumnLen != m_N)
            return false;
        if (yMatrix.RowLen != xMatrix.RowLen)
            return false;
        if (yMatrix.ColumnLen != m_K)
            return false;

        if (alpha <= 0.0f)
            return false;

        // ���ӳ���������������
        LRegressionMatrix X;
        Regression::SampleAddConstantItem(xMatrix, X);

        // Ȩ�ؾ���
        LRegressionMatrix& W = m_wMatrix;

        // ���ʾ���
        LRegressionMatrix P(X.RowLen, m_K, 0.0f);

        // ������ʾ���
        this->SampleProbK(X, W, P);

        LRegressionMatrix::SUB(yMatrix, P, P);

        // Ȩ������(������)
        LRegressionMatrix dwVec(m_N + 1, 1, 0.0f);
        for (unsigned int k = 0; k < m_K; k++)
        {
            dwVec.Reset(m_N + 1, 1, 0.0f);
            for (unsigned int row = 0; row < X.RowLen; row++)
            {
                for (unsigned int col = 0; col < X.ColumnLen; col++)
                {
                    dwVec[col][0] += X[row][col] * P[row][k];
                }
            }

            LRegressionMatrix::SCALARDIV(dwVec, (float)m_M, dwVec);

            LRegressionMatrix::SCALARMUL(dwVec, alpha, dwVec);

            for (unsigned int row = 0; row < m_wMatrix.RowLen; row++)
            {
                m_wMatrix[row][k] += dwVec[row][0];
            }
        }

        return true;
    }

    /// @brief ʹ��ѵ���õ�ģ��Ԥ������
    bool Predict(IN const LRegressionMatrix& xMatrix, OUT LRegressionMatrix& yMatrix) const
    {
        // ������
        if (m_M < 2 || m_N < 1 || m_K < 2)
            return false;

        if (xMatrix.RowLen < 1)
            return false;
        if (xMatrix.ColumnLen != m_N)
            return false;

        yMatrix.Reset(xMatrix.RowLen, m_K, 0.0f);

        LRegressionMatrix X;
        Regression::SampleAddConstantItem(xMatrix, X);

        this->SampleProbK(X, m_wMatrix, yMatrix);

        return true;
    }

private:
    /// @brief ������������K������ĸ�������
    /// @param[in] sampleMatrix ��������, m * n
    /// @param[in] weightMatrix Ȩ�ؾ���, n * k, ÿһ��Ϊһ������Ȩ��
    /// @param[out] probMatrix ���ʾ���, �洢ÿ���������ڲ�ͬ����ĸ���
    /// @return �ɹ�����true, ʧ�ܷ���false(�������������»᷵��ʧ��)
    void SampleProbK(
        IN const LRegressionMatrix& sampleMatrix, 
        IN const LRegressionMatrix& weightMatrix, 
        OUT LRegressionMatrix& probMatrix) const
    {
        LRegressionMatrix::MUL(sampleMatrix, weightMatrix, probMatrix);

        for (unsigned int row = 0; row < probMatrix.RowLen; row++)
        {
            for (unsigned int col = 0; col < probMatrix.ColumnLen; col++)
            {
                probMatrix[row][col] = exp(probMatrix[row][col]);
            }
        }

        for (unsigned int row = 0; row < probMatrix.RowLen; row++)
        {
            float sum = 0.0f;
            for (unsigned int col = 0; col < probMatrix.ColumnLen; col++)
            {
                sum += probMatrix[row][col];
            }

            for (unsigned int col = 0; col < probMatrix.ColumnLen; col++)
            {
                probMatrix[row][col] = probMatrix[row][col]/sum;
            }
        }
    }
private:
    unsigned int m_M; ///< �����ܸ���
    unsigned int m_N; ///< ��������ֵ����
    unsigned int m_K; ///< ����������

    LRegressionMatrix m_wMatrix; ///<Ȩ�ؾ���, ÿһ����Ϊһ�������Ȩ������
};

LSoftmaxRegression::LSoftmaxRegression(IN unsigned int m, IN unsigned int n, IN unsigned int k)
    :m_pSoftmaxRegression(0)
{
    m_pSoftmaxRegression = new CSoftmaxRegression(m, n ,k);
}

LSoftmaxRegression::~LSoftmaxRegression()
{
    if (m_pSoftmaxRegression != 0)
    {
        delete m_pSoftmaxRegression;
        m_pSoftmaxRegression = 0;
    }
}


bool LSoftmaxRegression::TrainModel(IN const LRegressionMatrix& xMatrix, IN const LRegressionMatrix& yMatrix, IN float alpha)
{
    return m_pSoftmaxRegression->TrainModel(xMatrix, yMatrix, alpha);
}

bool LSoftmaxRegression::Predict(IN const LRegressionMatrix& xMatrix, OUT LRegressionMatrix& yMatrix) const
{
    return m_pSoftmaxRegression->Predict(xMatrix, yMatrix);
}
