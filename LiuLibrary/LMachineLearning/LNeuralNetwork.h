

#ifndef _LNEURALNETWORK_H_
#define _LNEURALNETWORK_H_

#include <LDataStruct/LMatrix.h>

typedef LMatrix<float> LNNMatrix; // ���������


/// @brief ���򴫲�����(BackPropagation)
class LBPNetwork
{
public:
    /// @brief ���캯��
    LBPNetwork();

    /// @brief ��������
    ~LBPNetwork();

    /// @brief ��ʼ��BP����  
    /// @param[in] inputNum ������Ŀ, ����0��ֵ
    /// @param[in] outputNum �����Ŀ, ����0��ֵ
    /// @param[in] hiddenLayerNum ���ز���Ŀ, ����0��ֵ
    /// @param[in] neuronNumPerHiddenLayer ÿ�����ز��е���Ԫ��Ŀ, ����0��ֵ
    bool Init(
        IN unsigned int inputNum, 
        IN unsigned int outputNum, 
        IN unsigned int hiddenLayerNum, 
        IN unsigned int neuronNumPerHiddenLayer);

    /// @brief �̼�BP����
    /// @param[in] inputData ���������б�
    /// @param[inout] outputData ��������б�
    void Active(IN LNNInputList& inputList, INOUT LNNOutputList& outputList);

    /// @brief ѵ��BP����
    /// @param[in] inputList ��������
    /// @param[in] targetOutputList Ŀ���������
    /// @return ���������ֵ
    float Train(IN LNNInputList& inputList, IN LNNOutputList& targetOutputList);

};


#endif