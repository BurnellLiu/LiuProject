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
    ///  
    /// @param[in] sampleMatrix ��������, ÿһ��Ϊһ������, ÿ���е�ֵΪ����������ֵ
    /// @param[in] classVector �������(������), ����Ϊ�������������, ����Ϊ1, ֻ��Ϊ-1.0f��1.0f
    LSVMProblem(IN const LSVMMatrix& sampleMatrix, IN const LSVMMatrix& classVector)
		: XMatrix(sampleMatrix), YVector(classVector)
    {
    }

    const LSVMMatrix& XMatrix; ///< ��������
    const LSVMMatrix& YVector; ///< �������(������)
};

/// @brief SVM�����ṹ
struct LSVMParam;
/// @brief SVM��ṹ
struct LSVMSolution;

/// @brief �˺���
class LSVMKernelFunc
{
public:
    /// @brief ת��
    ///  
    /// Ҫ������A��B�ĳ�����ͬ, ���Ҷ���������
    /// @param[in] vectorA ����A(������)
    /// @param[in] vectorB ����B(������)
    /// @return ��������A, Bӳ���ڸ�γ�ռ��ϵ��������ڻ�
    virtual float Translate(IN const LSVMMatrix& vectorA, IN const LSVMMatrix& vectorB) const = 0;
};

/// @brief ���������
///
/// Ĭ��gammaֵΪ0.1
class LSVMRBF : public LSVMKernelFunc
{
public:
    LSVMRBF();
    ~LSVMRBF();

    /// @brief ����gamma����
    ///  
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

/// @brief ֧��������
class LSVM
{
public:
    LSVM();
    ~LSVM();
    /// @brief ѵ��ģ��
    ///  
    /// problem�����е��������������Ҫ�ͱ�ǩ������������ͬ
    /// �ú������ܺܺ�ʱ, ������ĸ��Ӷȶ���
    /// @param[in] problem ԭʼ����
    /// @param[in] pKernelFunc �˺���, ���Ϊ����ʹ�ú˺���
    /// @return �ɹ�����true, ʧ�ܷ���false(�������������»᷵��ʧ��)
    bool TrainModel(IN const LSVMProblem& problem, IN const LSVMKernelFunc* pKernelFunc);

    /// @brief ʹ��ѵ���õ�ģ�ͽ���Ԥ��
    ///  
    /// �뱣֤��ҪԤ����������������Ⱥ�ѵ������������������ͬ
    /// @param[in] sample ��ҪԤ�������
    /// @return ����Ԥ����: -1.0 or 1.0, ����0.0��ʾ����(��ҪԤ��������������ģ��û��ѵ����)
    float Predict(IN const LSVMMatrix& sample);


    /// @brief ��ȡ֧��������
    ///  
    /// @return ֧����������Ŀ, ����0��ʾģ��û��ѵ����
    unsigned int GetSupportVectorNumber();

private:

    /// @brief ����һ��������i�������, ��Χ[0, max]
    ///  
    /// @param[in] i ������б����˵�ֵ
    /// @param[in] max ������е����ֵ, Ҫ�����0
    /// @return �����
    int SelectRand(IN unsigned int i, IN unsigned int max);

    /// @brief ʹ������ʽ����ѡ��ڶ���alpha
    ///  
    /// @param[in] i ��һ��alpha����
    /// @param[in] E ���������������
    /// @return �ڶ���alpha����
    unsigned int SelectSecondAlpha(IN unsigned int i, IN const LSVMMatrix& E);

    /// @brief ����aֵ
    ///  aӦ����min~max, ��a > maxʱa = max, ��a < minʱa = min
    /// @param[in]  a ��Ҫ������ֵ
    /// @param[in] min ��Сֵ
    /// @param[in] max ���ֵ
    /// @return �������ֵ
    float ClipAlpha(IN float a, IN float min, IN float max);

    /// @brief ���ݽ������������������
    /// 
    /// @param[in] problem ԭʼ����
    /// @param[in] solution ��
    /// @param[in] k ��������
    /// @return ���ֵ
    void CalculateError(IN const LSVMProblem& problem, IN const LSVMSolution& solution, OUT LSVMMatrix& errorVector);

    /// @brief SMOѵ���㷨(ʾ��)
    ///  
    /// ��ʾ������û��ʹ������ʽ�ķ���ѡ��alpha, Ҳû��ʹ�ú˺���, �ú����������ο���
    /// @param[in] sampleMatrix ��������, ÿһ��Ϊһ������, ÿ���е�ֵΪ����������ֵ
    /// @param[in] classVector �������(������), ����Ϊ�������������, ����Ϊ1, ֻ��Ϊ-1.0f��1.0f
    /// @param[in] C ����C
    /// @param[in] toler �ݴ���
    /// @param[out] b ��ƽ��ؾ�
    /// @param[out] alphaVector alpha����
    void SMOTrainModelExample(
        IN const LSVMMatrix& sampleMatrix, 
        IN const LSVMMatrix& classVector, 
        IN float C, 
        IN float toler, 
        OUT float& b,
        OUT LSVMMatrix& alphaVector);

    /// @brief SMOѵ���㷨
    ///  
    /// @param[in] problem ԭʼ����
    /// @param[in] param ����
    /// @param[out] solution ����Ľ�
    void SMOTrainModel(IN const LSVMProblem& problem, IN const LSVMParam& param, OUT LSVMSolution& solution);

    /// @brief SMO�Ż�Alpha
    ///  
    /// @param[in] fristAlpha ��Ҫ�Ż���alpha������
    /// @param[in] problem ԭʼ����
    /// @param[in] param ����
    /// @param[in] error ��������
    /// @param[out] solution ����Ľ�
    /// @return �ɹ��Ż�����true, �Ż�ʧ�ܷ���false
    bool SMOOptimizeAlpha(
        IN unsigned int fristAlpha, 
        IN const LSVMProblem& problem, 
        IN const LSVMParam& param, 
        IN const LSVMMatrix& error,
        OUT LSVMSolution& solution);

private:
    const LSVMProblem* m_pProblem; ///< ԭʼ����
    LSVMParam* m_pParam; ///< ����
    LSVMSolution* m_pSolution; ///< SVM�Ľ�
    LSVMMatrix* m_pKMatrix; ///< K����
    const LSVMKernelFunc* m_pKernelFunc; ///< �˺���
    bool m_bUseKernelFunc; ///< �Ƿ�ʹ�ú˺����ı�ʶ��
    LSVMMatrix m_supportVectorIndex; ///< ��¼֧������������������(������)
};

#endif
