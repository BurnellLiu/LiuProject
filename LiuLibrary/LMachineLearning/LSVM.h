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


/// @brief �˺����ӿ�
class ISVMKernelFunc
{
public:
	/// @brief ��������
	virtual ~ISVMKernelFunc() = 0 {}

	/// @brief ת��
	/// Ҫ������A��B�ĳ�����ͬ, ���Ҷ���������
	/// @param[in] vectorA ����A(������)
	/// @param[in] vectorB ����B(������)
	/// @return ��������A, Bӳ���ڸ�γ�ռ��ϵ��������ڻ�
	virtual float Translate(IN const LSVMMatrix& vectorA, IN const LSVMMatrix& vectorB) = 0;
};

/// @brief ������˺���
/// Ĭ��gammaֵΪ0.1
class LSVMKRBF : public ISVMKernelFunc
{
public:
	/// @brief ���캯��
	/// gammaѡ�ĺ�С����Խ����������ӳ��Ϊ���Կɷ�, ���ǻ��������ϵ�����
	/// gammaѡ�ĺܴ���ߴ�������Ȩ��˥���ĺܿ�
	/// @param[in] gamma ����
	explicit LSVMKRBF(IN float gamma);

	/// @brief ��������
	~LSVMKRBF();

	/// @brief ת��
	/// Ҫ������A��B�ĳ�����ͬ, ���Ҷ���������
	/// @param[in] vectorA ����A(������)
	/// @param[in] vectorB ����B(������)
	/// @return ��������A, Bӳ���ڸ�γ�ռ��ϵ������ڻ�
	virtual float Translate(IN const LSVMMatrix& vectorA, IN const LSVMMatrix& vectorB);

private:
	float m_gamma; ///< gamma����

    /*
	���±�������Ϊ��Ա����Ϊ�Ż�����Ч��Ŀ��
    */
	LSVMMatrix m_deltaRow;
	LSVMMatrix m_deltaRowT;
	LSVMMatrix m_k;
};

/// @brief SVM�����ṹ
struct LSVMParam
{
	float C; ///< ����C, CԽ�����ģ�Ͷ���Ⱥ��Խ����(��Խ���C����Խ��ϣ������Ⱥ��)
	unsigned int MaxIterCount; ///< ����������
	ISVMKernelFunc* PKernelFunc; ///< �˺����ӿ�ָ��, ���Ϊ����ʹ�ú˺���
};

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

/// @brief SVM���
struct LSVMResult
{
	unsigned int IterCount; ///< ��������
	unsigned int SupportVevtorNum; ///< ֧��������
};


class CSVM;

/// @brief ֧��������
class LSVM
{
public:
    /// @brief ���캯��
    explicit LSVM(IN const LSVMParam& param);

    /// @brief ��������
    ~LSVM();

    /// @brief ѵ��ģ��
    /// problem�����е��������������Ҫ�ͱ�ǩ������������ͬ
    /// �ú������ܺܺ�ʱ, ������ĸ��Ӷȶ���
    /// @param[in] problem ԭʼ����
	/// @param[result] result �洢��� 
    /// @return �ɹ�����true, ʧ�ܷ���false(�������������»᷵��ʧ��)
    bool TrainModel(IN const LSVMProblem& problem, OUT LSVMResult& result);

    /// @brief ʹ��ѵ���õ�ģ�ͽ���Ԥ��
    /// �뱣֤��ҪԤ����������������Ⱥ�ѵ������������������ͬ
    /// @param[in] sampleSet ��ҪԤ���������
	/// @param[out] yVector �洢Ԥ��Ľ������(������), ֵΪ-1.0 or 1.0
	/// @return �ɹ�����true, ʧ�ܷ���false(ģ��δѵ����������������»᷵��ʧ��)
    bool Predict(IN const LSVMMatrix& sampleSet, OUT LSVMMatrix& yVector);

private:
    CSVM* m_pSVM; ///< SVMʵ�ֶ���

private:
    // ��ֹ�������캯���͸�ֵ������
    LSVM(const LSVM&);
    LSVM& operator = (const LSVM&);
};

#endif
