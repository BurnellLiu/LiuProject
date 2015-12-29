
/// @file LPerceptron.h
/// @brief 感知机
/// 
/// Detail:感知机要求训练样本线性可分
/// @author Burnell_Liu  
/// @version   
/// @date 30:7:2015

#ifndef _LPERCEPTRON_H_
#define _LPERCEPTRON_H_

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
#ifndef LPERCEPTRON_MOON
#define LPERCEPTRON_MOON -1.0f
#endif

/// @brief 二元分类中的一类: 日(阳), 取太极两仪之意
#ifndef LPERCEPTRON_SUN
#define LPERCEPTRON_SUN 1.0f
#endif

typedef LMatrix<float> LPerceptronMatrix;

/// @brief 感知机原始问题结构
struct LPerceptronProblem
{
    /// @brief 构造函数
    ///  
    /// @param[in] sampleMatrix 训练样本矩阵, 每一行为一个样本, 每行中的值为样本的特征值
    /// @param[in] classVector 类别向量(列向量), 行数为样本矩阵的行数, 列数为1, 只能为LPERCEPTRON_MOON或LPERCEPTRON_SUN
    LPerceptronProblem(IN const LPerceptronMatrix& sampleMatrix, IN const LPerceptronMatrix& classVector)
        : XMatrix(sampleMatrix), YVector(classVector)
    {
    }

    const LPerceptronMatrix& XMatrix; ///< 训练样本矩阵
    const LPerceptronMatrix& YVector; ///< 标签向量(列向量)
};


class CPerceptron;

/// @brief 感知机
class LPerceptron
{
public:
    /// @brief 构造函数
    LPerceptron();

    /// @brief 析构函数
    ~LPerceptron();

    /// @brief 设置学习速率(默认值为1.0f)
    /// @param[in] rate 学习速率, 学习速率为大于0的数
    /// @return 成功设置返回true, 设置失败返回false, 参数有误会失败
    bool SetLearningRate(IN float rate);

    /// @brief 训练模型
    /// @param[in] problem 原始问题
    /// @return 返回true表示训练成功, 返回false表示参数数据错误
    bool TrainModel(IN const LPerceptronProblem& problem);

    /// @brief 使用训练好的模型进行预测(单样本预测)
    /// 请保证需要预测的样本的特征长度和训练样本的特征长度相同
    /// @param[in] sample 需要预测的样本(行向量)
    /// @return 返回预测结果: LPERCEPTRON_SUN or LPERCEPTRON_MOON, 返回0.0表示出错(需要预测的样本出错或者模型没有训练好)
    float Predict(IN const LPerceptronMatrix& sample);

private:
    CPerceptron* m_pPerceptron; ///< 感知机实现对象
};



#endif