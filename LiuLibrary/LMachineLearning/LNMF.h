

#ifndef _LNMF_H_
#define _LNMF_H_

#include "LDataStruct/LMatrix.h"

typedef LMatrix<float> LNMFMatrix;

/// @brief �Ǹ�������ʽ�ֽ�
class LNMF
{
public:
    LNMF();
    virtual ~LNMF();
    
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
    LNMF(const LNMF&);
    LNMF& operator = (const LNMF&);
};

#endif