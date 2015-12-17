
/// @file LNMF.h
/// @brief �Ǹ�������ʽ�ֽ�
/// ��һ���Ǹ�����ֽ�Ϊ�����Ǹ�����ĳ�, V = W * H
/// VΪԭʼ����, WΪ������, HΪϵ������
/// �������������ϵ�����������ΪR
/// @author Burnell Liu Email:burnell_liu@outlook.com
/// @version   
/// @date 2015:12:17

#ifndef _LNMF_H_
#define _LNMF_H_

#include "LDataStruct/LMatrix.h"

typedef LMatrix<float> LNMFMatrix; ///< NMF����

/// @brief NMF����ṹ
struct LNMFProblem
{
    /// @brief ���캯��
    /// @param[in] v ԭʼ����, ԭʼ�����в����и���
    /// @param[in] r �����������(ϵ�����������)
    /// @param[in] iterCount ��������
    LNMFProblem(IN const LNMFMatrix& v, IN unsigned int r, IN unsigned int iterCount)
        : V(v), R(r), IterCount(iterCount)
    {

    }

    const LNMFMatrix& V; ///< ԭʼ����
    const unsigned int R; ///< �����������(ϵ�����������)
    const unsigned int IterCount; ///< ��������
};

/// @brief �Ǹ�������ʽ�ֽ�
class LNMF
{
public:
    /// @brief ���캯��
    LNMF();

    /// @brief ��������
    ~LNMF();

    /// @brief ��ʽ�ֽ�
    /// @param[in] problem ԭʼ����, ԭʼ�����в����и���
    /// @param[out] pW �洢�ֽ��Ļ�����, ����Ϊ0
    /// @param[out] pH �洢�ֽ���ϵ������, ����Ϊ0
    /// @return �ɹ�����true, ʧ�ܷ���false, ���������ʧ��
    bool Factoring(IN const LNMFProblem& problem, OUT LNMFMatrix* pW, OUT LNMFMatrix* pH);
};

/// @brief �Ǹ�������ʽ�ֽ�
class CNMF
{
public:
    CNMF();
    virtual ~CNMF();
    
    /// @brief ������������
    /// @param[in] num ��������(Ҫ�����0������)
    /// @return �������󷵻�false
    bool SetFeatureNum(int num);

    /// @brief ����ԭʼ����
    /// @param[in] matrix
    /// @return true
    bool SetPrimitiveMatrix(const LNMFMatrix& primitiveMatrix);

    /// @brief ��ʼ��
    /// @return δ�Բ���������ȷ���ô��󷵻�false
    bool Init();

    /// @brief ��ʽ�ֽ�һ��
    /// @return δ��ʼ������false
    bool FactoringOnce();

    /// @brief ��ȡȨ�ؾ���
    /// @param[out] weightedMatrix
    /// @return δ��ʼ������false
    bool GetWeightedMatrix( LNMFMatrix& weightedMatrix) const;

    /// @brief ��ȡ��������
    /// @param[out] featureMatrix
    /// @return δ��ʼ������false
    bool GetFeatureMatrix( LNMFMatrix& featureMatrix) const;

private:
    /// @brief �����������е�ֵ(0~10֮��ĸ�����)
    /// @param[inout] matrix
    void RandMatrix(LNMFMatrix& matrix);

    /// @brief �Ծ������ϵ����ݽ��й�һ��
    /// @param[in] matrix
    void NomalMatrixC(LNMFMatrix& matrix);

private:
    bool m_bNeedInit; ///< ��Ҫ��ʼ��
    int m_featureNum; ///< ������Ŀ
    LNMFMatrix m_primitiveMatrix; ///< ��ʼ����
    LNMFMatrix m_weightedMatrix; ///< Ȩ�ؾ���
    LNMFMatrix m_featureMatrix; ///< ��������

    LNMFMatrix TF;
    LNMFMatrix TW;
    LNMFMatrix TWW;

    LNMFMatrix FN;
    LNMFMatrix FD;
    LNMFMatrix FFN;

    LNMFMatrix WN;
    LNMFMatrix WD;
    LNMFMatrix WF;
    LNMFMatrix WWN;

private:
    CNMF(const CNMF&);
    CNMF& operator = (const CNMF&);
};

#endif