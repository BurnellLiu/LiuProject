
#include "LBayesClassify.h"

#include <map>
using std::map;
#include <vector>
using std::vector;

/// @brief ������������
class CFeatureClassCount
{
public:
    /// @brief ��ָ��������ָ������������1
    /// @param[in] featureValue ����ֵ
    /// @param[in] classValue ���ֵ
    void CountInc(IN int featureValue, IN int classValue)
    {
        m_featureClassMap[featureValue][classValue]++;
    }

    /// @brief ��ȡָ��������ָ�����ļ���
    /// @param[in] featureValue ����ֵ
    /// @param[in] classValue ���ֵ
    /// @return ���ļ���
    unsigned int GetCount(IN int featureValue, IN int classValue)
    {
        return m_featureClassMap[featureValue][classValue];
    }

    /// @brief ��ȡָ���������ܼ���
    /// @param[in] featureValue ����ֵ
    /// @return ����ֵ���ܼ���
    unsigned int GetTotalCount(IN int featureValue)
    {
        auto classMap = m_featureClassMap[featureValue];
        unsigned int totalCount = 0;
        for (auto iter = classMap.begin(); iter != classMap.end(); iter++)
        {
            totalCount += iter->second;
        }

        return totalCount;
    }

    /// @brief �������
    void Clear()
    {
        m_featureClassMap.clear();
    }
private:
    map<int, map<int, unsigned int>> m_featureClassMap; ///< ����ӳ��, <����ֵ, <���ֵ, ������>>
};

/// @brief ��Ҷ˹������ʵ����
class CBayesClassify
{
public:
    CBayesClassify();
    ~CBayesClassify();

    /// @brief ѵ��ģ��
    /// @param[in] problem ��Ҷ˹����
    /// @return �ɹ�����true, ʧ�ܷ���false, �������������»᷵��false
    bool TrainModel(IN const LBayesProblem& problem);

    /// @brief ʹ��ѵ���õ�ģ�ͽ���Ԥ��
    /// �뱣֤��ҪԤ����������������Ⱥ�ѵ������������������ͬ
    /// @param[in] sample ��ҪԤ�������
    /// @param[out] pClassValue �洢Ԥ����
    /// @return �ɹ�Ԥ�ⷵ��true, ʧ�ܷ���false, ���������ģ��δѵ��������»᷵��false
    bool Predict(IN const LBayesMatrix& sample, OUT int* pClassValue);

private:
    /// @brief ��ȡָ����������ָ�����ĸ���ֵ, Pr(class | sample)
    /// @param[in] sample ����
    /// @param[in] classValue ���ֵ
    /// @return ����ֵ
    float GetProbSampleInClass(IN const LBayesMatrix& sample, IN int classValue);

private:
    vector<CFeatureClassCount> m_featureClassCountList; ///< ������������
    map<int, unsigned int> m_sampleClassCount; ///< ѵ������������
    unsigned int m_featureCount; ///< ������������
    unsigned int m_sampleCount; ///< ѵ����������
};

CBayesClassify::CBayesClassify()
{
    m_featureCount = 0;
    m_sampleCount = 0;
}

CBayesClassify::~CBayesClassify()
{

}

bool CBayesClassify::TrainModel(IN const LBayesProblem& problem)
{
    // ���в������
    if (problem.XMatrix.ColumnLen < 1)
        return false;
    if (problem.XMatrix.RowLen < 1)
        return false;
    if (problem.YVector.ColumnLen != 1)
        return false;
    if (problem.XMatrix.RowLen != problem.YVector.RowLen)
        return false;

    m_sampleClassCount.clear();
    m_featureClassCountList.clear();
    m_sampleCount = problem.XMatrix.RowLen;
    m_featureCount = problem.XMatrix.ColumnLen;
    for (unsigned int i = 0; i < m_featureCount; i++)
    {
        m_featureClassCountList.push_back(CFeatureClassCount());
    }

    for (unsigned int row = 0; row < problem.XMatrix.RowLen; row++)
    {
        int classValue = problem.YVector[row][0];
        m_sampleClassCount[classValue]++;

        for (unsigned int col = 0; col < problem.XMatrix.ColumnLen; col++)
        {
            int featureValue = problem.XMatrix[row][col];
            m_featureClassCountList[col].CountInc(featureValue, classValue);
        }
        
    }

    return true;
}

bool CBayesClassify::Predict(IN const LBayesMatrix& sample, OUT int* pClassValue)
{
    // ������
    if (1 != sample.RowLen)
        return false;
    if (m_featureCount != sample.ColumnLen)
        return false;
    if (0 == pClassValue)
        return false;

    if (m_sampleCount == 0)
        return false;


    float maxProb = 0;
    int bestClassValue;

    for (auto iter = m_sampleClassCount.begin(); iter != m_sampleClassCount.end(); iter++)
    {
        int classValue = iter->first;
        float prob = this->GetProbSampleInClass(sample, classValue);
        if (prob > maxProb)
        {
            maxProb = prob;
            bestClassValue = classValue;
        }
    }

    (*pClassValue) = bestClassValue;

    return true;
}


float CBayesClassify::GetProbSampleInClass(IN const LBayesMatrix& sample, IN int classValue)
{
    // ��Ҷ˹��ʽ:
    // P(y|x) = P(x|y) * P(y) / P(x)
    // ����ÿ��������˵P(x)ֵ��ͬ, ��������ֻ�迼���ĸ��ֵ, Ҳ����P(x|y) * P(y)
    // ��Ϊ����������������
    // P(x|y) * P(y) = P(a1|y) * P(a2|y) * ... * P(an|y) * P(y)

    unsigned int classCount = m_sampleClassCount[classValue];

    float prob = 1.0f;
    for (unsigned int col = 0; col < sample.ColumnLen; col++)
    {
        int featureValue = sample[0][col];
        unsigned int featureClassCount = m_featureClassCountList[col].GetCount(featureValue, classValue);
        float basicProb = (float)featureClassCount/(float)classCount;
        unsigned int featureTotalCount = m_featureClassCountList[col].GetTotalCount(featureValue);
        // w = 0.5 + totalCount/(1 + totalCount) * (basicProb - 0.5)
        // ʹ��Ȩ�ظ��ʿ��Խ����������:
        // ����ֵ��ָ��������ִ���Ϊ0���¸���Ϊ0�����
        float weightedProb = 0.5f + (float)featureTotalCount/(1.0f + (float)featureTotalCount) * (basicProb - 0.5f);

        prob *= weightedProb;
    }

    prob *= (float)classCount/(float)m_sampleCount;

    return prob;
}


LBayesClassify::LBayesClassify()
{
    m_pBayesClassify = 0;
    m_pBayesClassify = new CBayesClassify();
}

LBayesClassify::~LBayesClassify()
{
    if (0 != m_pBayesClassify)
    {
        delete m_pBayesClassify;
        m_pBayesClassify = 0;
    }
}

bool LBayesClassify::TrainModel(IN const LBayesProblem& problem)
{
    return m_pBayesClassify->TrainModel(problem);
}

bool LBayesClassify::Predict(IN const LBayesMatrix& sample, OUT int* classValue)
{
    return m_pBayesClassify->Predict(sample, classValue);
}