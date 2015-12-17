
/// @file LNMF.h
/// @brief 非负矩阵因式分解
/// 将一个非负矩阵分解为两个非负矩阵的乘, V = W * H
/// V为原始矩阵, W为基矩阵, H为系数矩阵
/// 基矩阵的列数和系数矩阵的行数为R
/// @author Burnell Liu Email:burnell_liu@outlook.com
/// @version   
/// @date 2015:12:17

#ifndef _LNMF_H_
#define _LNMF_H_

#include "LDataStruct/LMatrix.h"

typedef LMatrix<float> LNMFMatrix; ///< NMF矩阵

/// @brief NMF问题结构
struct LNMFProblem
{
    /// @brief 构造函数
    /// @param[in] v 原始矩阵, 原始矩阵中不能有负数
    /// @param[in] r 基矩阵的列数(系数矩阵的行数)
    /// @param[in] iterCount 迭代次数
    LNMFProblem(IN const LNMFMatrix& v, IN unsigned int r, IN unsigned int iterCount)
        : V(v), R(r), IterCount(iterCount)
    {

    }

    const LNMFMatrix& V; ///< 原始矩阵
    const unsigned int R; ///< 基矩阵的列数(系数矩阵的行数)
    const unsigned int IterCount; ///< 迭代次数
};

/// @brief 非负矩阵因式分解
class LNMF
{
public:
    /// @brief 构造函数
    LNMF();

    /// @brief 析构函数
    ~LNMF();

    /// @brief 因式分解
    /// @param[in] problem 原始问题, 原始矩阵中不能有负数
    /// @param[out] pW 存储分解后的基矩阵, 不能为0
    /// @param[out] pH 存储分解后的系数矩阵, 不能为0
    /// @return 成功返回true, 失败返回false, 参数有误会失败
    bool Factoring(IN const LNMFProblem& problem, OUT LNMFMatrix* pW, OUT LNMFMatrix* pH);
};

/// @brief 非负矩阵因式分解
class CNMF
{
public:
    CNMF();
    virtual ~CNMF();
    
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
    CNMF(const CNMF&);
    CNMF& operator = (const CNMF&);
};

#endif