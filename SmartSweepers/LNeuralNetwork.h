

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

typedef LArray<float> LNNWeightList; ///< ������Ȩ���б�
typedef LArray<float> LNNInputList; ///< �����������б�
typedef LArray<float> LNNOutputList; ///< ����������б�
typedef LArray<int> LNNSplitPointList; ///< ������Ȩ�طָ���б�


/// @brief ������
class LNeuronNet
{
public:
    LNeuronNet();
    ~LNeuronNet();

    /// @brief ��ʼ��������  
    /// @param[in] inputNum ������Ŀ
    /// @param[in] outputNum �����Ŀ
    /// @param[in] hiddenLayerNum ���ز���Ŀ
    /// @param[in] neuronNumPerHiddenLayer ÿ�����ز��е���Ԫ��Ŀ
    void Init(IN int inputNum, IN int outputNum, 
        IN int hiddenLayerNum, IN int neuronNumPerHiddenLayer);

    /// @brief �̼�������
    /// @param[in] inputData ���������б�
    /// @param[inout] outputData ��������б�
    void Active(IN LNNInputList& inputList, INOUT LNNOutputList& outputList);

protected:
    /// @brief S�ͺ���
    /// @param[in] inputA ����ֵ
    /// @param[in] inputP �������߱仯�����Ĳ���
    /// @return ���ֵ
    float Sigmoid(IN float inputA, IN float inputP);

    /// @brief ������Դ
    void CleanUp();

protected:
    /// @brief ��Ԫ�ṹ
    struct SNeuron
    {
        /// @brief ���캯��
        /// @param[in] IN inputNum �������
        explicit SNeuron(IN int inputNum);
        ~SNeuron();

        int InputNum; ///< �������
        LNNWeightList WeightList; ///< Ȩ���б�

        // �����������мල�����練�򴫲��б�ʹ��
        float Output; ///< ��Ԫ�����ֵ
        float Error; ///< ��Ԫ�����ֵ
        LNNWeightList PrevUpdate; ///< ǰһ��Ȩ�ظ���ֵ
    private:
        // ��ֹĬ�ϸ�ֵ�������Ϳ������캯��
        SNeuron(const SNeuron&);
        SNeuron& operator = (const SNeuron&);
    };
    typedef LArray<SNeuron*> LNeuronList; ///< ��Ԫ�б�

    /// @brief ��Ԫ��
    struct SNeuronLayer
    {
        /// @brief ���캯��
        /// @param[in] neuronNum ��Ԫ��Ŀ
        /// @param[in] neuronInputNum ��Ԫ�������
        SNeuronLayer(IN int neuronNum, IN int neuronInputNum);

        ~SNeuronLayer();

        int NeuronNum; ///< ��Ԫ��Ŀ
        LNeuronList NeuronList; ///< ��Ԫ�б�
        LNNInputList InputList; ///< ��Ԫ�����б�
        LNNOutputList OutputList; ///< ��Ԫ����б�
    private:
        // ��ֹĬ�ϸ�ֵ�������Ϳ������캯��
        SNeuronLayer(const SNeuronLayer&);
        SNeuronLayer& operator = (const SNeuronLayer&);
    };
    typedef LArray<SNeuronLayer*> LNeuronLayerList; ///< ��Ԫ���б�

protected:
    int m_inputNum; ///< ������Ŀ(����ƫ��ֵ����)
    int m_outputNum; ///< �����Ŀ
    int m_neuronNumPerHiddenLayer; ///< ÿ�����ز��е���Ԫ��Ŀ

    int m_weightNumTotal; ///< Ȩ������Ŀ
    float m_biasInput; ///< ƫ��ֵ����
    int m_layerNum; ///< ��Ԫ������Ŀ(���ز�������)
    LNeuronLayerList m_layerList; ///< ��Ԫ���б�(���ز�������)

private:
    // ��ֹĬ�ϸ�ֵ�������Ϳ������캯��
    LNeuronNet(const LNeuronNet&);
    LNeuronNet& operator = (const LNeuronNet&);
};

/// @brief �޼ලѵ��������
///
/// ��ʹ���Ŵ��㷨������������Ȩ��
class LUnSuperviseNN : public LNeuronNet
{
public:
    /// @brief ��ȡ�����������Ȩ���б�
    /// @param[inout] weightListTotal Ȩ���б�
    void GetWeightTotal(INOUT LNNWeightList& weightList);

    /// @brief ���������������Ȩ���б�
    ///
    /// Ҫ��Ȩ�ط�Χ-1~1
    /// @param[in] weightList Ȩ���б�
    void PutWeightTotal(IN LNNWeightList& weightList);

    /// @brief ��ȡ�и���б�
    /// @param[inout] splitPointList �ָ���б� 
    void GetSplitPointList(INOUT LNNSplitPointList& splitPointList);
};

/// @brief �мලѵ��������
class LSuperviseNN : public LNeuronNet
{
public:
    LSuperviseNN();
    ~LSuperviseNN();

    /// @brief �����������Ȩ��ֵΪ���ֵ
    void SetRandomWeight();

    /// @brief ѵ��������
    /// @param[in] inputList ��������
    /// @param[in] targetOutputList Ŀ���������
    /// @return ���������ֵ
    float Train(IN LNNInputList& inputList, IN LNNOutputList& targetOutputList);

private:
    float m_momentum; ///< ����
    float m_learnRate; ///< ѧϰ��
};
#endif