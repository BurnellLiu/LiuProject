
/// Author:Burnell_Liu Email:674288799@qq.com Date: 2014/08/07
/// Description: ���������
/// 
/// ŷ����þ�������
/// Ƥ��ѷ���ϵ������
/// Others: 
/// Function List: 
///
/// History: 
///  1. Date, Author 
///  Modification
///




#ifndef _LDATACORRETATION_H_
#define _LDATACORRETATION_H_

#include "LDataStruct/include/LArray.h"

#ifndef IN
#define IN
#endif

#ifndef INOUT
#define INOUT
#endif

#ifndef OUT
#define OUT
#endif

typedef LArray<float> LDSDataList;

/// @brief �������ƶ�
class LDataSimilar
{
public:
    /// @brief ���������б�A��B�����ƶ�
    ///
    /// Ҫ��A�б��B�б�����ͬ, ����ÿ�����ݶ�Ӧ�ĺ�����ͬ
    virtual float Calculate(IN const LDSDataList& dataListA, IN const LDSDataList& dataListB) = 0;
};

/// @brief ŷ����þ�������
class LEuclideanDistance : public LDataSimilar
{
public:
    /// @brief ���������б�A��B�����ƶ�
    ///
    /// Ҫ��A�б��B�б�����ͬ, ����ÿ�����ݶ�Ӧ�ĺ�����ͬ
    /// @param[in] dataListA
    /// @param[in] dataListB
    /// @return 0~1 ���ݾ���Խ������ֵԽ��, ����ֵΪ1ʱ������ͬ, ����ֵΪ0ʱ��ʾ���ݳ��Ȳ�ͬ
    virtual float Calculate(IN const LDSDataList& dataListA, IN const LDSDataList& dataListB);
};

/// @brief Ƥ��ѷ��ض�����
///
/// Ƥ��ѷ���ϵ����һ�ֶ���������������س̶ȵķ���,
/// ����һ������ 1 �� -1 ֮���ֵ�����У�1 ��ʾ������ȫ����أ� 0 ��ʾ�޹أ�-1 ��ʾ��ȫ�����
/// �����ŷ����þ�������, ���㷨���㸴��, �����㷨��������"����ֵ�����"
class LPearsonCorretation : public LDataSimilar
{
public:
    /// @brief ���������б�A��B����ض�
    ///
    /// Ҫ��A�б��B�б�����ͬ, ����ÿ�����ݶ�Ӧ�ĺ�����ͬ
    /// @param[in] dataListA
    /// @param[in] dataListB
    /// @return -1~1, 1��ʾ������ȫ�����, 0��ʾ�޹�, -1 ��ʾ��ȫ�����(���ݳ��Ȳ�ͬ����0)
    virtual float Calculate(IN const LDSDataList& dataListA, IN const LDSDataList& dataListB);
};

/// @brief Tanimotoϵ����������
///
/// ���������ݵ�ָ������ص�����Ķ���
class LTanimotoCoefficient : public LDataSimilar
{
public:
    LTanimotoCoefficient();
    ~LTanimotoCoefficient();

    /// @brief �����ص�ֵ(Ĭ��1.0)
    /// @param[in] sameValue �ص�ֵ
    void SetSameValue(IN float sameValue);

    /// @brief �����ص����
    ///
    /// ����ǰ���������ص�ֵ
    /// @param[in] dataListA
    /// @param[in] dataListB
    /// @return 0~1, 1��ʾ����������ȫ�ص�, 0��ʾ��������û��ָ�����ص���
    virtual float Calculate(IN const LDSDataList& dataListA, IN const LDSDataList& dataListB);

private:
    float m_sameValue;
};

#endif