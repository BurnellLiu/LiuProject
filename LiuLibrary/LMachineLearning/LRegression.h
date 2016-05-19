/// @file LRegression.h
/// @brief ���ļ���������һЩ�ع��㷨
/// ���Իع�, �߼��ع�, Softmax�ع�
/// Detail:
/// ���Իع�:
/// �ŵ�: ����������, �����ϲ�����
/// ȱ��: �Է�����������ϲ���
/// ���Իع�ĸĽ��㷨�оֲ���Ȩ�ع�
/// 
/// �߼��ع�:
/// �ŵ�: ������۲���, ��������ʵ��
/// ȱ��: ����Ƿ���, ���ྫ�ȿ��ܲ���
/// 
/// Softmax�ع�:
/// �ûع����߼��ع��ڶ���������ϵ��ƹ�
///
/// @author Burnell_Liu Email:burnell_liu@outlook.com
/// @version   
/// @date 10:10:2015


/*  ʹ��Softmax�ع�ʾ����������:

// ѵ����������
float trainSample[36] = 
{
    1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
    1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f
};
LRegressionMatrix X(6, 6, trainSample);

// ѵ��������ǩ����
float trainLabel[12] = 
{
    1.0f, 0.0f,
    1.0f, 0.0f,
    1.0f, 0.0f,
    0.0f, 1.0f,
    0.0f, 1.0f,
    0.0f, 1.0f
};
LRegressionMatrix Y(6, 2, trainLabel);

// ����Softmax�ع�
LSoftmaxRegression softmaxReg(6, 6, 2);

// ѵ��500��
for (unsigned int i = 0; i < 500; i++)
{
    softmaxReg.TrainModel(X, Y, 0.1f);   
}

// ��������
float testSample[12] = 
{
    1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f
};
LRegressionMatrix testX(2, 6, testSample);

// �Բ�����������Ԥ��
LRegressionMatrix testY;
softmaxReg.Predict(testX, testY);
*/

#ifndef _LREGRESSION_H_
#define _LREGRESSION_H_


#include "LDataStruct/LMatrix.h"

typedef LMatrix<float> LRegressionMatrix;



/// @brief �ع�ԭʼ����ṹ
struct LRegressionProblem
{
    /// @brief ���캯��
    /// @param[in] sampleMatrix ��������, ÿһ��Ϊһ������, ÿ���е�ֵΪ����������ֵ, ������Ҫ��������
    /// @param[in] targetVector Ŀ������(������), ����Ϊ�������������, ����Ϊ1, ÿ�е�ֵ��������������ÿ��������Ŀ��(���)
    /// @param[in] learningRate ѧϰ�ٶ�(����Ϊ����0����)
    /// @param[in] trainTimes ѵ������
    LRegressionProblem(
        IN const LRegressionMatrix& sampleMatrix, 
        IN const LRegressionMatrix& targetVector,
        IN float learningRate,
        IN unsigned int trainTimes)
        : 
        XMatrix(sampleMatrix), 
        YVector(targetVector), 
        LearningRate(learningRate), 
        TrainTimes(trainTimes)
    {
    }

    const LRegressionMatrix& XMatrix; ///< ��������
    const LRegressionMatrix& YVector; ///< Ŀ������(������)

    float LearningRate; ///< ѧϰ�ٶ�
    unsigned int TrainTimes; ///< ѵ������
};

class CLinearRegression;

/// @brief ���Իع���
class LLinearRegression
{
public:
    /// @brief ���캯��
    LLinearRegression();

    /// @brief ��������
    ~LLinearRegression();

    /// @brief ѵ��ģ��
    /// @param[in] problem ԭʼ����
    /// @return �ɹ�����true, ʧ�ܷ���false(�������������»᷵��ʧ��)
    bool TrainModel(IN const LRegressionProblem& problem);

    /// @brief ʹ��ѵ���õ�ģ��Ԥ������
    /// @param[in] sampleMatrix ��ҪԤ�����������
    /// @param[out] yVector �洢Ԥ��Ľ������(������)
    /// @return �ɹ�����true, ʧ�ܷ���false(ģ��δѵ����������������»᷵��ʧ��)
    bool Predict(IN const LRegressionMatrix& sampleMatrix, OUT LRegressionMatrix& yVector) const;

    /// @brief ѵ����Ԥ��������ѵ�����ݵ���ض�(��ģ�͵����ӳ̶�)
    /// ��ض�Խ�ӽ�1��ʾģ��Խ��
    /// @return ��ض�, ģ��δѵ���򷵻�0
    float GetCorrelation() const;

private:
    CLinearRegression* m_pLinearRegression; ///< ���Իع�ʵ�ֶ���
};

/// @brief �߼��ع��е�ZERO����
#ifndef REGRESSION_ZERO
#define REGRESSION_ZERO 0.0f
#endif

/// @brief �߼��ع��е�ONE����
#ifndef REGRESSION_ONE
#define REGRESSION_ONE 1.0f
#endif

class CLogisticRegression;

/// @brief �߼��ع�(����)
class LLogisticRegression
{
public:
    /// @brief ���캯��
    LLogisticRegression();

    /// @brief ��������
    ~LLogisticRegression();

    /// @brief ѵ��ģ��
    /// @param[in] problem ԭʼ����, Ŀ�������е�ֵֻ��Ϊ0.0f��1.0f
    /// @return �ɹ�����true, ʧ�ܷ���false(�������������»᷵��ʧ��)
    bool TrainModel(IN const LRegressionProblem& problem);

    /// @brief ʹ��ѵ���õ�ģ��Ԥ������
    /// @param[in] sampleMatrix ��ҪԤ�����������
    /// @param[out] yVector �洢Ԥ��Ľ������(������)
    /// @return �ɹ�����true, ʧ�ܷ���false(ģ��δѵ����������������»᷵��ʧ��)
    bool Predict(IN const LRegressionMatrix& sampleMatrix, OUT LRegressionMatrix& yVector) const;

    /// @brief ��ȡ��ǰѵ��ģ�͵���ȷ��
    /// @return ��ȷ��, ģ��δѵ������-1.0f
    float GetAccuracy() const;

private:
    CLogisticRegression* m_pLogisticRegression; ///< �߼��ع�ʵ����
};


class CSoftmaxRegression;

/// @brief Softmax�ع�(�����)
/// ����ݶ��½���ʵ��, �˷�����������̫��, ���µ��ڴ濪�����Լ�ѵ����ʱ��������
class LSoftmaxRegression
{
public:
    /// @brief ���캯��
    /// @param[in] m �����ܸ���, ����С��2
    /// @param[in] n ��������ֵ����, ����С��1
    /// @param[in] k ����������, ����С��2
    LSoftmaxRegression(IN unsigned int m, IN unsigned int n, IN unsigned int k);

    /// @brief ��������
    ~LSoftmaxRegression();

    /// @brief ѵ��ģ��
    /// ���ѵ������������Ϊ1, ��Ϊ����ݶ��½�
    /// ���ѵ������������ΪM(��������), ��Ϊ�ݶ��½�
    /// ���ѵ������������Ϊm(1 < m < M), ��Ϊ�����ݶ��½�
    /// @param[in] xMatrix ��������, ÿһ�д���һ������, ÿһ�д���������һ������
    /// @param[in] yMatrix ���Ǿ���, ÿһ�д���һ������, ÿһ�д���������һ�����, 
    /// ����������ڸ��������ΪREGRESSION_ONE, ����������ΪREGRESSION_ZERO
    /// @param[in] alpha ѧϰ�ٶ�, ��ֵ�������0.0f
    /// @return �ɹ�����true, ʧ�ܷ���false(�������������»᷵��ʧ��)
    bool TrainModel(IN const LRegressionMatrix& xMatrix, IN const LRegressionMatrix& yMatrix, IN float alpha);

    /// @brief ʹ��ѵ���õ�ģ��Ԥ������
    /// @param[in] xMatrix ��ҪԤ�����������
    /// @param[out] yMatrix �洢Ԥ��Ľ������, ÿһ�д���һ������, ÿһ�д����ڸ�����µĸ���
    /// @return �ɹ�����true, ʧ�ܷ���false(�������������»᷵��ʧ��)
    bool Predict(IN const LRegressionMatrix& xMatrix, OUT LRegressionMatrix& yMatrix) const;

private:
    CSoftmaxRegression* m_pSoftmaxRegression; ///< Softmax�ع�ʵ�ֶ���
};

#endif