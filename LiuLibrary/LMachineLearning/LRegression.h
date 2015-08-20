
/// Author:Burnell_Liu Email:674288799@qq.com Date: 2015/03/03
/// Description: �ع����
/// 
/// ���Իع����
/// �߼��ع����(����)
/// Others: 
/// Function List: 
///
/// History: 
///  1. Date, Author 
///  Modification
///

#ifndef _LREGRESSION_H_
#define _LREGRESSION_H_

#include "LDataStruct/LArray.h"

typedef float LRType;

typedef LArray<LRType> LRFeatureList; ///< �����б�(��������б�)

/// @brief ѵ������
struct LRTrainingSample
{
    LRFeatureList FeatureList; ///< �����б�(��������б�)
    LRType Target; ///< Ŀ��(�������)
};

typedef LArray<LRTrainingSample> LRTrainingSet; ///< ѵ������

typedef LArray<LRType> LRFeatureParamList; ///< ���������б�(������������б�)

/// @brief ���Իع���
///
/// Detailed: ���Ժ���Ϊ h(x)  =  ��0 * x0 + ��1 * x1 + ��2 * x2 + ��3 * x3 + ... = �� * X
/// ����xiΪ��i����������, ��iΪ��i����������, x0Ϊ1
/// ��Ϊ����������������, XΪ������������
class LLinearRegression
{
public:
    /// @brief ���캯��
    /// @param[in] trainingSet ѵ������, Ҫ��ѵ�������е�����ѵ��������������Ŀ��ͬ
    /// @param[in ] learningRate ѧϰ�ٶ�, ѧϰ�ٶ�Ҫ��Ϊ����0����
    LLinearRegression(IN const LRTrainingSet& trainingSet, IN LRType learningRate);
    ~LLinearRegression();

    /// @brief ���ݶ��½�һ��
    /// @return ����ѵ����������ƽ�����ܺ�, �������󷵻�-1.0
    LRType BatchDescend();

    /// @brief ����Ԥ��
    /// @param[in] featureList ��ҪԤ�������ֵ�б�
    /// @return Ԥ����Ľ��
    LRType ForeCast(IN const LRFeatureList& featureList);

    /// @brief ��ȡ��ǰ���������Ķ���ֵ
    /// @return ����ֵ, ����ֵΪ����0����, ����ֵԽС��ʾ��������Խ׼ȷ
    LRType GetSquareValue();

    /// @brief ��ȡ�����Ĳ���
    /// @param[out] paramList ���������б�, �����б�ĳ���Ϊѵ�����������ĳ��ȼ�1(���������б�����һ��Ϊ�������ֵ)
    void GetFeatureParam(OUT LRFeatureParamList& paramList);

private:
    const LRTrainingSet& m_trainingSet; ///< ѵ������
    LRType m_learningRate; ///< ѧϰ�ٶ� 
    LRFeatureParamList m_paramList; ///< ���������б�
};

/// @brief �߼��ع�(����)
///
/// Detailed: �߼�����Ϊ h(x)  =  1/(1 + e**(-�� * X)) = 1/(1 + e**(-��0 * x0 - ��1 * x1 - ��2 * x2 - ��3 * x3 - ...))
/// ����xiΪ��i����������, ��iΪ��i����������, x0Ϊ1
/// ��Ϊ����������������, XΪ������������
class LLogisticRegression
{
public:
    /// @brief ��Ԫ����
    enum LRBINARY_CLASS
    {
        ZERO = 0, ///< 0
        ONE ///< 1
    };
    /// @brief ���캯��
    /// @param[in] trainingSet ѵ������, Ҫ��ѵ�������е�����ѵ��������������Ŀ��ͬ, ��ѵ��������Targetֵֻ��ΪLRBINARY_CLASS(��Ԫ����)��ֵ
    /// @param[in ] learningRate ѧϰ�ٶ�, ѧϰ�ٶ�Ҫ��Ϊ����0����
    LLogisticRegression(IN const LRTrainingSet& trainingSet, IN LRType learningRate);
    ~LLogisticRegression();

    /// @brief ���ݶ�����һ��
    void BatchAscend();

    /// @brief ����Ԥ��
    /// @param[in] featureList ��ҪԤ�������ֵ�б�
    /// @param[in] bClass ָ���Ľ��
    /// @return ָ����������ĸ���, ��Χ(0~1), �������󷵻�-1.0f
    LRType ForeCast(IN const LRFeatureList& featureList, IN LRBINARY_CLASS bClass);

    /// @brief ��ȡ��ǰ������������Ȼֵ
    /// @return ��Ȼֵ, ��Χ(0~1), ��ȻֵԽ���ʾ��������Խ׼ȷ
    LRType GetLikelihood();

    /// @brief ��ȡ�����Ĳ���
    /// @param[out] paramList ���������б�, �����б�ĳ���Ϊѵ�����������ĳ��ȼ�1(���������б�����һ��Ϊ�������ֵ)
    void GetFeatureParam(OUT LRFeatureParamList& paramList);

private:
    const LRTrainingSet& m_trainingSet; ///< ѵ������
    LRType m_learningRate; ///< ѧϰ�ٶ� 
    LRFeatureParamList m_paramList; ///< ���������б�
};

#endif

