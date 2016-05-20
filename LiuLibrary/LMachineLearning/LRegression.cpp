
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
    CLinearRegression(IN unsigned int m, IN unsigned int n)
    {
        m_M = m;
        m_N = n;

        m_wVector.Reset(n + 1, 1, 0.0f);
    }

    ~CLinearRegression()
    {

    }

    /// @brief ѵ��ģ��
    bool TrainModel(IN const LRegressionMatrix& xMatrix, IN const LRegressionMatrix& yVector, IN float alpha)
    {
        // ������
        if (m_M < 2 || m_N < 1)
            return false;

        if (xMatrix.RowLen < 1)
            return false;
        if (xMatrix.ColumnLen != m_N)
            return false;

        if (yVector.ColumnLen != 1)
            return false;
        if (yVector.RowLen != xMatrix.RowLen)
            return false;

        if (alpha <= 0.0f)
            return false;

        LRegressionMatrix X;
        Regression::SampleAddConstantItem(xMatrix, X);

        const LRegressionMatrix& Y = yVector;
        LRegressionMatrix& W = m_wVector;

        LRegressionMatrix XT = X.T();

        LRegressionMatrix XW;
        LRegressionMatrix DW;

        /*
        h(x) = X * W
        wj = wj - �� * ��((h(x)-y) * xj)
        */
        LRegressionMatrix::MUL(X, W, XW);
        LRegressionMatrix::SUB(XW, Y, XW);
        LRegressionMatrix::MUL(XT, XW, DW);
        LRegressionMatrix::SCALARMUL(DW, -1.0f * alpha, DW);
        LRegressionMatrix::ADD(W, DW, W);

        return true;
    }

    /// @brief ʹ��ѵ���õ�ģ��Ԥ������
    bool Predict(IN const LRegressionMatrix& xMatrix, OUT LRegressionMatrix& yVector) const
    {
        // ������
        if (m_M < 2 || m_N < 1)
            return false;

        if (xMatrix.RowLen < 1)
            return false;
        if (xMatrix.ColumnLen != m_N)
            return false;

        LRegressionMatrix X;
        Regression::SampleAddConstantItem(xMatrix, X);

        LRegressionMatrix::MUL(X, m_wVector, yVector);

        return true;
    }

private:
    unsigned int m_M; ///< �����ܸ���
    unsigned int m_N; ///< ��������ֵ����
    LRegressionMatrix m_wVector; ///<Ȩ�ؾ���(������)
};

LLinearRegression::LLinearRegression(IN unsigned int m, IN unsigned int n)
    : m_pLinearRegression(0)
{
    m_pLinearRegression = new CLinearRegression(m, n);
}

LLinearRegression::~LLinearRegression()
{
    if (m_pLinearRegression != 0)
    {
        delete m_pLinearRegression;
        m_pLinearRegression = 0;
    }
}

bool LLinearRegression::TrainModel(IN const LRegressionMatrix& xMatrix, IN const LRegressionMatrix& yVector, IN float alpha)
{
    return m_pLinearRegression->TrainModel(xMatrix, yVector, alpha);
}

bool LLinearRegression::Predict(IN const LRegressionMatrix& xMatrix, OUT LRegressionMatrix& yVector) const
{
    return m_pLinearRegression->Predict(xMatrix, yVector);
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
    CLogisticRegression(IN unsigned int m, IN unsigned int n)
    {
        m_M = m;
        m_N = n;
        m_wVector.Reset(n + 1, 1, 0.0f);
    }

    /// @brief ��������
    ~CLogisticRegression()
    {

    }

    /// @brief ѵ��ģ��
    bool TrainModel(IN const LRegressionMatrix& xMatrix, IN const LRegressionMatrix& yVector, IN float alpha)
    {
        // ������
        if (m_M < 2 || m_N < 1)
            return false;

        if (xMatrix.RowLen < 1)
            return false;
        if (xMatrix.ColumnLen != m_N)
            return false;

        if (yVector.ColumnLen != 1)
            return false;
        if (yVector.RowLen != xMatrix.RowLen)
            return false;

        if (alpha <= 0.0f)
            return false;

        for (unsigned int i = 0; i < yVector.RowLen; i++)
        {
            if (yVector[i][0] != REGRESSION_ONE &&
                yVector[i][0] != REGRESSION_ZERO)
                return false;
        }

        LRegressionMatrix X;
        Regression::SampleAddConstantItem(xMatrix, X);

        const LRegressionMatrix& Y = yVector;

        LRegressionMatrix& W = m_wVector;
        LRegressionMatrix XT = X.T();

        /*
        ���h(x)  =  1/(1 + e^(X * W)) ��
        wj = wj - �� * ��((y - h(x)) * xj)
        ���h(x)  =  1/(1 + e^(-X * W)) ��
        wj = wj + �� * ��((y - h(x)) * xj)
        */

        LRegressionMatrix XW(X.RowLen, 1);
        LRegressionMatrix DW;

        LRegressionMatrix::MUL(X, W, XW);
        for (unsigned int m = 0; m < XW.RowLen; m++)
        {
            this->Sigmoid(XW[m][0], XW[m][0]);
        }

        LRegressionMatrix::SUB(Y, XW, XW);
        LRegressionMatrix::MUL(XT, XW, DW);

        LRegressionMatrix::SCALARMUL(DW, -1.0f * alpha, DW);
        LRegressionMatrix::ADD(W, DW, W);

        return true;
    }

    /// @brief ʹ��ѵ���õ�ģ��Ԥ������
    bool Predict(IN const LRegressionMatrix& xMatrix, OUT LRegressionMatrix& yVector) const
    {
        // ������
        if (m_M < 2 || m_N < 1)
            return false;

        if (xMatrix.RowLen < 1)
            return false;
        if (xMatrix.ColumnLen != m_N)
            return false;

        LRegressionMatrix X;
        Regression::SampleAddConstantItem(xMatrix, X);

        yVector.Reset(X.RowLen, 1, 0.0f);
        LRegressionMatrix::MUL(X, m_wVector, yVector);

        for (unsigned int m = 0; m < yVector.RowLen; m++)
        {
            this->Sigmoid(yVector[m][0], yVector[m][0]);
        }

        return true;

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
    unsigned int m_M; ///< �����ܸ���
    unsigned int m_N; ///< ��������ֵ����
    LRegressionMatrix m_wVector; ///<Ȩ�ؾ���(������)
};

LLogisticRegression::LLogisticRegression(IN unsigned int m, IN unsigned int n)
    : m_pLogisticRegression(0)
{
    m_pLogisticRegression = new CLogisticRegression(m, n); 
}

LLogisticRegression::~LLogisticRegression()
{
    if (m_pLogisticRegression != 0)
    {
        delete m_pLogisticRegression;
        m_pLogisticRegression = 0;
    }
}

bool LLogisticRegression::TrainModel(IN const LRegressionMatrix& xMatrix, IN const LRegressionMatrix& yVector, IN float alpha)
{
    return m_pLogisticRegression->TrainModel(xMatrix, yVector, alpha);
}

bool LLogisticRegression::Predict(IN const LRegressionMatrix& xMatrix, OUT LRegressionMatrix& yVector) const
{
    return m_pLogisticRegression->Predict(xMatrix, yVector);
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
