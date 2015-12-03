/// @file LBayesClassify.h
/// @brief 贝叶斯分类器
/// 
/// Detail:
/// @author Burnell_Liu Email:burnell_liu@qq.com
/// @version   
/// @date 3:12:2015

#ifndef _LBAYESCLASSIFY_H_
#define _LBAYESCLASSIFY_H_

#include "LDataStruct/LMatrix.h"

typedef LMatrix<int> LBayesMatrix;

/// @brief 贝叶斯特征数据类型
enum LBayesFeatureDataType
{
    BAYES_FEATURE_DISCRETE = 1, //< 离散
    BAYES_FEATURE_NONE_DISCAETE = 2, ///< 非离散(连续)
};

/// @brief 贝叶斯原始问题结构
/// 特征数据为非离散(连续)表示值服从高斯分布
struct LBayesProblem
{
    /// @brief 构造函数
    /// @param[in] sampleMatrix 样本矩阵, 每一行为一个样本, 每行中的值为样本的特征值
    /// @param[in] classVector 类别向量(列向量), 行数为样本矩阵的行数, 列数为1
    /// @param[in] dataType 特征数据类型
    LBayesProblem(
        IN const LBayesMatrix& sampleMatrix, 
        IN const LBayesMatrix& classVector, 
        IN LBayesFeatureDataType dataType)
        : XMatrix(sampleMatrix), YVector(classVector), FeatureDataType(dataType)
    {
    }

    const LBayesMatrix& XMatrix; ///< 样本矩阵
    const LBayesMatrix& YVector; ///< 类别向量(列向量)
    const LBayesFeatureDataType FeatureDataType; ///< 贝叶斯特征数据类型
};


class CBayesClassify;

/// @brief 贝叶斯分类器接口类
class LBayesClassify
{
public:
    LBayesClassify();
    ~LBayesClassify();

    /// @brief 训练模型
    /// @param[in] problem 贝叶斯问题
    /// @return 成功返回true, 失败返回false, 参数错误的情况下会返回false
    bool TrainModel(IN const LBayesProblem& problem);

    /// @brief 使用训练好的模型进行预测
    ///  
    /// 请保证需要预测的样本的特征长度和训练样本的特征长度相同
    /// @param[in] sample 需要预测的样本
    /// @param[out] classType 存储预测结果
    /// @return 成功预测返回true, 失败返回false, 参数错误或模型未训练的情况下会返回false
    bool Predict(IN const LBayesMatrix& sample, OUT int* classValue);


private:
    CBayesClassify* m_pBayesClassify; ///< 贝叶斯分类器实现对象

private:
    // 禁止拷贝构造函数和赋值操作符
    LBayesClassify(const LBayesClassify&);
    LBayesClassify& operator = (const LBayesClassify&);
};


#endif