
/// @file LBayesClassifier.h
/// @brief ��Ҷ˹������
/// 
/// Detail:
/// ��Ҷ˹��������Ҫ���ݱ�Ҷ˹��ʽ: P(Y|X) = P(Y, X)/P(X) = P(X, Y)/P(X) = P(X|Y) * P(Y)/P(X)
/// @author Burnell Liu Email:burnell_liu@outlook.com
/// @version   
/// @date 2015:12:17
/// @sample


/*  ʹ�ñ�Ҷ˹��������ʾ����������:

// ������������
// ÿһ�д���һ����
// ��һ��Ϊ���(Ӣ��), �ڶ���Ϊ����(��), ��3��Ϊ����(Ӣ��)
LBayesMatrix sampleMatrix(8, 3);
sampleMatrix[0][0] = 600; sampleMatrix[0][1] = 180; sampleMatrix[0][2] = 12;
sampleMatrix[1][0] = 592; sampleMatrix[1][1] = 190; sampleMatrix[1][2] = 11;
sampleMatrix[2][0] = 558; sampleMatrix[2][1] = 170; sampleMatrix[2][2] = 12;
sampleMatrix[3][0] = 592; sampleMatrix[3][1] = 165; sampleMatrix[3][2] = 10;
sampleMatrix[4][0] = 500; sampleMatrix[4][1] = 100; sampleMatrix[4][2] = 6;
sampleMatrix[5][0] = 550; sampleMatrix[5][1] = 150; sampleMatrix[5][2] = 8;
sampleMatrix[6][0] = 542; sampleMatrix[6][1] = 130; sampleMatrix[6][2] = 7;
sampleMatrix[7][0] = 575; sampleMatrix[7][1] = 150; sampleMatrix[7][2] = 9;

// �����������������
// 1: ��
// 0: Ů
LBayesMatrix classVector(8, 1);
classVector[0][0] = 1;
classVector[1][0] = 1;
classVector[2][0] = 1;
classVector[3][0] = 1;
classVector[4][0] = 0;
classVector[5][0] = 0;
classVector[6][0] = 0;
classVector[7][0] = 0;

// ���屴Ҷ˹ԭʼ����
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
    /// @param[out] classType �洢Ԥ����
    /// @return �ɹ�Ԥ�ⷵ��true, ʧ�ܷ���false, ���������ģ��δѵ��������»᷵��false
    bool Predict(IN const LBayesMatrix& sample, OUT int* classValue);


private:
    CBayesClassifier* m_pBayesClassifier; ///< ��Ҷ˹������ʵ�ֶ���

private:
    // ��ֹ�������캯���͸�ֵ������
    LBayesClassifier(const LBayesClassifier&);
    LBayesClassifier& operator = (const LBayesClassifier&);
};


#endif