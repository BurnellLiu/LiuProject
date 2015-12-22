
/// @file LBayesClassifier.h
/// @brief ��Ҷ˹������
/// 
/// Detail: 
/// ��Ҷ˹������Ϊ: ����ģ��, �мල����, ��Ԫ����
/// ���ļ�ʵ�ֵı�Ҷ˹����������������ֵ���ݿ�Ϊ���������ݻ���ɢ������, �����ɻ��ʹ��, ���������ݲ���ͬʱ������ɢ�����ݺ�����������
/// ������������: �˵����, ��ɢ��������: �ı������еĵ�������
/// ��Ҷ˹��������Ҫ���ݱ�Ҷ˹��ʽ: P(Y|X) = P(Y, X)/P(X) = P(X, Y)/P(X) = P(X|Y) * P(Y)/P(X)
/// @author Burnell Liu Email:burnell_liu@outlook.com
/// @version   
/// @date 2015:12:17
/// @sample


/*  ʹ�ñ�Ҷ˹��������ʾ����������:

// ������������
// ÿһ�д���һ����
// ��һ��Ϊ���(Ӣ��), �ڶ���Ϊ����(��), ��3��Ϊ����(Ӣ��)
int dataList[24] = 
{
    600, 180, 12,
    592, 190, 11,
    558, 170, 12,
    592, 165, 10,
    500, 100, 6,
    550, 150, 8,
    542, 130, 7,
    575, 150, 9
};
LBayesMatrix sampleMatrix(8, 3, dataList);

// �����������������
// 1: ��
// 0: Ů
int classList[8] = 
{
    1,
    1,
    1,
    1,
    0,
    0,
    0,
    0
};
LBayesMatrix classVector(8, 1, classList);

// ���屴Ҷ˹ԭʼ����, ָ����������Ϊ����ֵ
LBayesProblem problem(sampleMatrix, classVector, BAYES_FEATURE_CONTINUS);

// ���屴Ҷ˹������, ����ѵ��
LBayesClassifier classifier;
classifier.TrainModel(problem);


// ����������, ��Ԥ�������������
LBayesMatrix newSample(1, 3);
newSample[0][0] = 600; newSample[0][1] = 130; newSample[0][2] = 8;

int predictValue = -1; 
classifier.Predict(newSample, &predictValue);

*/

#ifndef _LBAYESCLASSIFIER_H_
#define _LBAYESCLASSIFIER_H_

#include "LDataStruct/LMatrix.h"

typedef LMatrix<int> LBayesMatrix; ///< ��Ҷ˹����

/// @brief ��Ҷ˹����ֵ�ֲ�
enum LBayesFeatureDistribution
{
    BAYES_FEATURE_DISCRETE = 1, ///< ��ɢ
    BAYES_FEATURE_CONTINUS = 2, ///< ����
};

/// @brief ��Ҷ˹ԭʼ����ṹ
/// ��������Ϊ����ɢ(����)��ʾֵ���Ӹ�˹�ֲ�
struct LBayesProblem
{
    /// @brief ���캯��
    /// @param[in] sampleMatrix ��������, ÿһ��Ϊһ������, ÿ���е�ֵΪ����������ֵ
    /// @param[in] classVector �������(������), ����Ϊ�������������, ����Ϊ1
    /// @param[in] dataType ����ֵ�ֲ�
    LBayesProblem(
        IN const LBayesMatrix& sampleMatrix, 
        IN const LBayesMatrix& classVector, 
        IN LBayesFeatureDistribution distribution)
        : XMatrix(sampleMatrix), YVector(classVector), FeatureDistribution(distribution)
    {
    }

    const LBayesMatrix& XMatrix; ///< ��������
    const LBayesMatrix& YVector; ///< �������(������)
    const LBayesFeatureDistribution FeatureDistribution; ///< ��Ҷ˹����ֵ�ֲ�
};


class CBayesClassifier;

/// @brief ��Ҷ˹�������ӿ���
class LBayesClassifier
{
public:
    /// @brief ���캯��
    LBayesClassifier();

    /// @brief ��������
    ~LBayesClassifier();

    /// @brief ѵ��ģ��
    /// @param[in] problem ��Ҷ˹����
    /// @return �ɹ�����true, ʧ�ܷ���false, �������������»᷵��false
    bool TrainModel(IN const LBayesProblem& problem);

    /// @brief ʹ��ѵ���õ�ģ�ͽ���Ԥ��
    /// �뱣֤��ҪԤ����������������Ⱥ�ѵ������������������ͬ
    /// @param[in] sample ��ҪԤ�������
    /// @param[out] pClassValue �洢Ԥ����, ����Ϊ0
    /// @return �ɹ�Ԥ�ⷵ��true, ʧ�ܷ���false, ���������ģ��δѵ��������»᷵��false
    bool Predict(IN const LBayesMatrix& sample, OUT int* pClassValue);


private:
    CBayesClassifier* m_pBayesClassifier; ///< ��Ҷ˹������ʵ�ֶ���

private:
    // ��ֹ�������캯���͸�ֵ������
    LBayesClassifier(const LBayesClassifier&);
    LBayesClassifier& operator = (const LBayesClassifier&);
};


#endif