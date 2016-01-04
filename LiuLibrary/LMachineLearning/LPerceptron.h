
/// @file LPerceptron.h
/// @brief ��֪��
/// 
/// Detail:
/// ��֪��Ϊ: �б�ģ��, �мලѧϰ, ��Ԫ����(Ҫ��ѵ���������Կɷ�)
/// @author Burnell_Liu  Email:burnell_liu@outlook.com
/// @version   
/// @date 30:7:2015

/* ʹ�ø�֪�����OR����ʾ����������, OR����Ϊ��������

// ����ѵ�����������������
float inputList[8] = 
{
    1.0f, 1.0f,
    1.0f, 0.0f,
    0.0f, 1.0f,
    0.0f, 0.0f
};

LPerceptronMatrix X(4, 2, inputList);

LPerceptronMatrix Y(4, 1);
Y[0][0] = LPERCEPTRON_SUN;
Y[1][0] = LPERCEPTRON_SUN;
Y[2][0] = LPERCEPTRON_SUN;
Y[3][0] = LPERCEPTRON_MOON;

LPerceptronProblem problem(X, Y);

// ѵ��ģ��
LPerceptron perceptron;
perceptron.TrainModel(problem);

// ʹ�ò�����������
LPerceptronMatrix testSample(1, 2);
testSample[0][0] = 0.0f;
testSample[0][1] = 0.0f;

printf("Predict: %f\n", perceptron.Predict(testSample));

*/

#ifndef _LPERCEPTRON_H_
#define _LPERCEPTRON_H_

#include "LDataStruct/LMatrix.h"

#ifndef IN
#define IN
#endif

#ifndef INOUT
#define INOUT
#endif

#ifndef OUT
#define OUT
#endif

/// @brief ��Ԫ�����е�һ��: ��(��), ȡ̫������֮��
#ifndef LPERCEPTRON_MOON
#define LPERCEPTRON_MOON -1.0f
#endif

/// @brief ��Ԫ�����е�һ��: ��(��), ȡ̫������֮��
#ifndef LPERCEPTRON_SUN
#define LPERCEPTRON_SUN 1.0f
#endif

typedef LMatrix<float> LPerceptronMatrix;

/// @brief ��֪��ԭʼ����ṹ
struct LPerceptronProblem
{
    /// @brief ���캯��
    ///  
    /// @param[in] sampleMatrix ѵ����������, ÿһ��Ϊһ������, ÿ���е�ֵΪ����������ֵ
    /// @param[in] classVector �������(������), ����Ϊ�������������, ����Ϊ1, ֻ��ΪLPERCEPTRON_MOON��LPERCEPTRON_SUN
    LPerceptronProblem(IN const LPerceptronMatrix& sampleMatrix, IN const LPerceptronMatrix& classVector)
        : XMatrix(sampleMatrix), YVector(classVector)
    {
    }

    const LPerceptronMatrix& XMatrix; ///< ѵ����������
    const LPerceptronMatrix& YVector; ///< ��ǩ����(������)
};


class CPerceptron;

/// @brief ��֪��
class LPerceptron
{
public:
    /// @brief ���캯��
    LPerceptron();

    /// @brief ��������
    ~LPerceptron();

    /// @brief ����ѧϰ����(Ĭ��ֵΪ1.0f)
    /// @param[in] rate ѧϰ����, ѧϰ����Ϊ����0����
    /// @return �ɹ����÷���true, ����ʧ�ܷ���false, ���������ʧ��
    bool SetLearningRate(IN float rate);

    /// @brief ѵ��ģ��
    /// @param[in] problem ԭʼ����
    /// @return ����true��ʾѵ���ɹ�, ����false��ʾ�������ݴ���
    bool TrainModel(IN const LPerceptronProblem& problem);

    /// @brief ʹ��ѵ���õ�ģ�ͽ���Ԥ��(������Ԥ��)
    /// �뱣֤��ҪԤ����������������Ⱥ�ѵ������������������ͬ
    /// @param[in] sample ��ҪԤ�������(������)
    /// @return ����Ԥ����: LPERCEPTRON_SUN or LPERCEPTRON_MOON, ����0.0��ʾ����(��ҪԤ��������������ģ��û��ѵ����)
    float Predict(IN const LPerceptronMatrix& sample);

private:
    CPerceptron* m_pPerceptron; ///< ��֪��ʵ�ֶ���

private:
    // ��ֹ�������캯���͸�ֵ������
    LPerceptron(const LPerceptron&);
    LPerceptron& operator = (const LPerceptron&);
};



#endif