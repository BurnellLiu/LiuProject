
/// Author:Burnell_Liu Email:674288799@qq.com Date: 2015/03/03
/// Description: 回归分析
/// 
/// 线性回归分析
/// 逻辑回归分析(分类)
/// Others: 
/// Function List: 
///
/// History: 
///  1. Date, Author 
///  Modification
///

#ifndef _LREGRESSION_H_
#define _LREGRESSION_H_

#include "LDataStruct/LArray.h"

typedef float LRType;

typedef LArray<LRType> LRFeatureList; ///< 特征列表(输入变量列表)

/// @brief 训练样本
struct LRTrainingSample
{
    LRFeatureList FeatureList; ///< 特征列表(输入变量列表)
    LRType Target; ///< 目标(输出变量)
};

typedef LArray<LRTrainingSample> LRTrainingSet; ///< 训练集合

typedef LArray<LRType> LRFeatureParamList; ///< 特征参数列表(输入变量参数列表)

/// @brief 线性回归类
///
/// Detailed: 线性函数为 h(x)  =  θ0 * x0 + θ1 * x1 + θ2 * x2 + θ3 * x3 + ... = θ * X
/// 其中xi为第i个特征变量, θi为第i个特征参数, x0为1
/// θ为特征参数的列向量, X为特征的行向量
class LLinearRegression
{
public:
    /// @brief 构造函数
    /// @param[in] trainingSet 训练集合, 要求训练集合中的所有训练样本的特征数目相同
    /// @param[in ] learningRate 学习速度, 学习速度要求为大于0的数
    LLinearRegression(IN const LRTrainingSet& trainingSet, IN LRType learningRate);
    ~LLinearRegression();

    /// @brief 批梯度下降一次
    /// @return 各个训练样本误差的平方的总和, 发生错误返回-1.0
    LRType BatchDescend();

    /// @brief 进行预测
    /// @param[in] featureList 需要预测的特征值列表
    /// @return 预测出的结果
    LRType ForeCast(IN const LRFeatureList& featureList);

    /// @brief 获取当前特征参数的二乘值
    /// @return 二乘值, 二乘值为大于0的数, 二乘值越小表示特征参数越准确
    LRType GetSquareValue();

    /// @brief 获取特征的参数
    /// @param[out] paramList 特征参数列表, 参数列表的长度为训练样本特征的长度加1(特征参数列表的最后一项为常数项的值)
    void GetFeatureParam(OUT LRFeatureParamList& paramList);

private:
    const LRTrainingSet& m_trainingSet; ///< 训练集合
    LRType m_learningRate; ///< 学习速度 
    LRFeatureParamList m_paramList; ///< 特征参数列表
};

/// @brief 逻辑回归(分类)
///
/// Detailed: 逻辑函数为 h(x)  =  1/(1 + e**(-θ * X)) = 1/(1 + e**(-θ0 * x0 - θ1 * x1 - θ2 * x2 - θ3 * x3 - ...))
/// 其中xi为第i个特征变量, θi为第i个特征参数, x0为1
/// θ为特征参数的列向量, X为特征的行向量
class LLogisticRegression
{
public:
    /// @brief 二元分类
    enum LRBINARY_CLASS
    {
        ZERO = 0, ///< 0
        ONE ///< 1
    };
    /// @brief 构造函数
    /// @param[in] trainingSet 训练集合, 要求训练集合中的所有训练样本的特征数目相同, 切训练样本的Target值只能为LRBINARY_CLASS(二元分类)的值
    /// @param[in ] learningRate 学习速度, 学习速度要求为大于0的数
    LLogisticRegression(IN const LRTrainingSet& trainingSet, IN LRType learningRate);
    ~LLogisticRegression();

    /// @brief 批梯度上升一次
    void BatchAscend();

    /// @brief 进行预测
    /// @param[in] featureList 需要预测的特征值列表
    /// @param[in] bClass 指定的结果
    /// @return 指定发生结果的概率, 范围(0~1), 发生错误返回-1.0f
    LRType ForeCast(IN const LRFeatureList& featureList, IN LRBINARY_CLASS bClass);

    /// @brief 获取当前特征参数的似然值
    /// @return 似然值, 范围(0~1), 似然值越大表示特征参数越准确
    LRType GetLikelihood();

    /// @brief 获取特征的参数
    /// @param[out] paramList 特征参数列表, 参数列表的长度为训练样本特征的长度加1(特征参数列表的最后一项为常数项的值)
    void GetFeatureParam(OUT LRFeatureParamList& paramList);

private:
    const LRTrainingSet& m_trainingSet; ///< 训练集合
    LRType m_learningRate; ///< 学习速度 
    LRFeatureParamList m_paramList; ///< 特征参数列表
};

#endif

