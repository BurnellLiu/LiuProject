/// @file LRegression.h
/// @brief �ع����
/// 
/// Detail:
/// @author Burnell_Liu Email:burnell_liu@qq.com
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
///
/// Detailed: ���Ժ���Ϊ h(x)  =  ��0 * x0 + ��1 * x1 + ��2 * x2 + ��3 * x3 + ... =  X * ��
/// ����xiΪ��i����������, ��iΪ��i����������, x0Ϊ1
/// ��Ϊ����������������, XΪ������������
class LLinearRegression
{
public:
    LLinearRegression();
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
    ///  Ȩ�����������һ��Ϊ������
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

#endif