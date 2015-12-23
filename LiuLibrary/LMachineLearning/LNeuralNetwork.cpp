
#include "LNeuralNetwork.h"

#include <cmath>
#include <cstdlib>

#include <vector>
using std::vector;

/// @brief 产生随机小数, 范围0~1
/// @return 随机小数
static float RandFloat()		   
{
    return (rand())/(RAND_MAX + 1.0f);
}

/// @brief 产生随机小数, 范围-1~1
/// @return 随机小数
static float RandClamped()	   
{
    return RandFloat() - RandFloat();
}

/// @brief BP网络中的神经元
/// 该类中的方法不会检查参数的有效性
class CBPNeuron
{
public:
    /// @brief 构造函数
    /// @param[in] inputNum 输入个数, 必须大于等于1
    explicit CBPNeuron(IN unsigned int inputNum)
    {
        m_b = RandClamped();

        m_weightVector.Reset(inputNum, 1);
        for (unsigned int row = 0; row < m_weightVector.RowLen; row++)
        {
            m_weightVector[row][0] = RandClamped();
        }

        m_activeMatrix.Reset(1, 1); 
    }

    /// @brief 析构函数
    ~CBPNeuron()
    {

    }

    /// @brief 激活神经元
    /// @param[in] inputVector 输入向量(行向量), 向量长度必须等于神经元的输入个数
    /// @return 激活值, 激活值范围0~1
    float Active(IN const LNNMatrix& inputVector)
    {
        LNNMatrix::MUL(inputVector, m_weightVector, m_activeMatrix);

        return this->Sigmoid(m_activeMatrix[0][0] + m_b);
    }

private:
    /// @brief S型激活函数
    /// @param[in] input 激励值
    /// @return 激活值
    float Sigmoid(IN float input)
    {
        return ( 1.0f / ( 1.0f + exp(-input)));
    }



private:
    float m_b; ///< 偏移值
    LNNMatrix m_weightVector; ///< 权重向量(列向量)
    LNNMatrix m_activeMatrix; ///< 存储激励值的矩阵, 一行一列
};

/// @brief BP网络中的神经元层
class CBPNeuronLayer
{
public:
    /// @brief 构造函数
    /// @param[in] neuronInputNum 神经元输入个数, 必须为大于等于1的数
    /// @param[in] neuronNum 神经元个数, 必须为大于等于1的数
    CBPNeuronLayer(IN unsigned int neuronInputNum, IN unsigned int neuronNum)
    {
        m_neuronInputNum = neuronInputNum;

        for (unsigned int i = 0; i < neuronNum; i++)
        {
            CBPNeuron* pNeuron = new CBPNeuron(neuronInputNum);
            m_neuronList.push_back(pNeuron);
        }
    }

    ~CBPNeuronLayer()
    {
        for (unsigned int i = 0; i < m_neuronList.size(); i++)
        {
            if (m_neuronList[i] != 0)
            {
                delete m_neuronList[i];
                m_neuronList[i] = 0;
            }
            
        }
    }

    /// @brief 激活神经元层
    /// @param[in] inputVector 输入向量(行向量), 向量长度必须等于神经元的输入个数
    /// @param[out] pOutputVector 输出向量(行向量), 存储神经元层的输出, 输出向量的长度等于神经元的个数, 该值不能为0
    /// @return 成功返回true, 失败返回false, 参数有误会失败
    bool Active(IN const LNNMatrix& inputVector, OUT LNNMatrix* pOutputVector)
    {
        if (m_neuronInputNum < 1 || m_neuronList.size() < 1)
            return false;

        if (inputVector.ColumnLen != m_neuronInputNum)
            return false;

        if (0 == pOutputVector)
            return false;

        pOutputVector->Reset(1, m_neuronList.size());

        for (unsigned int i = 0; i < m_neuronList.size(); i++)
        {
            (*pOutputVector)[0][i] = m_neuronList[i]->Active(inputVector);
        }

        return true;
    }

private:
    unsigned int m_neuronInputNum; ///< 神经元输入个数
    vector<CBPNeuron*> m_neuronList; ///< 神经元列表
};

/// @brief BP网络实现类
class CBPNetwork
{
public:
    /// @brief 构造函数
    CBPNetwork()
    {
        m_networkPogology.InputNumber = 0;
        m_networkPogology.OutputNumber = 0;
        m_networkPogology.HiddenLayerNumber = 0;
        m_networkPogology.NeuronsOfHiddenLayer = 0;

        m_learnRate = 0.5f;

        m_bInitDone = false;
    }

    /// @brief 析构函数
    ~CBPNetwork()
    {
        this->CleanUp();
    }

    /// @brief 初始化BP网络
    /// 详细解释见头文件LBPNetwork中的声明
    bool Init(IN const LBPNetworkPogology& pogology)
    {
        if (pogology.InputNumber < 1 || pogology.OutputNumber < 1 ||
            pogology.HiddenLayerNumber < 1 || pogology.NeuronsOfHiddenLayer < 1)
            return false;

        m_networkPogology = pogology;

        this->CleanUp();

        // 创建第一个隐藏层
        CBPNeuronLayer* pFirstHiddenLayer = new CBPNeuronLayer(pogology.InputNumber, pogology.NeuronsOfHiddenLayer);
        m_layerList.push_back(pFirstHiddenLayer);

        // 创建剩余的隐藏层
        for (unsigned int i = 1; i < pogology.HiddenLayerNumber; i++)
        {
            CBPNeuronLayer* pHiddenLayer = new CBPNeuronLayer(pogology.NeuronsOfHiddenLayer, pogology.NeuronsOfHiddenLayer);
            m_layerList.push_back(pHiddenLayer);
        }

        // 创建输出层
        CBPNeuronLayer* pOutputLayer = new CBPNeuronLayer(pogology.NeuronsOfHiddenLayer, pogology.OutputNumber);
        m_layerList.push_back(pOutputLayer);


        m_bInitDone = true;
        return true;
    }

    /// @brief 设置学习速率
    /// 详细解释见头文件LBPNetwork中的声明
    bool SetLearnRate(IN float rate)
    {
        if (rate <= 0.0f)
            return false;

        m_learnRate = rate;

        return true;
    }

    /// @brief 训练BP网络
    /// 详细解释见头文件LBPNetwork中的声明
    bool Train(IN const LNNMatrix& inputMatrix, IN const LNNMatrix& outputMatrix)
    {
        return true;
    }

    /// @brief 激活BP网络
    /// 详细解释见头文件LBPNetwork中的声明
    bool Active(IN const LNNMatrix& inputMatrix, OUT LNNMatrix* pOutputMatrix)
    {
        if (!m_bInitDone)
            return false;

        if (inputMatrix.RowLen < 1)
            return false;

        if (inputMatrix.ColumnLen != m_networkPogology.InputNumber)
            return false;

        if (0 == pOutputMatrix)
            return false;

        pOutputMatrix->Reset(inputMatrix.RowLen, m_networkPogology.OutputNumber);


        vector<LNNMatrix> layerOutputList(m_layerList.size()); // 神经元层输出列表
        LNNMatrix inputVector;

        for (unsigned int row = 0; row < inputMatrix.RowLen; row++)
        {
            inputVector = inputMatrix.GetRow(row);

            for (unsigned int i = 0; i < m_layerList.size(); i++)
            {
                if (0 == i)
                    m_layerList[i]->Active(inputVector, &layerOutputList[i]);
                else
                    m_layerList[i]->Active(layerOutputList[i-1], &layerOutputList[i]);
            }

            for (unsigned int col = 0; col < pOutputMatrix->ColumnLen; col++)
            {
                (*pOutputMatrix)[row][col] = layerOutputList[layerOutputList.size()-1][0][col];
            }
        }

        return true;
    }

private:
    /// @brief 清理资源
    void CleanUp()
    {
        m_bInitDone = false;
        for (unsigned int i = 0; i < m_layerList.size(); i++)
        {
            if (0 != m_layerList[i])
            {
                delete m_layerList[i];
                m_layerList[i] = 0;
            }
        }

        m_layerList.clear();
    }

private:
    bool m_bInitDone; ///< 标识是否初始化网络完成
    float m_learnRate; ///< 学习速率
    LBPNetworkPogology m_networkPogology; ///< 网络拓扑结构
    vector<CBPNeuronLayer*> m_layerList; ///< 神经元层列表
};

LBPNetwork::LBPNetwork()
{
    m_pBPNetwork = 0;
    m_pBPNetwork = new CBPNetwork();
}

LBPNetwork::~LBPNetwork()
{
    if (0 != m_pBPNetwork)
    {
        delete m_pBPNetwork;
        m_pBPNetwork = 0;
    }
}

bool LBPNetwork::Init(IN const LBPNetworkPogology& pogology)
{
    return m_pBPNetwork->Init(pogology);
}

bool LBPNetwork::SetLearnRate(IN float rate)
{
    return m_pBPNetwork->SetLearnRate(rate);
}

bool LBPNetwork::Train(IN const LNNMatrix& inputMatrix, IN const LNNMatrix& outputMatrix)
{
    return m_pBPNetwork->Train(inputMatrix, outputMatrix);
}

bool LBPNetwork::Active(IN const LNNMatrix& inputMatrix, OUT LNNMatrix* pOutputMatrix)
{
    return m_pBPNetwork->Active(inputMatrix, pOutputMatrix);
}