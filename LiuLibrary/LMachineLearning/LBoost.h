/// @file LBoost.h
/// @brief �����㷨
/// 
/// Detail:
/// @author Burnell_Liu  
/// @version   
/// @date 22:7:2015


#ifndef _LBOOST_H_
#define _LBOOST_H_

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
#ifndef LBOOST_MOON
#define LBOOST_MOON -1.0f
#endif

/// @brief ��Ԫ�����е�һ��: ��(��), ȡ̫������֮��
#ifndef LBOOST_SUN
#define LBOOST_SUN 1.0f
#endif

typedef LMatrix<float> LBoostMatrix;
struct LBoostStump;

/// @brief Boostԭʼ����ṹ
struct LBoostProblem
{
    /// @brief ���캯��
    ///  
    /// @param[in] sampleMatrix ��������, ÿһ��Ϊһ������, ÿ���е�ֵΪ����������ֵ
    /// @param[in] classVector �������(������), ����Ϊ�������������, ����Ϊ1, ֻ��ΪBOOST_MOON��BOOST_SUN
    LBoostProblem(IN const LBoostMatrix& sampleMatrix, IN const LBoostMatrix& classVector)
        : XMatrix(sampleMatrix), YVector(classVector)
    {
    }

    const LBoostMatrix& XMatrix; ///< ��������
    const LBoostMatrix& YVector; ///< ��ǩ����(������)
};

/// @brief ������
///
/// �Ծ�����Ϊ������������������Ϊ������
class LBoostTree
{
public:
    LBoostTree();
    ~LBoostTree();

    /// @brief �������������������
    ///  
    /// @param[in] num ������������
    void SetMaxClassifierNum(IN unsigned int num);

    /// @brief ѵ��ģ��
    ///  
    /// @param[in] problem ԭʼ����
    /// @return ����true��ʾѵ���ɹ�, ����false��ʾ�������ݴ���
    bool TrainModel(IN const LBoostProblem& problem);

    /// @brief ʹ��ѵ���õ�ģ�ͽ���Ԥ��(������Ԥ��)
    ///  
    /// �뱣֤��ҪԤ����������������Ⱥ�ѵ������������������ͬ
    /// @param[in] sample ��ҪԤ�������
    /// @return ����Ԥ����: BOOST_SUN or BOOST_MOON, ����0.0��ʾ����(��ҪԤ��������������ģ��û��ѵ����)
    float Predict(IN const LBoostMatrix& sample);

    /// @brief ʹ��ѵ���õ�ģ�ͽ���Ԥ��(������Ԥ��)
    ///  
    /// �뱣֤��ҪԤ����������������Ⱥ�ѵ������������������ͬ
    /// @param[in] sampleMatrix ��ҪԤ�����������
    /// @return ����true��ʾ�ɹ�, ����false��ʾ����(��ҪԤ��������������ģ��û��ѵ����)
    bool Predict(IN const LBoostMatrix& sampleMatrix, OUT LBoostMatrix& classisVector);

public:
    /// @brief ������׮
    ///  
    /// @param[in] problem ԭʼ����
    /// @param[in] weightVector Ȩ������(������), ����Ϊԭʼ�����������������, ����Ϊ1
    /// @param[out] stump ����õ���׮
    /// @param[out] classisVector ����ŵ��������(������), ��������Ϊ�������������, ��������Ϊ1
    /// @return
    void BuildStump(
        IN const LBoostProblem& problem, 
        IN const LBoostMatrix& weightVector,
        OUT LBoostStump& stump,
        OUT LBoostMatrix& classisVector);

    /// @brief ʹ����׮��ѵ���������з���
    ///  
    /// @param[in] sampleMatrix ��������
    /// @param[in] stump ���з������׮
    /// @param[out] classisVector �������(������), ��������Ϊ�������������, ��������Ϊ1
    /// @return
    void StumpClassify(
        IN const LBoostMatrix& sampleMatrix,
        IN const LBoostStump& stump,
        OUT LBoostMatrix& classisVector);

private:
    LBoostStump* m_pWeakClassifierList; ///< ���������б�
    unsigned int m_weakClassifierNum; ///< ������������
    unsigned int m_maxWeakClassifierNum; ///< ���������������
    unsigned int m_featureNumber; ///< ��������������
};


#endif