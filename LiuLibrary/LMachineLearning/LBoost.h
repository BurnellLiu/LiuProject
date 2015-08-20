/// @file LBoost.h
/// @brief 提升算法
/// 
/// Detail:
/// @author Burnell_Liu  
/// @version   
/// @date 22:7:2015


#ifndef _LBOOST_H_
#define _LBOOST_H_

#include "LDataStruct/LMatrix.h"

#ifndef IN
#define IN
#endif

#ifndef INOUT
#define INOUT
#endif

#ifndef OUT
#define OUT
#endif

/// @brief 二元分类中的一类: 月(阴), 取太极两仪之意
#ifndef LBOOST_MOON
#define LBOOST_MOON -1.0f
#endif

/// @brief 二元分类中的一类: 日(阳), 取太极两仪之意
#ifndef LBOOST_SUN
#define LBOOST_SUN 1.0f
#endif

typedef LMatrix<float> LBoostMatrix;
struct LBoostStump;

/// @brief Boost原始问题结构
struct LBoostProblem
{
    /// @brief 构造函数
    ///  
    /// @param[in] sampleMatrix 样本矩阵, 每一行为一个样本, 每行中的值为样本的特征值
    /// @param[in] classVector 类别向量(列向量), 行数为样本矩阵的行数, 列数为1, 只能为BOOST_MOON或BOOST_SUN
    LBoostProblem(IN const LBoostMatrix& sampleMatrix, IN const LBoostMatrix& classVector)
        : XMatrix(sampleMatrix), YVector(classVector)
    {
    }

    const LBoostMatrix& XMatrix; ///< 样本矩阵
    const LBoostMatrix& YVector; ///< 标签向量(列向量)
};

/// @brief 提升树
///
/// 以决策树为基函数的提升方法称为提升树
class LBoostTree
{
public:
    LBoostTree();
    ~LBoostTree();

    /// @brief 设置最大弱分类器数量
    ///  
    /// @param[in] num 弱分类器数量
    void SetMaxClassifierNum(IN unsigned int num);

    /// @brief 训练模型
    ///  
    /// @param[in] problem 原始问题
    /// @return 返回true表示训练成功, 返回false表示参数数据错误
    bool TrainModel(IN const LBoostProblem& problem);

    /// @brief 使用训练好的模型进行预测(单样本预测)
    ///  
    /// 请保证需要预测的样本的特征长度和训练样本的特征长度相同
    /// @param[in] sample 需要预测的样本
    /// @return 返回预测结果: BOOST_SUN or BOOST_MOON, 返回0.0表示出错(需要预测的样本出错或者模型没有训练好)
    float Predict(IN const LBoostMatrix& sample);

    /// @brief 使用训练好的模型进行预测(多样本预测)
    ///  
    /// 请保证需要预测的样本的特征长度和训练样本的特征长度相同
    /// @param[in] sampleMatrix 需要预测的样本矩阵
    /// @return 返回true表示成功, 返回false表示出错(需要预测的样本出错或者模型没有训练好)
    bool Predict(IN const LBoostMatrix& sampleMatrix, OUT LBoostMatrix& classisVector);

public:
    /// @brief 构造树桩
    ///  
    /// @param[in] problem 原始问题
    /// @param[in] weightVector 权重向量(列向量), 行数为原始问题样本矩阵的行数, 列数为1
    /// @param[out] stump 构造好的树桩
    /// @param[out] classisVector 分类号的类别向量(列向量), 向量行数为样本矩阵的行数, 向量列数为1
    /// @return
    void BuildStump(
        IN const LBoostProblem& problem, 
        IN const LBoostMatrix& weightVector,
        OUT LBoostStump& stump,
        OUT LBoostMatrix& classisVector);

    /// @brief 使用树桩对训练样本进行分类
    ///  
    /// @param[in] sampleMatrix 样本矩阵
    /// @param[in] stump 进行分类的树桩
    /// @param[out] classisVector 类别向量(列向量), 向量行数为样本矩阵的行数, 向量列数为1
    /// @return
    void StumpClassify(
        IN const LBoostMatrix& sampleMatrix,
        IN const LBoostStump& stump,
        OUT LBoostMatrix& classisVector);

private:
    LBoostStump* m_pWeakClassifierList; ///< 弱分类器列表
    unsigned int m_weakClassifierNum; ///< 弱分类器数量
    unsigned int m_maxWeakClassifierNum; ///< 最大弱分类器数量
    unsigned int m_featureNumber; ///< 样本的特征数量
};


#endif