/// @file LRegression.h
/// @brief 回归分析
/// 
/// Detail:
/// @author Burnell_Liu Email:burnell_liu@qq.com
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
///
/// Detailed: 线性函数为 h(x)  =  θ0 * x0 + θ1 * x1 + θ2 * x2 + θ3 * x3 + ... =  X * θ
/// 其中xi为第i个特征变量, θi为第i个特征参数, x0为1
/// θ为特征参数的列向量, X为特征的行向量
class LLinearRegression
{
public:
    LLinearRegression();
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
    ///  权重向量的最后一项为常数项
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

#endif