
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
        m_weightList.resize(inputNum + 1);
        for (unsigned int i = 0; i < m_weightList.size(); i++)
        {
            m_weightList[i] = RandClamped();
        }
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
        float sum = 0.0f;
        for (unsigned int i = 0; i < inputVector.ColumnLen; i++)
        {
            sum += inputVector[0][i] * m_weightList[i];
        }

        sum += m_weightList[inputVector.ColumnLen] * 1.0f;

        return this->Sigmoid(sum);
    }

    /// @brief ����ѵ��
    /// @param[in] inputList ����Ԫ�������б�
    /// @param[in] error ����Ԫ��������
    /// @param[in] learnRate ѧϰ����
    /// @param[out] pFrontErrorList �洢ǰ���������б� , ����Ϊ0
    /// @return �ɹ�����true, ʧ�ܷ���false, ���������ʧ��
    bool BackTrain(
        IN const vector<float>& inputList, 
        IN float error, 
        IN float learnRate,
        OUT vector<float>* pFrontErrorList)
    {
        if (inputList.size() != (m_weightList.size()-1))
            return false;

        if (0 == pFrontErrorList)
            return false;

        for (unsigned int i = 0; i < inputList.size(); i++)
        {
            m_weightList[i] += learnRate * inputList[i] * error;
            (*pFrontErrorList)[i] = m_weightList[i] * error;
        }

        m_weightList[m_weightList.size()-1] += learnRate * 1.0f * error;

        return true;

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
    vector<float> m_weightList; ///< Ȩ���б�, Ȩ��ֵ���һ��Ϊƫ��ֵ
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

        m_inputList.resize(neuronInputNum);
        m_frontErrorList.resize(neuronInputNum);
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

        for (unsigned int i = 0; i < inputVector.ColumnLen; i++)
        {
            m_inputList[i] = inputVector[0][i];
        }

        for (unsigned int i = 0; i < m_neuronList.size(); i++)
        {
            (*pOutputVector)[0][i] = m_neuronList[i]->Active(inputVector);
        }

        return true;
    }

    /// @brief ����ѵ��
    /// @param[in] opErrorList ������������б�
    /// @param[in] learnRate ѧϰ����
    /// @param[out] pFrontOpErrorList �洢ǰһ����������б�, ����Ϊ0
    /// @return �ɹ�����true, ʧ�ܷ���false, ���������ʧ��
    bool BackTrain(IN const vector<float>& opErrorList, IN float learnRate, OUT vector<float>* pFrontOpErrorList)
    {
        if (opErrorList.size() != m_neuronList.size())
            return false;

        if (0 == pFrontOpErrorList)
            return false;

        // ��ʼ��ǰһ����������б�
        for (unsigned int i = 0; i < pFrontOpErrorList->size(); i++)
        {
            (*pFrontOpErrorList)[i] = 0.0f; 
        }

        // ��ÿ����Ԫ���з���ѵ��, ����ȡÿ����Ԫ��ǰ�����������б�
        for (unsigned int i = 0; i < m_neuronList.size(); i++)
        {
            m_neuronList[i]->BackTrain(m_inputList, opErrorList[i], learnRate, &m_frontErrorList);

            // �ۼӸ�����Ԫ�����
            for (unsigned int j = 0; j < pFrontOpErrorList->size(); j++)
            {
                (*pFrontOpErrorList)[j] += m_frontErrorList[j];
            }
        }

        for (unsigned int i = 0; i < pFrontOpErrorList->size(); i++)
        {
            (*pFrontOpErrorList)[i] *= m_inputList[i] * (1.0f-m_inputList[i]);
        }

        return true;

    }

private:
    unsigned int m_neuronInputNum; ///< ��Ԫ�������
    vector<CBPNeuron*> m_neuronList; ///< ��Ԫ�б�
    vector<float> m_inputList; ///< ��Ԫ������ֵ�б�, ÿ�ε���Active����������
    vector<float> m_frontErrorList; ///< ��Ԫǰ��������
};

/// @brief BP����ʵ����
class CBPNetwork
{
public:
    /// @brief ���캯��
    CBPNetwork(IN const LBPNetworkPogology& pogology)
    {
        m_networkPogology.InputNumber = 0;
        m_networkPogology.OutputNumber = 0;
        m_networkPogology.HiddenLayerNumber = 0;
        m_networkPogology.NeuronsOfHiddenLayer = 0;

        m_bInitDone = false;

        this->Init(pogology);
    }

    /// @brief ��������
    ~CBPNetwork()
    {
        this->CleanUp();
    }

    /// @brief ѵ��BP����
    /// ��ϸ���ͼ�ͷ�ļ�LBPNetwork�е�����
    bool Train(IN const LNNMatrix& inputMatrix, IN const LNNMatrix& outputMatrix, IN float rate)
    {
        if (!m_bInitDone)
            return false;

        // ������
        if (inputMatrix.RowLen < 1)
            return false;

        if (inputMatrix.ColumnLen != m_networkPogology.InputNumber)
            return false;

        if (outputMatrix.ColumnLen != m_networkPogology.OutputNumber)
            return false;

        if (outputMatrix.RowLen != inputMatrix.RowLen)
            return false;


        // ���ÿ��ѵ������, �ֱ�ѵ��
        for (unsigned int row = 0; row < inputMatrix.RowLen; row++)
        {
            inputMatrix.GetRow(row, m_inputVectorForTrain);
            this->Active(m_inputVectorForTrain, &m_outputVectorForTrain);

            // ������������
            vector<float>& errorList = m_layerErrorList[m_layerErrorList.size()-1];
            for (unsigned int i = 0; i < m_outputVectorForTrain.ColumnLen; i++)
            {
                errorList[i] = outputMatrix[row][i]-m_outputVectorForTrain[0][i];
                errorList[i] *= m_outputVectorForTrain[0][i] * (1.0f-m_outputVectorForTrain[0][i]);
            }

            // �Ӻ���ǰ���з���ѵ��
            for (int i = int(m_layerList.size()-1); i >= 0; i--)
            {
                m_layerList[i]->BackTrain(m_layerErrorList[i + 1], rate, &m_layerErrorList[i]);
            }

        }

        
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


        for (unsigned int row = 0; row < inputMatrix.RowLen; row++)
        {
            inputMatrix.GetRow(row, m_inputVectorForActive);

            for (unsigned int i = 0; i < m_layerList.size(); i++)
            {
                if (0 == i)
                    m_layerList[i]->Active(m_inputVectorForActive, &m_layerOutList[i]);
                else
                    m_layerList[i]->Active(m_layerOutList[i-1], &m_layerOutList[i]);
            }

            for (unsigned int col = 0; col < pOutputMatrix->ColumnLen; col++)
            {
                (*pOutputMatrix)[row][col] = m_layerOutList[m_layerOutList.size()-1][0][col];
            }
        }

        return true;
    }

private:
    /// @brief ��ʼ��BP����
    bool Init(IN const LBPNetworkPogology& pogology)
    {
        if (pogology.InputNumber < 1 || pogology.OutputNumber < 1 ||
            pogology.HiddenLayerNumber < 1 || pogology.NeuronsOfHiddenLayer < 1)
            return false;

        m_networkPogology = pogology;

        this->CleanUp();

        m_layerOutList.resize(pogology.HiddenLayerNumber + 1);
        m_layerErrorList.resize(pogology.HiddenLayerNumber + 2);
        m_inputVectorForTrain.Reset(1, pogology.InputNumber);
        m_outputVectorForTrain.Reset(1, pogology.OutputNumber);
        m_inputVectorForActive.Reset(1, pogology.InputNumber);

        m_layerErrorList[0].resize(pogology.InputNumber);


        // ������һ�����ز�
        CBPNeuronLayer* pFirstHiddenLayer = new CBPNeuronLayer(pogology.InputNumber, pogology.NeuronsOfHiddenLayer);
        m_layerList.push_back(pFirstHiddenLayer);
        m_layerOutList[0].Reset(1, pogology.NeuronsOfHiddenLayer);
        m_layerErrorList[1].resize(pogology.NeuronsOfHiddenLayer);

        // ����ʣ������ز�
        for (unsigned int i = 1; i < pogology.HiddenLayerNumber; i++)
        {
            CBPNeuronLayer* pHiddenLayer = new CBPNeuronLayer(pogology.NeuronsOfHiddenLayer, pogology.NeuronsOfHiddenLayer);
            m_layerList.push_back(pHiddenLayer);
            m_layerOutList[i].Reset(1, pogology.NeuronsOfHiddenLayer);
            m_layerErrorList[i + 1].resize(pogology.NeuronsOfHiddenLayer);
        }

        // ���������
        CBPNeuronLayer* pOutputLayer = new CBPNeuronLayer(pogology.NeuronsOfHiddenLayer, pogology.OutputNumber);
        m_layerList.push_back(pOutputLayer);
        m_layerOutList[pogology.HiddenLayerNumber].Reset(1, pogology.OutputNumber);
        m_layerErrorList[pogology.HiddenLayerNumber + 1].resize(pogology.OutputNumber);


        m_bInitDone = true;
        return true;
    }

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
        m_layerOutList.clear();
        m_layerErrorList.clear();
    }

private:
    bool m_bInitDone; ///< ��ʶ�Ƿ��ʼ���������
    LBPNetworkPogology m_networkPogology; ///< �������˽ṹ
    vector<CBPNeuronLayer*> m_layerList; ///< ��Ԫ���б�

    /*
    ���³�Ա����ΪTrain��Active����, Ϊ���ڶ�ε���Train��Active����ʱ��߳���Ч��
    */
    vector<LNNMatrix> m_layerOutList; ///< ��Ԫ������б�
    vector<vector<float>> m_layerErrorList; ///< ��Ԫ���������б�
    LNNMatrix m_inputVectorForTrain; ///< ��������Train����ʹ��
    LNNMatrix m_outputVectorForTrain; ///< �������Train����ʹ��
    LNNMatrix m_inputVectorForActive; ///< ��������Active����ʹ��
};

LBPNetwork::LBPNetwork(IN const LBPNetworkPogology& pogology)
{
    m_pBPNetwork = 0;
    m_pBPNetwork = new CBPNetwork(pogology);
}

LBPNetwork::~LBPNetwork()
{
    if (0 != m_pBPNetwork)
    {
        delete m_pBPNetwork;
        m_pBPNetwork = 0;
    }
}

bool LBPNetwork::Train(IN const LNNMatrix& inputMatrix, IN const LNNMatrix& outputMatrix, IN float rate)
{
    return m_pBPNetwork->Train(inputMatrix, outputMatrix, rate);
}

bool LBPNetwork::Active(IN const LNNMatrix& inputMatrix, OUT LNNMatrix* pOutputMatrix)
{
    return m_pBPNetwork->Active(inputMatrix, pOutputMatrix);
}