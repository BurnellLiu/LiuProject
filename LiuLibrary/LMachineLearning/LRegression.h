/// @file LRegression.h
/// @brief ���ļ���������һЩ�ع��㷨
/// 
/// Detail:
/// ���Իع�
/// �ŵ�: ����������, �����ϲ�����
/// ȱ��: �Է�����������ϲ���
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
    LRegressionProblem(IN const LRegressionMatrix& sampleMatrix, IN const LRegressionMatrix& targetVector)
        : XMatrix(sampleMatrix), YVector(targetVector)
    {
    }

    const LRegressionMatrix& XMatrix; ///< ��������
    const LRegressionMatrix& YVector; ///< Ŀ������(������)
};

/// @brief ���Իع���
/// ���Ժ���Ϊ h(x)  =  X * W
/// WΪ����Ȩ�ص�������, XΪ������������
class LLinearRegression
{
public:
    /// @brief ���캯��
    LLinearRegression();

    /// @brief ��������
    ~LLinearRegression();

    /// @brief ѵ��ģ��
    /// @param[in] problem ԭʼ����
    /// @param[in] learningRate ѧϰ�ٶ�, ѧϰ�ٶȱ������0
    /// @param[in] trainTimes ѵ������
    /// @return �ɹ�����true, ʧ�ܷ���false(�������������»᷵��ʧ��)
    bool TrainModel(
        IN const LRegressionProblem& problem,
        IN float learningRate,
        IN unsigned int trainTimes);

    /// @brief ��ȡȨ������(������)
    /// Ȩ�������ĳ���Ϊ����������Ŀ��1, Ȩ�����������һ��Ϊ������ֵ
    /// @param[out] weightVector
    /// @return ģ��δѵ������false, ���򷵻�true
    bool GetWeightVector(OUT LRegressionMatrix& weightVector);

    /// @brief ��ȡ���ֵ(����ֵ)
    /// @return ģ��δѵ������-1.0f, ���򷵻����ֵ
    float GetErrorValue();

private:
    LRegressionMatrix m_xMatrix; ///< ��������
    LRegressionMatrix m_yVector; ///< Ŀ������(������)
    LRegressionMatrix m_wVector; ///<Ȩ�ؾ���(������)
};

/// @brief �߼��ع��е�ZERO����
#ifndef REGRESSION_ZERO
#define REGRESSION_ZERO 0.0f
#endif

/// @brief �߼��ع��е�ONE����
#ifndef REGRESSION_ONE
#define REGRESSION_ONE 1.0f
#endif

/// @brief �߼��ع�(����)
/// �߼�����Ϊ h(x)  =  1/(1 + e^(X * W)) 
/// WΪ����Ȩ�ص�������, XΪ������������
/// ԭʼ�����е�Ŀ�������е�ֵֻ��Ϊ0.0f��1.0f
/// P(1) = h(x), P(0) = 1-h(x)
class LLogisticRegression
{
public:
    /// @brief ���캯��
    LLogisticRegression();

    /// @brief ��������
    ~LLogisticRegression();

    /// @brief ѵ��ģ��
    /// @param[in] problem ԭʼ����, Ŀ�������е�ֵֻ��Ϊ0.0f��1.0f
    /// @param[in] learningRate ѧϰ�ٶ�, ѧϰ�ٶȱ������0
    /// @param[in] trainTimes ѵ������
    /// @return �ɹ�����true, ʧ�ܷ���false(�������������»᷵��ʧ��)
    bool TrainModel(
        IN const LRegressionProblem& problem,
        IN float learningRate,
        IN unsigned int trainTimes);

    /// @brief ��ȡȨ������(������)
    /// Ȩ�������ĳ���Ϊ����������Ŀ��1, Ȩ�����������һ��Ϊ������ֵ
    /// @param[out] weightVector
    /// @return ģ��δѵ������false, ���򷵻�true
    bool GetWeightVector(OUT LRegressionMatrix& weightVector);

    /// @brief ��ȡ��ǰ������������Ȼֵ
    /// @return ��Ȼֵ, ��Χ(0~1), ��ȻֵԽ���ʾ��������Խ׼ȷ, ģ��δѵ������-1.0f
    float GetLikelihood();

private:
    LRegressionMatrix m_xMatrix; ///< ��������
    LRegressionMatrix m_yVector; ///< Ŀ������(������)
    LRegressionMatrix m_wVector; ///<Ȩ�ؾ���(������)
};

#endif