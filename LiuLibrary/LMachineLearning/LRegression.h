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
/// �ݶ��½��㷨: ÿ��ѵ��ʹ������������, ����������ܴ�, �����ڴ濪����, ����ѵ����ʱ��, �ŵ���������
/// ����ݶ��½��㷨: ÿ��ѵ��ʹ���������е�һ������, ȱ����������
/// �����ݶ��½��㷨: �ۺ���������
/// @author Burnell_Liu Email:burnell_liu@outlook.com
/// @version   
/// @date 10:10:2015

/*  ʹ�����Իع�ʾ����������:

// ����ѵ������
float trainX[4] =
{
    2.0f,
    4.0f,
    6.0f,
    8.0f
};
LRegressionMatrix xMatrix(4, 1, trainX);

// ����ѵ���������
float trainY[4] =
{
    1.0f,
    2.0f,
    3.0f,
    4.0f
};
LRegressionMatrix yMatrix(4, 1, trainY);


// �������Իع����
LLinearRegression linearReg(4, 1);

// ѵ��ģ��
for (unsigned int i = 0; i < 500; i++)
{
    linearReg.TrainModel(xMatrix, yMatrix, 0.01f);
}

// ����Ԥ��
LRegressionMatrix yVector;
linearReg.Predict(xMatrix, yVector);

*/


/*  ʹ���߼��ع�ʾ����������:

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
    1.0f,
    1.0f,
    1.0f,
    0.0f,
    0.0f,
    0.0f
};
LRegressionMatrix Y(6, 1, trainLabel);

// �����߼��ع�
LLogisticRegression logisticReg(6, 6);

// ѵ��500��
for (unsigned int i = 0; i < 500; i++)
{
    logisticReg.TrainModel(X, Y, 0.1f);
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
logisticReg.Predict(testX, testY);
*/


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

class CLinearRegression;

/// @brief ���Իع���
class LLinearRegression
{
public:
    /// @brief ���캯��
    /// @param[in] m �����ܸ���, ����С��2
    /// @param[in] n ��������ֵ����, ����С��1
    LLinearRegression(IN unsigned int m, IN unsigned int n);

    /// @brief ��������
    ~LLinearRegression();

    /// @brief ѵ��ģ��
    /// ���һ��ѵ������������Ϊ1, ��Ϊ����ݶ��½�
    /// ���һ��ѵ������������ΪM(��������), ��Ϊ�ݶ��½�
    /// ���һ��ѵ������������Ϊm(1 < m < M), ��Ϊ�����ݶ��½�
    /// @param[in] xMatrix ��������, ÿһ�д���һ������, ÿһ�д���������һ������
    /// @param[in] yVector(������) �����������, ÿһ�д���һ������
    /// @param[in] alpha ѧϰ�ٶ�, ��ֵ�������0.0f
    /// @return �ɹ�����true, ʧ�ܷ���false(�������������»᷵��ʧ��)
    bool TrainModel(IN const LRegressionMatrix& xMatrix, IN const LRegressionMatrix& yVector, IN float alpha);

    /// @brief ʹ��ѵ���õ�ģ��Ԥ������
    /// @param[in] xMatrix ��ҪԤ�����������
    /// @param[out] yVector �洢Ԥ��Ľ������(������)
    /// @return �ɹ�����true, ʧ�ܷ���false(ģ��δѵ����������������»᷵��ʧ��)
    bool Predict(IN const LRegressionMatrix& xMatrix, OUT LRegressionMatrix& yVector) const;

private:
    CLinearRegression* m_pLinearRegression; ///< ���Իع�ʵ�ֶ���
};

/// @brief �ع��е�ZERO����
#ifndef REGRESSION_ZERO
#define REGRESSION_ZERO 0.0f
#endif

/// @brief �ع��е�ONE����
#ifndef REGRESSION_ONE
#define REGRESSION_ONE 1.0f
#endif

class CLogisticRegression;

/// @brief �߼��ع�(����)
class LLogisticRegression
{
public:
    /// @brief ���캯��
    /// @param[in] m �����ܸ���, ����С��2
    /// @param[in] n ��������ֵ����, ����С��1
    LLogisticRegression(IN unsigned int m, IN unsigned int n);

    /// @brief ��������
    ~LLogisticRegression();

    /// @brief ѵ��ģ��
    /// ���һ��ѵ������������Ϊ1, ��Ϊ����ݶ��½�
    /// ���һ��ѵ������������ΪM(��������), ��Ϊ�ݶ��½�
    /// ���һ��ѵ������������Ϊm(1 < m < M), ��Ϊ�����ݶ��½�
    /// @param[in] xMatrix ��������, ÿһ�д���һ������, ÿһ�д���������һ������
    /// @param[in] yVector(������) �����������, ÿһ�д���һ������, ֵֻ��ΪREGRESSION_ONE��REGRESSION_ZERO 
    /// @param[in] alpha ѧϰ�ٶ�, ��ֵ�������0.0f
    /// @return �ɹ�����true, ʧ�ܷ���false(�������������»᷵��ʧ��)
    bool TrainModel(IN const LRegressionMatrix& xMatrix, IN const LRegressionMatrix& yVector, IN float alpha);

    /// @brief ʹ��ѵ���õ�ģ��Ԥ������
    /// @param[in] xMatrix ��ҪԤ�����������
    /// @param[out] yVector �洢Ԥ��Ľ������(������), ֵΪREGRESSION_ONE��ǵĸ���
    /// @return �ɹ�����true, ʧ�ܷ���false(ģ��δѵ����������������»᷵��ʧ��)
    bool Predict(IN const LRegressionMatrix& xMatrix, OUT LRegressionMatrix& yVector) const;

private:
    CLogisticRegression* m_pLogisticRegression; ///< �߼��ع�ʵ����
};


class CSoftmaxRegression;

/// @brief Softmax�ع�(�����)
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
    /// ���һ��ѵ������������Ϊ1, ��Ϊ����ݶ��½�
    /// ���һ��ѵ������������ΪM(��������), ��Ϊ�ݶ��½�
    /// ���һ��ѵ������������Ϊm(1 < m < M), ��Ϊ�����ݶ��½�
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