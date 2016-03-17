/// @file LSVM.h
/// @brief  ֧��������
/// 
/// Detail:
/// @author Burnell_Liu  
/// @version   
/// @date 6:7:2015


#ifndef _LSVM_H_
#define _LSVM_H_

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

typedef LMatrix<float> LSVMMatrix;

/// @brief SVMԭʼ����ṹ
struct LSVMProblem
{
    /// @brief ���캯��
    /// @param[in] sampleMatrix ��������, ÿһ��Ϊһ������, ÿ���е�ֵΪ����������ֵ
    /// @param[in] classVector �������(������), ����Ϊ�������������, ����Ϊ1, ֻ��Ϊ-1.0f��1.0f
    LSVMProblem(IN const LSVMMatrix& sampleMatrix, IN const LSVMMatrix& classVector)
		: XMatrix(sampleMatrix), YVector(classVector)
    {
    }

    const LSVMMatrix& XMatrix; ///< ��������
    const LSVMMatrix& YVector; ///< �������(������)
};

/// @brief �˺����ӿ�
class ISVMKernelFunc
{
public:
    /// @brief ��������
    virtual ~ISVMKernelFunc() = 0{}

    /// @brief ת��
    /// Ҫ������A��B�ĳ�����ͬ, ���Ҷ���������
    /// @param[in] vectorA ����A(������)
    /// @param[in] vectorB ����B(������)
    /// @return ��������A, Bӳ���ڸ�γ�ռ��ϵ��������ڻ�
    virtual float Translate(IN const LSVMMatrix& vectorA, IN const LSVMMatrix& vectorB) const = 0;
};

/// @brief ������˺���
/// Ĭ��gammaֵΪ0.1
class LSVMKRBF : public ISVMKernelFunc
{
public:
    /// @brief ���캯��
    LSVMKRBF();

    /// @brief ��������
    ~LSVMKRBF();

    /// @brief ����gamma����
    /// gammaѡ�ĺ�С����Խ����������ӳ��Ϊ���Կɷ�, ���ǻ��������ϵ�����
    /// gammaѡ�ĺܴ���ߴ�������Ȩ��˥���ĺܿ�
    /// @param[in] gamma
    void SetGamma(IN float gamma);

    /// @brief ת��
    ///  
    /// Ҫ������A��B�ĳ�����ͬ, ���Ҷ���������
    /// @param[in] vectorA ����A(������)
    /// @param[in] vectorB ����B(������)
    /// @return ��������A, Bӳ���ڸ�γ�ռ��ϵ������ڻ�
    virtual float Translate(IN const LSVMMatrix& vectorA, IN const LSVMMatrix& vectorB) const;

private:
    float m_gamma; ///< gamma����
};

class CSVM;

/// @brief ֧��������
class LSVM
{
public:
    /// @brief ���캯��
    LSVM();

    /// @brief ��������
    ~LSVM();

    /// @brief ѵ��ģ��
    /// problem�����е��������������Ҫ�ͱ�ǩ������������ͬ
    /// �ú������ܺܺ�ʱ, ������ĸ��Ӷȶ���
    /// @param[in] problem ԭʼ����
    /// @param[in] pKernelFunc �˺����ӿ�, ���Ϊ����ʹ�ú˺���
    /// @return �ɹ�����true, ʧ�ܷ���false(�������������»᷵��ʧ��)
    bool TrainModel(IN const LSVMProblem& problem, IN const ISVMKernelFunc* pKernelFunc);

    /// @brief ʹ��ѵ���õ�ģ�ͽ���Ԥ��
    /// �뱣֤��ҪԤ����������������Ⱥ�ѵ������������������ͬ
    /// @param[in] sample ��ҪԤ�������
    /// @return ����Ԥ����: -1.0 or 1.0, ����0.0��ʾ����(��ҪԤ��������������ģ��û��ѵ����)
    float Predict(IN const LSVMMatrix& sample);

    /// @brief ��ȡ֧��������
    /// @return ֧����������Ŀ, ����0��ʾģ��û��ѵ����
    unsigned int GetSupportVectorNumber();

private:
    CSVM* m_pSVM; ///< SVMʵ�ֶ���

private:
    // ��ֹ�������캯���͸�ֵ������
    LSVM(const LSVM&);
    LSVM& operator = (const LSVM&);
};

#endif
