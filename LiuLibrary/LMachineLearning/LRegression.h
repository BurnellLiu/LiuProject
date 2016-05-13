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
/// @author Burnell_Liu Email:burnell_liu@outlook.com
/// @version   
/// @date 10:10:2015

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

/// @brief Softmax�ع�(�����)
class LSoftmaxRegression
{
public:
    /// @brief ���캯��
    LSoftmaxRegression();

    /// @brief ��������
    ~LSoftmaxRegression();

    /// @brief ѵ��ģ��
    /// @param[in] problem ԭʼ����, Ŀ�������е�ֵֻ��Ϊ[1.0f, 2.0f, ..., k.0f]
    /// @param[in] k �������������
    /// @param[in] learningRate ѧϰ�ٶ�, ѧϰ�ٶȱ������0
    /// @param[in] trainTimes ѵ������
    /// @return �ɹ�����true, ʧ�ܷ���false(�������������»᷵��ʧ��)
    bool TrainModel(
        IN const LRegressionProblem& problem,
        IN unsigned int k,
        IN float learningRate,
        IN unsigned int trainTimes);

private:
    LRegressionMatrix m_xMatrix; ///< ��������
    LRegressionMatrix m_yVector; ///< Ŀ������(������)
    LRegressionMatrix m_wMatrix; ///<Ȩ�ؾ���
};

#endif