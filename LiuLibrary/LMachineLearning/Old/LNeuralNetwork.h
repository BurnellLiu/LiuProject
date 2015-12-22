

#ifndef _LNEURALNETWORK_H_
#define _LNEURALNETWORK_H_

#include <LDataStruct/LArray.h>

#ifndef IN
#define IN
#endif

#ifndef INOUT
#define INOUT
#endif

#ifndef OUT
#define OUT
#endif

typedef LArray<float> LNNWeightList; ///< 神经网络权重列表
typedef LArray<float> LNNInputList; ///< 神经网络输入列表
typedef LArray<float> LNNOutputList; ///< 神经网络输出列表
typedef LArray<int> LNNSplitPointList; ///< 神经网络权重分割点列表


/// @brief 神经网络
class LNeuronNet
{
public:
    LNeuronNet();
    ~LNeuronNet();

    /// @brief 初始化神经网络  
    /// @param[in] inputNum 输入数目
    /// @param[in] outputNum 输出数目
    /// @param[in] hiddenLayerNum 隐藏层数目
    /// @param[in] neuronNumPerHiddenLayer 每个隐藏层中的神经元数目
    void Init(IN int inputNum, IN int outputNum, 
        IN int hiddenLayerNum, IN int neuronNumPerHiddenLayer);

    /// @brief 刺激神经网络
    /// @param[in] inputData 输入数据列表
    /// @param[inout] outputData 输出数据列表
    void Active(IN LNNInputList& inputList, INOUT LNNOutputList& outputList);

protected:
    /// @brief S型函数
    /// @param[in] inputA 激励值
    /// @param[in] inputP 控制曲线变化快慢的参数
    /// @return 输出值
    float Sigmoid(IN float inputA, IN float inputP);

    /// @brief 清理资源
    void CleanUp();

protected:
    /// @brief 神经元结构
    struct SNeuron
    {
        /// @brief 构造函数
        /// @param[in] IN inputNum 输入个数
        explicit SNeuron(IN int inputNum);
        ~SNeuron();

        int InputNum; ///< 输入个数
        LNNWeightList WeightList; ///< 权重列表

        // 以下属性在有监督神经网络反向传播中被使用
        float Output; ///< 神经元的输出值
        float Error; ///< 神经元的误差值
        LNNWeightList PrevUpdate; ///< 前一次权重更新值
    private:
        // 禁止默认赋值操作符和拷贝构造函数
        SNeuron(const SNeuron&);
        SNeuron& operator = (const SNeuron&);
    };
    typedef LArray<SNeuron*> LNeuronList; ///< 神经元列表

    /// @brief 神经元层
    struct SNeuronLayer
    {
        /// @brief 构造函数
        /// @param[in] neuronNum 神经元数目
        /// @param[in] neuronInputNum 神经元输入个数
        SNeuronLayer(IN int neuronNum, IN int neuronInputNum);

        ~SNeuronLayer();

        int NeuronNum; ///< 神经元数目
        LNeuronList NeuronList; ///< 神经元列表
        LNNInputList InputList; ///< 神经元输入列表
        LNNOutputList OutputList; ///< 神经元输出列表
    private:
        // 禁止默认赋值操作符和拷贝构造函数
        SNeuronLayer(const SNeuronLayer&);
        SNeuronLayer& operator = (const SNeuronLayer&);
    };
    typedef LArray<SNeuronLayer*> LNeuronLayerList; ///< 神经元层列表

protected:
    int m_inputNum; ///< 输入数目(包含偏移值输入)
    int m_outputNum; ///< 输出数目
    int m_neuronNumPerHiddenLayer; ///< 每个隐藏层中的神经元数目

    int m_weightNumTotal; ///< 权重总数目
    float m_biasInput; ///< 偏移值输入
    int m_layerNum; ///< 神经元层总数目(隐藏层和输出层)
    LNeuronLayerList m_layerList; ///< 神经元层列表(隐藏层和输出层)

private:
    // 禁止默认赋值操作符和拷贝构造函数
    LNeuronNet(const LNeuronNet&);
    LNeuronNet& operator = (const LNeuronNet&);
};

/// @brief 无监督训练神经网络
///
/// 可使用遗传算法来更新神经网络权重
class LUnSuperviseNN : public LNeuronNet
{
public:
    /// @brief 获取整个神经网络的权重列表
    /// @param[inout] weightListTotal 权重列表
    void GetWeightTotal(INOUT LNNWeightList& weightList);

    /// @brief 更新整个神经网络的权重列表
    ///
    /// 要求权重范围-1~1
    /// @param[in] weightList 权重列表
    void PutWeightTotal(IN LNNWeightList& weightList);

    /// @brief 获取切割点列表
    /// @param[inout] splitPointList 分割点列表 
    void GetSplitPointList(INOUT LNNSplitPointList& splitPointList);
};

/// @brief 有监督训练神经网络
class LSuperviseNN : public LNeuronNet
{
public:
    LSuperviseNN();
    ~LSuperviseNN();

    /// @brief 设置神经网络的权重值为随机值
    void SetRandomWeight();

    /// @brief 训练神经网络
    /// @param[in] inputList 输入数据
    /// @param[in] targetOutputList 目标输出数据
    /// @return 返回误差总值
    float Train(IN LNNInputList& inputList, IN LNNOutputList& targetOutputList);

private:
    float m_momentum; ///< 动量
    float m_learnRate; ///< 学习率
};
#endif