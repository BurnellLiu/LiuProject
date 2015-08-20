

#ifndef _LNMF_H_
#define _LNMF_H_

#include "LDataStruct/LMatrix.h"

typedef LMatrix<float> LNMFMatrix;

/// @brief 非负矩阵因式分解
class LNMF
{
public:
    LNMF();
    virtual ~LNMF();
    
    /// @brief 设置特征数量
    /// @param[in] num 特征数量(要求大于0的整数)
    /// @return 参数错误返回false
    bool SetFeatureNum(int num);

    /// @brief 设置原始矩阵
    /// @param[in] matrix
    /// @return true
    bool SetPrimitiveMatrix(const LNMFMatrix& primitiveMatrix);

    /// @brief 初始化
    /// @return 未对参数进行正确设置错误返回false
    bool Init();

    /// @brief 因式分解一次
    /// @return 未初始化返回false
    bool FactoringOnce();

    /// @brief 获取权重矩阵
    /// @param[out] weightedMatrix
    /// @return 未初始化返回false
    bool GetWeightedMatrix( LNMFMatrix& weightedMatrix) const;

    /// @brief 获取特征矩阵
    /// @param[out] featureMatrix
    /// @return 未初始化返回false
    bool GetFeatureMatrix( LNMFMatrix& featureMatrix) const;

private:
    /// @brief 随机构造矩阵中的值(0~10之间的浮点数)
    /// @param[inout] matrix
    void RandMatrix(LNMFMatrix& matrix);

    /// @brief 对矩阵列上的数据进行归一化
    /// @param[in] matrix
    void NomalMatrixC(LNMFMatrix& matrix);

private:
    bool m_bNeedInit; ///< 需要初始化
    int m_featureNum; ///< 特征数目
    LNMFMatrix m_primitiveMatrix; ///< 初始矩阵
    LNMFMatrix m_weightedMatrix; ///< 权重矩阵
    LNMFMatrix m_featureMatrix; ///< 特征矩阵

    LNMFMatrix TF;
    LNMFMatrix TW;
    LNMFMatrix TWW;

    LNMFMatrix FN;
    LNMFMatrix FD;
    LNMFMatrix FFN;

    LNMFMatrix WN;
    LNMFMatrix WD;
    LNMFMatrix WF;
    LNMFMatrix WWN;

private:
    LNMF(const LNMF&);
    LNMF& operator = (const LNMF&);
};

#endif