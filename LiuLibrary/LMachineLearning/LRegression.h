/// @file LRegression.h
/// @brief 本文件中声明了一些回归算法
/// 线性回归, 逻辑回归, Softmax回归
/// Detail:
/// 线性回归:
/// 优点: 结果易于理解, 计算上不复杂
/// 缺点: 对非线性数据拟合不好
/// 线性回归的改进算法有局部加权回归
/// 
/// 逻辑回归:
/// 优点: 计算代价不高, 易于理解和实现
/// 缺点: 容易欠拟合, 分类精度可能不高
/// 
/// Softmax回归:
/// 该回归是逻辑回归在多分类问题上的推广
///
/// @author Burnell_Liu Email:burnell_liu@outlook.com
/// @version   
/// @date 10:10:2015


/*  使用Softmax回归示例代码如下:

// 训练样本数据
float trainSample[36] = 
{
    1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
    1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f
};
LRegressionMatrix X(6, 6, trainSample);

// 训练样本标签数据
float trainLabel[12] = 
{
    1.0f, 0.0f,
    1.0f, 0.0f,
    1.0f, 0.0f,
    0.0f, 1.0f,
    0.0f, 1.0f,
    0.0f, 1.0f
};
LRegressionMatrix Y(6, 2, trainLabel);

// 定义Softmax回归
LSoftmaxRegression softmaxReg(6, 6, 2);

// 训练500次
for (unsigned int i = 0; i < 500; i++)
{
    softmaxReg.TrainModel(X, Y, 0.1f);   
}

// 测试样本
float testSample[12] = 
{
    1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f
};
LRegressionMatrix testX(2, 6, testSample);

// 对测试样本进行预测
LRegressionMatrix testY;
softmaxReg.Predict(testX, testY);
*/

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
    /// @param[in] learningRate 学习速度(必须为大于0的数)
    /// @param[in] trainTimes 训练次数
    LRegressionProblem(
        IN const LRegressionMatrix& sampleMatrix, 
        IN const LRegressionMatrix& targetVector,
        IN float learningRate,
        IN unsigned int trainTimes)
        : 
        XMatrix(sampleMatrix), 
        YVector(targetVector), 
        LearningRate(learningRate), 
        TrainTimes(trainTimes)
    {
    }

    const LRegressionMatrix& XMatrix; ///< 样本矩阵
    const LRegressionMatrix& YVector; ///< 目标向量(列向量)

    float LearningRate; ///< 学习速度
    unsigned int TrainTimes; ///< 训练次数
};

class CLinearRegression;

/// @brief 线性回归类
class LLinearRegression
{
public:
    /// @brief 构造函数
    LLinearRegression();

    /// @brief 析构函数
    ~LLinearRegression();

    /// @brief 训练模型
    /// @param[in] problem 原始问题
    /// @return 成功返回true, 失败返回false(参数错误的情况下会返回失败)
    bool TrainModel(IN const LRegressionProblem& problem);

    /// @brief 使用训练好的模型预测数据
    /// @param[in] sampleMatrix 需要预测的样本矩阵
    /// @param[out] yVector 存储预测的结果向量(列向量)
    /// @return 成功返回true, 失败返回false(模型未训练或参数错误的情况下会返回失败)
    bool Predict(IN const LRegressionMatrix& sampleMatrix, OUT LRegressionMatrix& yVector) const;

    /// @brief 训练后预测数据与训练数据的相关度(即模型的优劣程度)
    /// 相关度越接近1表示模型越好
    /// @return 相关度, 模型未训练则返回0
    float GetCorrelation() const;

private:
    CLinearRegression* m_pLinearRegression; ///< 线性回归实现对象
};

/// @brief 逻辑回归中的ZERO分类
#ifndef REGRESSION_ZERO
#define REGRESSION_ZERO 0.0f
#endif

/// @brief 逻辑回归中的ONE分类
#ifndef REGRESSION_ONE
#define REGRESSION_ONE 1.0f
#endif

class CLogisticRegression;

/// @brief 逻辑回归(分类)
class LLogisticRegression
{
public:
    /// @brief 构造函数
    LLogisticRegression();

    /// @brief 析构函数
    ~LLogisticRegression();

    /// @brief 训练模型
    /// @param[in] problem 原始问题, 目标向量中的值只能为0.0f或1.0f
    /// @return 成功返回true, 失败返回false(参数错误的情况下会返回失败)
    bool TrainModel(IN const LRegressionProblem& problem);

    /// @brief 使用训练好的模型预测数据
    /// @param[in] sampleMatrix 需要预测的样本矩阵
    /// @param[out] yVector 存储预测的结果向量(列向量)
    /// @return 成功返回true, 失败返回false(模型未训练或参数错误的情况下会返回失败)
    bool Predict(IN const LRegressionMatrix& sampleMatrix, OUT LRegressionMatrix& yVector) const;

    /// @brief 获取当前训练模型的正确率
    /// @return 正确率, 模型未训练返回-1.0f
    float GetAccuracy() const;

private:
    CLogisticRegression* m_pLogisticRegression; ///< 逻辑回归实现类
};


class CSoftmaxRegression;

/// @brief Softmax回归(多分类)
/// 随机梯度下降法实现, 克服样本数据量太大, 导致的内存开销大以及训练耗时长的问题
class LSoftmaxRegression
{
public:
    /// @brief 构造函数
    /// @param[in] m 样本总个数, 不能小于2
    /// @param[in] n 样本特征值个数, 不能小于1
    /// @param[in] k 样本类别个数, 不能小于2
    LSoftmaxRegression(IN unsigned int m, IN unsigned int n, IN unsigned int k);

    /// @brief 析构函数
    ~LSoftmaxRegression();

    /// @brief 训练模型
    /// 如果训练的样本数量为1, 则为随机梯度下降
    /// 如果训练的样本数量为M(样本总数), 则为梯度下降
    /// 如果训练的样本数量为m(1 < m < M), 则为批量梯度下降
    /// @param[in] xMatrix 样本矩阵, 每一行代表一个样本, 每一列代表样本的一个特征
    /// @param[in] yMatrix 类标记矩阵, 每一行代表一个样本, 每一列代表样本的一个类别, 
    /// 如果样本属于该类别则标记为REGRESSION_ONE, 不属于则标记为REGRESSION_ZERO
    /// @param[in] alpha 学习速度, 该值必须大于0.0f
    /// @return 成功返回true, 失败返回false(参数错误的情况下会返回失败)
    bool TrainModel(IN const LRegressionMatrix& xMatrix, IN const LRegressionMatrix& yMatrix, IN float alpha);

    /// @brief 使用训练好的模型预测数据
    /// @param[in] xMatrix 需要预测的样本矩阵
    /// @param[out] yMatrix 存储预测的结果矩阵, 每一行代表一个样本, 每一列代表在该类别下的概率
    /// @return 成功返回true, 失败返回false(参数错误的情况下会返回失败)
    bool Predict(IN const LRegressionMatrix& xMatrix, OUT LRegressionMatrix& yMatrix) const;

private:
    CSoftmaxRegression* m_pSoftmaxRegression; ///< Softmax回归实现对象
};

#endif