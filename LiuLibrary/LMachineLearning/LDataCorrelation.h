
/// Author:Burnell_Liu Email: burnell_liu@outlook.com Date: 2016/05/12
/// Description: ���������
/// 
/// ŷ����þ�������
/// Ƥ��ѷ���ϵ������
/// Tanimotoϵ��
/// Others: 
/// Function List: 
///
/// History: 
///  1. Date, Author 
///  Modification
///




#ifndef _LDATACORRELATION_H_
#define _LDATACORRELATION_H_

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

typedef LMatrix<float> LDCVector; // ������

/// @brief ������ض�
class LDataCorrelation
{
public:
    /// @brief ������������A��B�����ƶ�
    /// Ҫ��A������B����������ͬ, ����ÿ�����ݶ�Ӧ�ĺ�����ͬ
    virtual float Calculate(IN const LDCVector& dataVecA, IN const LDCVector& dataVecB) = 0;
};

/// @brief ŷ����þ�������
class LEuclideanDistance : public LDataCorrelation
{
public:
    /// @brief ���������б�A��B�����ƶ�
    /// @return 0~1 ���ݾ���Խ������ֵԽ��, ����ֵΪ1ʱ������ͬ, ����ֵΪ0ʱ��ʾ���ݳ��Ȳ�ͬ
    virtual float Calculate(IN const LDCVector& dataVecA, IN const LDCVector& dataVecB);
};

/// @brief Ƥ��ѷ��ض�����
/// Ƥ��ѷ���ϵ����һ�ֶ���������������س̶ȵķ���,
/// ����һ������ 1 �� -1 ֮���ֵ�����У�1 ��ʾ������ȫ����أ� 0 ��ʾ�޹أ�-1 ��ʾ��ȫ�����
/// �����ŷ����þ�������, ���㷨���㸴��, �����㷨��������"����ֵ�����"
class LPearsonCorrelation : public LDataCorrelation
{
public:
    /// @brief ���������б�A��B����ض�
    /// @return -1~1, 1��ʾ������ȫ�����, 0��ʾ�޹�, -1 ��ʾ��ȫ�����(���ݳ��Ȳ�ͬ����0)
    virtual float Calculate(IN const LDCVector& dataVecA, IN const LDCVector& dataVecB);
};

/// @brief Tanimotoϵ����������
/// ���������ݵ�ָ������ص�����Ķ���
class LTanimotoCoefficient : public LDataCorrelation
{
public:
    LTanimotoCoefficient();
    ~LTanimotoCoefficient();

    /// @brief �����ص�ֵ(Ĭ��1.0)
    /// @param[in] sameValue �ص�ֵ
    void SetSameValue(IN float sameValue);

    /// @brief �����ص����
    /// ����ǰ���������ص�ֵ
    /// @return 0~1, 1��ʾ����������ȫ�ص�, 0��ʾ��������û��ָ�����ص���
    virtual float Calculate(IN const LDCVector& dataVecA, IN const LDCVector& dataVecB);

private:
    float m_sameValue;
};

#endif