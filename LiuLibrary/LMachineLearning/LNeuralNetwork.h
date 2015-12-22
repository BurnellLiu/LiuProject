

#ifndef _LNEURALNETWORK_H_
#define _LNEURALNETWORK_H_

#include <LDataStruct/LMatrix.h>

typedef LMatrix<float> LNNMatrix; // 神经网络矩阵


/// @brief 反向传播网络(BackPropagation)
class LBPNetwork
{
public:
    /// @brief 构造函数
    LBPNetwork();

    /// @brief 析构函数
    ~LBPNetwork();

    /// @brief 初始化BP网络  
    /// @param[in] inputNum 输入数目, 大于0的值
    /// @param[in] outputNum 输出数目, 大于0的值
    /// @param[in] hiddenLayerNum 隐藏层数目, 大于0的值
    /// @param[in] neuronNumPerHiddenLayer 每个隐藏层中的神经元数目, 大于0的值
    bool Init(
        IN unsigned int inputNum, 
        IN unsigned int outputNum, 
        IN unsigned int hiddenLayerNum, 
        IN unsigned int neuronNumPerHiddenLayer);

    /// @brief 刺激BP网络
    /// @param[in] inputData 输入数据列表
    /// @param[inout] outputData 输出数据列表
    void Active(IN LNNInputList& inputList, INOUT LNNOutputList& outputList);

    /// @brief 训练BP网络
    /// @param[in] inputList 输入数据
    /// @param[in] targetOutputList 目标输出数据
    /// @return 返回误差总值
    float Train(IN LNNInputList& inputList, IN LNNOutputList& targetOutputList);

};


#endif