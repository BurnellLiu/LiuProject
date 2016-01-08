/// @file LRegression.h
/// @brief 本文件中声明了一些回归算法
/// 
/// Detail:
/// 线性回归
/// 优点: 结果易于理解, 计算上不复杂
/// 缺点: 对非线性数据拟合不好
/// @author Burnell_Liu Email:burnell_liu@outlook.com
/// @version   
/// @date 10:10:2015

#ifndef _LREGRESSION_H_
#define _LREGRESSION_H_


#include "LDataStruct/LMatrix.h"

typedef LMatrix<float> LRegressionMatrix;



/// @brief 回归原始问题结构
struct LRegressionProblem
{
    /// @brief 构造函数
    /// @param[in] sampleMatrix 样本矩阵, 每一行为一个样本, 每行中的值为样本的特征值, 至少需要两个样本
    /// @param[in] targetVector 目标向量(列向量), 行数为样本矩阵的行数, 列数为1, 每行的值代表样本矩阵中每个样本的目标(输出)
    LRegressionProblem(IN const LRegressionMatrix& sampleMatrix, IN const LRegressionMatrix& targetVector)
        : XMatrix(sampleMatrix), YVector(targetVector)
    {
    }

    const LRegressionMatrix& XMatrix; ///< 样本矩阵
    const LRegressionMatrix& YVector; ///< 目标向量(列向量)
};

/// @brief 线性回归类
/// 线性函数为 h(x)  =  X * W
/// W为特征权重的列向量, X为特征的行向量
class LLinearRegression
{
public:
    /// @brief 构造函数
    LLinearRegression();

    /// @brief 析构函数
    ~LLinearRegression();

    /// @brief 训练模型
    /// @param[in] problem 原始问题
    /// @param[in] learningRate 学习速度, 学习速度必须大于0
    /// @param[in] trainTimes 训练次数
    /// @return 成功返回true, 失败返回false(参数错误的情况下会返回失败)
    bool TrainModel(
        IN const LRegressionProblem& problem,
        IN float learningRate,
        IN unsigned int trainTimes);

    /// @brief 获取权重向量(列向量)
    /// 权重向量的长度为样本特征数目加1, 权重向量的最后一项为常数项值
    /// @param[out] weightVector
    /// @return 模型未训练返回false, 否则返回true
    bool GetWeightVector(OUT LRegressionMatrix& weightVector);

    /// @brief 获取误差值(二乘值)
    /// @return 模型未训练返回-1.0f, 否则返回误差值
    float GetErrorValue();

private:
    LRegressionMatrix m_xMatrix; ///< 样本矩阵
    LRegressionMatrix m_yVector; ///< 目标向量(列向量)
    LRegressionMatrix m_wVector; ///<权重矩阵(列向量)
};

/// @brief 逻辑回归中的ZERO分类
#ifndef REGRESSION_ZERO
#define REGRESSION_ZERO 0.0f
#endif

/// @brief 逻辑回归中的ONE分类
#ifndef REGRESSION_ONE
#define REGRESSION_ONE 1.0f
#endif

/// @brief 逻辑回归(分类)
/// 逻辑函数为 h(x)  =  1/(1 + e^(X * W)) 
/// W为特征权重的列向量, X为特征的行向量
/// 原始问题中的目标向量中的值只能为0.0f或1.0f
/// P(1) = h(x), P(0) = 1-h(x)
class LLogisticRegression
{
public:
    /// @brief 构造函数
    LLogisticRegression();

    /// @brief 析构函数
    ~LLogisticRegression();

    /// @brief 训练模型
    /// @param[in] problem 原始问题, 目标向量中的值只能为0.0f或1.0f
    /// @param[in] learningRate 学习速度, 学习速度必须大于0
    /// @param[in] trainTimes 训练次数
    /// @return 成功返回true, 失败返回false(参数错误的情况下会返回失败)
    bool TrainModel(
        IN const LRegressionProblem& problem,
        IN float learningRate,
        IN unsigned int trainTimes);

    /// @brief 获取权重向量(列向量)
    /// 权重向量的长度为样本特征数目加1, 权重向量的最后一项为常数项值
    /// @param[out] weightVector
    /// @return 模型未训练返回false, 否则返回true
    bool GetWeightVector(OUT LRegressionMatrix& weightVector);

    /// @brief 获取当前特征参数的似然值
    /// @return 似然值, 范围(0~1), 似然值越大表示特征参数越准确, 模型未训练返回-1.0f
    float GetLikelihood();

private:
    LRegressionMatrix m_xMatrix; ///< 样本矩阵
    LRegressionMatrix m_yVector; ///< 目标向量(列向量)
    LRegressionMatrix m_wVector; ///<权重矩阵(列向量)
};

#endif