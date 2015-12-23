
#include "LNeuralNetwork.h"

#include <cmath>
#include <cstdlib>

#include <vector>
using std::vector;

/// @brief �������С��, ��Χ0~1
/// @return ���С��
static float RandFloat()		   
{
    return (rand())/(RAND_MAX + 1.0f);
}

/// @brief �������С��, ��Χ-1~1
/// @return ���С��
static float RandClamped()	   
{
    return RandFloat() - RandFloat();
}

/// @brief BP�����е���Ԫ
/// �����еķ����������������Ч��
class CBPNeuron
{
public:
    /// @brief ���캯��
    /// @param[in] inputNum �������, ������ڵ���1
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

    /// @brief ��������
    ~CBPNeuron()
    {

    }

    /// @brief ������Ԫ
    /// @param[in] inputVector ��������(������), �������ȱ��������Ԫ���������
    /// @return ����ֵ, ����ֵ��Χ0~1
    float Active(IN const LNNMatrix& inputVector)
    {
        LNNMatrix::MUL(inputVector, m_weightVector, m_activeMatrix);

        return this->Sigmoid(m_activeMatrix[0][0] + m_b);
    }

private:
    /// @brief S�ͼ����
    /// @param[in] input ����ֵ
    /// @return ����ֵ
    float Sigmoid(IN float input)
    {
        return ( 1.0f / ( 1.0f + exp(-input)));
    }



private:
    float m_b; ///< ƫ��ֵ
    LNNMatrix m_weightVector; ///< Ȩ������(������)
    LNNMatrix m_activeMatrix; ///< �洢����ֵ�ľ���, һ��һ��
};

/// @brief BP�����е���Ԫ��
class CBPNeuronLayer
{
public:
    /// @brief ���캯��
    /// @param[in] neuronInputNum ��Ԫ�������, ����Ϊ���ڵ���1����
    /// @param[in] neuronNum ��Ԫ����, ����Ϊ���ڵ���1����
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

    /// @brief ������Ԫ��
    /// @param[in] inputVector ��������(������), �������ȱ��������Ԫ���������
    /// @param[out] pOutputVector �������(������), �洢��Ԫ������, ��������ĳ��ȵ�����Ԫ�ĸ���, ��ֵ����Ϊ0
    /// @return �ɹ�����true, ʧ�ܷ���false, ���������ʧ��
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
    unsigned int m_neuronInputNum; ///< ��Ԫ�������
    vector<CBPNeuron*> m_neuronList; ///< ��Ԫ�б�
};

/// @brief BP����ʵ����
class CBPNetwork
{
public:
    /// @brief ���캯��
    CBPNetwork()
    {
        m_networkPogology.InputNumber = 0;
        m_networkPogology.OutputNumber = 0;
        m_networkPogology.HiddenLayerNumber = 0;
        m_networkPogology.NeuronsOfHiddenLayer = 0;

        m_learnRate = 0.5f;

        m_bInitDone = false;
    }

    /// @brief ��������
    ~CBPNetwork()
    {
        this->CleanUp();
    }

    /// @brief ��ʼ��BP����
    /// ��ϸ���ͼ�ͷ�ļ�LBPNetwork�е�����
    bool Init(IN const LBPNetworkPogology& pogology)
    {
        if (pogology.InputNumber < 1 || pogology.OutputNumber < 1 ||
            pogology.HiddenLayerNumber < 1 || pogology.NeuronsOfHiddenLayer < 1)
            return false;

        m_networkPogology = pogology;

        this->CleanUp();

        // ������һ�����ز�
        CBPNeuronLayer* pFirstHiddenLayer = new CBPNeuronLayer(pogology.InputNumber, pogology.NeuronsOfHiddenLayer);
        m_layerList.push_back(pFirstHiddenLayer);

        // ����ʣ������ز�
        for (unsigned int i = 1; i < pogology.HiddenLayerNumber; i++)
        {
            CBPNeuronLayer* pHiddenLayer = new CBPNeuronLayer(pogology.NeuronsOfHiddenLayer, pogology.NeuronsOfHiddenLayer);
            m_layerList.push_back(pHiddenLayer);
        }

        // ���������
        CBPNeuronLayer* pOutputLayer = new CBPNeuronLayer(pogology.NeuronsOfHiddenLayer, pogology.OutputNumber);
        m_layerList.push_back(pOutputLayer);


        m_bInitDone = true;
        return true;
    }

    /// @brief ����ѧϰ����
    /// ��ϸ���ͼ�ͷ�ļ�LBPNetwork�е�����
    bool SetLearnRate(IN float rate)
    {
        if (rate <= 0.0f)
            return false;

        m_learnRate = rate;

        return true;
    }

    /// @brief ѵ��BP����
    /// ��ϸ���ͼ�ͷ�ļ�LBPNetwork�е�����
    bool Train(IN const LNNMatrix& inputMatrix, IN const LNNMatrix& outputMatrix)
    {
        return true;
    }

    /// @brief ����BP����
    /// ��ϸ���ͼ�ͷ�ļ�LBPNetwork�е�����
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


        vector<LNNMatrix> layerOutputList(m_layerList.size()); // ��Ԫ������б�
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
    /// @brief ������Դ
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
    bool m_bInitDone; ///< ��ʶ�Ƿ��ʼ���������
    float m_learnRate; ///< ѧϰ����
    LBPNetworkPogology m_networkPogology; ///< �������˽ṹ
    vector<CBPNeuronLayer*> m_layerList; ///< ��Ԫ���б�
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