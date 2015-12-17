#include "LNMF.h"

#include "LLib.h"

#define LNMF_ERROR_NUM -1

/// @brief 将矩阵中的值全部填1.0f
static void OneNMFMatrix(INOUT LNMFMatrix& m)
{
    for (unsigned int i = 0; i < m.RowLen; i++)
    {
        for (unsigned int j = 0; j < m.ColumnLen; j++)
        {
            m.Data[i][j] = 1.0f;
        }
    }
}


LNMF::LNMF()
{

}

LNMF::~LNMF()
{

}

bool LNMF::Factoring(IN const LNMFProblem& problem, OUT LNMFMatrix* pW, OUT LNMFMatrix* pH)
{
    // 检查参数
    if (0 == pW || 0 == pH)
        return false;

    if (0 == problem.R || 0 == problem.IterCount)
        return false;

    if (0 == problem.V.RowLen || 0 == problem.V.ColumnLen)
    {
        return false;
    }

    for (unsigned int row = 0; row < problem.V.RowLen; row++)
    {
        for (unsigned int col = 0; col < problem.V.ColumnLen; col++)
        {
            if (problem.V[row][col] < 0.0f)
                return false;
        }
    }

    const LNMFMatrix& V = problem.V; // 原始矩阵
    LNMFMatrix W; // 基矩阵
    LNMFMatrix H; // 系数矩阵

    // 生成基矩阵
    W.Reset(V.RowLen, problem.R);
    OneNMFMatrix(W);

    // 生成特征矩阵
    H.Reset(problem.R, V.ColumnLen);
    OneNMFMatrix(H);

    LNMFMatrix TF;
    LNMFMatrix TW;
    LNMFMatrix TWW;

    LNMFMatrix HN;
    LNMFMatrix HD;
    LNMFMatrix HHN;

    LNMFMatrix WN;
    LNMFMatrix WD;
    LNMFMatrix WH;
    LNMFMatrix WWN;

    // 迭代求解
    for (unsigned int i = 0; i < problem.IterCount; i++)
    {
        LNMFMatrix::T(W, TW);
        LNMFMatrix::MUL(TW, V, HN);
        LNMFMatrix::MUL(TW, W, TWW);
        LNMFMatrix::MUL(TWW, H, HD);

        LNMFMatrix::DOTMUL(H, HN, HHN);
        LNMFMatrix::DOTDIV(HHN, HD, H);


        LNMFMatrix::T(H, TF);
        LNMFMatrix::MUL(V, TF, WN);
        LNMFMatrix::MUL(W, H, WH);
        LNMFMatrix::MUL(WH, TF, WD);

        LNMFMatrix::DOTMUL(W, WN, WWN);
        LNMFMatrix::DOTDIV(WWN, WD, W);
    }

    (*pW) = W;
    (*pH) = H;


    return true;

}


CNMF::CNMF()
{
    m_bNeedInit = true;
    m_featureNum = LNMF_ERROR_NUM;
}

CNMF::~CNMF()
{

}

bool CNMF::SetFeatureNum(int num)
{
    if (num <= 0)
        return false;

    m_featureNum = num;
    m_bNeedInit = true;

    return true;
}

bool CNMF::SetPrimitiveMatrix(const LNMFMatrix& primitiveMatrix)
{
    m_primitiveMatrix = primitiveMatrix;
    m_bNeedInit = true;
    return true;
}

bool CNMF::Init()
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

bool CNMF::FactoringOnce()
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

bool CNMF::GetWeightedMatrix(LNMFMatrix& weightedMatrix) const
{
    if (m_bNeedInit)
        return false;

    weightedMatrix = m_weightedMatrix;
    return true;
}

bool CNMF::GetFeatureMatrix(LNMFMatrix& featureMatrix) const
{
    if (m_bNeedInit)
        return false;

    featureMatrix = m_featureMatrix;
    return true;
}

/// @brief 随机构造矩阵中的值(0~10之间的浮点数)
/// @param[in] matrix
void CNMF::RandMatrix(LNMFMatrix& matrix)
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
void CNMF::NomalMatrixC(LNMFMatrix& matrix)
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