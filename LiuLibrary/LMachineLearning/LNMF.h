
/// @file LNMF.h
/// @brief �Ǹ�������ʽ�ֽ�
/// ��һ���Ǹ�����ֽ�Ϊ�����Ǹ�����ĳ�, V = W * H
/// VΪԭʼ����, WΪ������, HΪϵ������
/// �������������ϵ�����������ΪR
/// @author Burnell Liu Email:burnell_liu@outlook.com
/// @version   
/// @date 2015:12:17
/// @sample

/*  ʹ��NMF��ʾ����������

// ����ԭʼ����
float dataList[4] = 
{
    1.0f, 2.0f,
    3.0f, 4.0f
};
LNMFMatrix V(2, 2, dataList);

// ����ԭʼ����, ����RΪ2, ��������Ϊ50
LNMFProblem problem(V, 2, 50);

// ������ʽ�ֽ�
LNMFMatrix W;
LNMFMatrix H;

LNMF nmf;
nmf.Factoring(problem, &W, &H);
*/

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

#endif