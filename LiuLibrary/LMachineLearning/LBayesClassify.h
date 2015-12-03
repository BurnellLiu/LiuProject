/// @file LBayesClassify.h
/// @brief ��Ҷ˹������
/// 
/// Detail:
/// @author Burnell_Liu Email:burnell_liu@qq.com
/// @version   
/// @date 3:12:2015

#ifndef _LBAYESCLASSIFY_H_
#define _LBAYESCLASSIFY_H_

#include "LDataStruct/LMatrix.h"

typedef LMatrix<int> LBayesMatrix;

/// @brief ��Ҷ˹������������
enum LBayesFeatureDataType
{
    BAYES_FEATURE_DISCRETE = 1, //< ��ɢ
    BAYES_FEATURE_NONE_DISCAETE = 2, ///< ����ɢ(����)
};

/// @brief ��Ҷ˹ԭʼ����ṹ
/// ��������Ϊ����ɢ(����)��ʾֵ���Ӹ�˹�ֲ�
struct LBayesProblem
{
    /// @brief ���캯��
    /// @param[in] sampleMatrix ��������, ÿһ��Ϊһ������, ÿ���е�ֵΪ����������ֵ
    /// @param[in] classVector �������(������), ����Ϊ�������������, ����Ϊ1
    /// @param[in] dataType ������������
    LBayesProblem(
        IN const LBayesMatrix& sampleMatrix, 
        IN const LBayesMatrix& classVector, 
        IN LBayesFeatureDataType dataType)
        : XMatrix(sampleMatrix), YVector(classVector), FeatureDataType(dataType)
    {
    }

    const LBayesMatrix& XMatrix; ///< ��������
    const LBayesMatrix& YVector; ///< �������(������)
    const LBayesFeatureDataType FeatureDataType; ///< ��Ҷ˹������������
};


class CBayesClassify;

/// @brief ��Ҷ˹�������ӿ���
class LBayesClassify
{
public:
    LBayesClassify();
    ~LBayesClassify();

    /// @brief ѵ��ģ��
    /// @param[in] problem ��Ҷ˹����
    /// @return �ɹ�����true, ʧ�ܷ���false, �������������»᷵��false
    bool TrainModel(IN const LBayesProblem& problem);

    /// @brief ʹ��ѵ���õ�ģ�ͽ���Ԥ��
    ///  
    /// �뱣֤��ҪԤ����������������Ⱥ�ѵ������������������ͬ
    /// @param[in] sample ��ҪԤ�������
    /// @param[out] classType �洢Ԥ����
    /// @return �ɹ�Ԥ�ⷵ��true, ʧ�ܷ���false, ���������ģ��δѵ��������»᷵��false
    bool Predict(IN const LBayesMatrix& sample, OUT int* classValue);


private:
    CBayesClassify* m_pBayesClassify; ///< ��Ҷ˹������ʵ�ֶ���

private:
    // ��ֹ�������캯���͸�ֵ������
    LBayesClassify(const LBayesClassify&);
    LBayesClassify& operator = (const LBayesClassify&);
};


#endif