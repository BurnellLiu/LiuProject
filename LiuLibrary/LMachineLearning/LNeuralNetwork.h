/// @file LNeuralNetwork.h
/// @brief ��ͷ�ļ���������һЩ������
/// 
/// Detail:
/// LBPNetwork(���򴫲�����): �мලѧϰ, BP���������������ù�һ��(����������ȫ������Ϊ0~1֮���ֵ), BP������������Ϊ0~1
///                          ѵ��BP�������õ�Ŀ��������ݱ����һ��
/// 
/// @author Burnell Liu Email:burnell_liu@outlook.com
/// @version   
/// @date 2015:12:23
/// @sample

/* ʹ��BP������XOR����ʾ����������, XOR����Ϊ����������

// ����������ṹ
LBPNetworkPogology pogology;
pogology.InputNumber = 2;
pogology.HiddenLayerNumber = 1;
pogology.OutputNumber = 1;
pogology.NeuronsOfHiddenLayer = 2;

// ��ʼ�������粢������ѧϰ����
LBPNetwork bpNetwork(pogology);

// �����������
float inputList[8] = 
{
    1.0f, 0.0f,
    0.0f, 1.0f,
    0.0f, 0.0f,
    1.0f, 1.0f,
};

LNNMatrix input(4, 2, inputList);

// ����Ŀ���������
LNNMatrix targetOutput(4, 1);
targetOutput[0][0] = 1.0f;
targetOutput[1][0] = 1.0f;
targetOutput[2][0] = 0.0f;
targetOutput[3][0] = 0.0f;

// ѵ��1000��
for (int i = 0; i < 1000; i++)
{
    bpNetwork.Train(input, targetOutput, 2.1f); 
}

// �۲�ѵ����Ľ��
LNNMatrix output;
bpNetwork.Active(input, &output);

// ʾ�����������Ϊ0.966, 0.966, 0.035, 0.036
*/

#ifndef _LNEURALNETWORK_H_
#define _LNEURALNETWORK_H_

#include <LDataStruct/LMatrix.h>

typedef LMatrix<float> LNNMatrix; // ���������


/// @brief BP��������˽ṹ
/// BP����������һ������, һ�����, һ�����ز�
/// ���ز���������һ����Ԫ
struct LBPNetworkPogology
{
    unsigned int InputNumber; ///< �������, Ҫ����ڵ���1����
    unsigned int OutputNumber; ///< �������, Ҫ����ڵ���1����
    unsigned int HiddenLayerNumber; ///< ���ز����, Ҫ����ڵ���1����
    unsigned int NeuronsOfHiddenLayer; ///< �������ز��е���Ԫ����, Ҫ����ڵ���1����
};

class CBPNetwork;

/// @brief ���򴫲�����(BackPropagation)
class LBPNetwork
{
public:
    /// @brief ���캯��
    /// @param[in] pogology BP�������˽ṹ
    explicit LBPNetwork(IN const LBPNetworkPogology& pogology);

    /// @brief ��������
    ~LBPNetwork();

    /// @brief ѵ��BP����
    /// ����������ù�һ��(����������ȫ������Ϊ0~1֮���ֵ), Ŀ��������ݱ����һ��
    /// @param[in] inputMatrix �������ݾ���, ÿһ��Ϊһ������, ����������������BP������������
    /// @param[in] outputMatrix Ŀ��������ݾ���, ÿһ��Ϊһ��Ŀ�����, ����������������������ݾ��������, �������������������BP������������
    /// @param[in] rate ѧϰ����Ϊ����0����
    /// @return �ɹ�ѵ������true, , ʧ�ܷ���false, ���������������δ��ʼ����ʧ��
    bool Train(IN const LNNMatrix& inputMatrix, IN const LNNMatrix& outputMatrix, IN float rate);

    /// @brief ����������
    /// 
    /// ����������ù�һ��(����������ȫ������Ϊ0~1֮���ֵ), �������Ϊ0~1֮���ֵ
    /// @param[in] inputMatrix �������ݾ���, ÿһ��Ϊһ������, ����������������BP������������
    /// @param[out] pOutputMatrix �洢������ݾ���, ÿһ��Ϊһ�����, ��ֵ����Ϊ0
    /// @return �ɹ�����true, ʧ�ܷ���false, ���������������δ��ʼ����ʧ��
    bool Active(IN const LNNMatrix& inputMatrix, OUT LNNMatrix* pOutputMatrix);

private:
    CBPNetwork* m_pBPNetwork; ///< BP�����ʵ�ֶ���

private:
    // ��ֹ�������캯���͸�ֵ������
    LBPNetwork(const LBPNetwork&);
    LBPNetwork& operator = (const LBPNetwork&);

};


#endif