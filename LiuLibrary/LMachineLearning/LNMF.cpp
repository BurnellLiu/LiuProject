#include "LNMF.h"

#include "LLib.h"

#define LNMF_ERROR_NUM -1


LNMF::LNMF()
{
    m_bNeedInit = true;
    m_featureNum = LNMF_ERROR_NUM;
}

LNMF::~LNMF()
{

}

bool LNMF::SetFeatureNum(int num)
{
    if (num <= 0)
        return false;

    m_featureNum = num;
    m_bNeedInit = true;

    return true;
}

bool LNMF::SetPrimitiveMatrix(const LNMFMatrix& primitiveMatrix)
{
    m_primitiveMatrix = primitiveMatrix;
    m_bNeedInit = true;
    return true;
}

bool LNMF::Init()
{
    if (m_featureNum == LNMF_ERROR_NUM 
        || (m_primitiveMatrix.RowLen == 0 && m_primitiveMatrix.ColumnLen == 0))
        return false;

    LRandom::SRandTime();

    LNMFMatrix& X = m_primitiveMatrix;
    LNMFMatrix& W = m_weightedMatrix;
    LNMFMatrix& F = m_featureMatrix;

    // 随机生成权重矩阵
    W.Reset(X.RowLen, m_featureNum);
    RandMatrix(W);

    // 归一化权重矩阵
    //NomalMatrix(W);

    // 随机生成特征矩阵
    F.Reset(m_featureNum, X.ColumnLen);
    RandMatrix(F);

    m_bNeedInit = false;
    return true;
}

bool LNMF::FactoringOnce()
{
    if (m_bNeedInit)
        return false;

    LNMFMatrix& X = m_primitiveMatrix;
    LNMFMatrix& W = m_weightedMatrix;
    LNMFMatrix& F = m_featureMatrix;

    LNMFMatrix::T(W, TW);
    LNMFMatrix::MUL(TW, X, FN);
    LNMFMatrix::MUL(TW, W, TWW);
    LNMFMatrix::MUL(TWW, F, FD);

    LNMFMatrix::DOTMUL(F, FN, FFN);
    LNMFMatrix::DOTDIV(FFN, FD, F);


    LNMFMatrix::T(F, TF);
    LNMFMatrix::MUL(X, TF, WN);
    LNMFMatrix::MUL(W, F, WF);
    LNMFMatrix::MUL(WF, TF, WD);

    LNMFMatrix::DOTMUL(W, WN, WWN);
    LNMFMatrix::DOTDIV(WWN, WD, W);

    return true;
}

bool LNMF::GetWeightedMatrix(LNMFMatrix& weightedMatrix) const
{
    if (m_bNeedInit)
        return false;

    weightedMatrix = m_weightedMatrix;
    return true;
}

bool LNMF::GetFeatureMatrix(LNMFMatrix& featureMatrix) const
{
    if (m_bNeedInit)
        return false;

    featureMatrix = m_featureMatrix;
    return true;
}

/// @brief 随机构造矩阵中的值(0~10之间的浮点数)
/// @param[in] matrix
void LNMF::RandMatrix(LNMFMatrix& matrix)
{
    for (unsigned int i = 0; i < matrix.RowLen; i++)
    {
        for (unsigned int j = 0; j < matrix.ColumnLen; j++)
        {
            matrix.Data[i][j] = LRandom::RandFloat() * 10;
        }
    }
}

/// @brief 对矩阵列上的数据进行归一化
/// @param[in] matrix
void LNMF::NomalMatrixC(LNMFMatrix& matrix)
{
    for (unsigned int j = 0; j < matrix.ColumnLen; j++)
    {
        float columnSum = 0.0f;
        for (unsigned int i = 0; i < matrix.RowLen; i++)
        {
            columnSum += matrix.Data[i][j];
        }

        for (unsigned int i = 0; i < matrix.RowLen; i++)
        {
            matrix.Data[i][j] /= columnSum;
        }
    }
}