/// @file LBoost.h
/// @brief ��ͷ�ļ���������һЩ�����㷨
/// 
/// Detail:
/// ���������ĺ���˼�������ͬһ��ѵ����ѵ����ͬ�ķ�����(��������), Ȼ�����Щ���������������, ����һ����ǿ�����շ�����(ǿ������)
/// ������: �мලѧϰ, �б�ģ��, ��Ԫ����, ����������������Ϊ����׮
/// @author Burnell_Liu  Email:burnell_liu@outlook.com
/// @version   
/// @date 22:7:2015
/// @sample

/* ʹ����������ʾ����������:

    //������������ͱ�ǩ����
    //���¶�������ݼ�, ֻ��һ��������������޷������
    //����������������������Ϳɽ����������
    float sampleList[5 * 2] =
    {
        1.0f, 2.0f,
        2.0f, 1.0f,
        2.0f, 0.0f,
        1.5f, 0.5f,
        1.0f, 1.0f
    };

    float labelList[5 * 1] = 
    {
        LBOOST_MOON,
        LBOOST_MOON,
        LBOOST_MOON,
        LBOOST_SUN,
        LBOOST_SUN
    };


    LBoostMatrix sampleMatrix(5, 2, sampleList);
    LBoostMatrix labelVector(5, 1, labelList);
    LBoostProblem problem(sampleMatrix, labelVector);


    // ����������, ʹ�����������в���ѵ��
    LBoostTree boostTree;
    boostTree.TrainModel(problem);

    // ����ѵ��������
    LBoostMatrix resultVector;
    boostTree.Predict(sampleMatrix, &resultVector);
    unsigned errorCount = 0;
    for (unsigned int i = 0; i < resultVector.RowLen; i++)
    {
        if (resultVector[i][0] != labelVector[i][0])
            errorCount++;
    }

    float errorRate = (float)errorCount/resultVector.RowLen;
*/


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

/// @brief Boostԭʼ����ṹ
struct LBoostProblem
{
    /// @brief ���캯��
    /// @param[in] sampleMatrix ��������, ÿһ��Ϊһ������, ÿ���е�ֵΪ����������ֵ
    /// @param[in] classVector �������(������), ����Ϊ�������������, ����Ϊ1, ֻ��ΪBOOST_MOON��BOOST_SUN
    LBoostProblem(IN const LBoostMatrix& sampleMatrix, IN const LBoostMatrix& classVector)
        : XMatrix(sampleMatrix), YVector(classVector)
    {
    }

    const LBoostMatrix& XMatrix; ///< ��������
    const LBoostMatrix& YVector; ///< ��ǩ����(������)
};

class CBoostTree;

/// @brief ������
class LBoostTree
{
public:
    /// @brief ���캯��
    LBoostTree();

    /// @brief ��������
    ~LBoostTree();

    /// @brief �������������������, Ĭ��ֵΪ40
    /// @param[in] num ������������
    void SetMaxClassifierNum(IN unsigned int num);

    /// @brief ѵ��ģ��
    /// @param[in] problem ԭʼ����
    /// @return ����true��ʾѵ���ɹ�, ����false��ʾ�������ݴ���
    bool TrainModel(IN const LBoostProblem& problem);

    /// @brief ʹ��ѵ���õ�ģ�ͽ���Ԥ��(������Ԥ��)
    /// �뱣֤��ҪԤ����������������Ⱥ�ѵ������������������ͬ
    /// @param[in] sample ��ҪԤ�������
    /// @return ����Ԥ����: BOOST_SUN or BOOST_MOON, ����0.0��ʾ����(��ҪԤ��������������ģ��û��ѵ����)
    float Predict(IN const LBoostMatrix& sample);

    /// @brief ʹ��ѵ���õ�ģ�ͽ���Ԥ��(������Ԥ��)
    /// �뱣֤��ҪԤ����������������Ⱥ�ѵ������������������ͬ
    /// @param[in] sampleMatrix ��ҪԤ�����������
    /// @param[out] pClassisVector �洢Ԥ����������
    /// @return ����true��ʾ�ɹ�, ����false��ʾ����(��ҪԤ��������������ģ��û��ѵ����)
    bool Predict(IN const LBoostMatrix& sampleMatrix, OUT LBoostMatrix* pClassisVector);

private:
    CBoostTree* m_pBoostTree; ///< ������ʵ�ֶ���

private:
    // ��ֹ�������캯���͸�ֵ������
    LBoostTree(const LBoostTree&);
    LBoostTree& operator = (const LBoostTree&);
};


#endif