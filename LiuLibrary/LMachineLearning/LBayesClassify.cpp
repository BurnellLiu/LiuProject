
#include "LBayesClassify.h"

#include <map>
using std::map;
#include <vector>
using std::vector;

/// @brief 特征类别计数类
class CFeatureClassCount
{
public:
    /// @brief 将指定特征的指定的类别计数加1
    /// @param[in] featureValue 特征值
    /// @param[in] classValue 类别值
    void CountInc(IN int featureValue, IN int classValue)
    {
        m_featureClassMap[featureValue][classValue]++;
    }

    /// @brief 获取指定特征的指定类别的计数
    /// @param[in] featureValue 特征值
    /// @param[in] classValue 类别值
    /// @return 类别的计数
    unsigned int GetCount(IN int featureValue, IN int classValue)
    {
        return m_featureClassMap[featureValue][classValue];
    }

    /// @brief 获取指定特征的总计数
    /// @param[in] featureValue 特征值
    /// @return 特征值得总计数
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

    /// @brief 清除数据
    void Clear()
    {
        m_featureClassMap.clear();
    }
private:
    map<int, map<int, unsigned int>> m_featureClassMap; ///< 特征映射, <特征值, <类别值, 类别计数>>
};

/// @brief 贝叶斯分类器实现类
class CBayesClassify
{
public:
    CBayesClassify();
    ~CBayesClassify();

    /// @brief 训练模型
    /// @param[in] problem 贝叶斯问题
    /// @return 成功返回true, 失败返回false, 参数错误的情况下会返回false
    bool TrainModel(IN const LBayesProblem& problem);

    /// @brief 使用训练好的模型进行预测
    /// 请保证需要预测的样本的特征长度和训练样本的特征长度相同
    /// @param[in] sample 需要预测的样本
    /// @param[out] pClassValue 存储预测结果
    /// @return 成功预测返回true, 失败返回false, 参数错误或模型未训练的情况下会返回false
    bool Predict(IN const LBayesMatrix& sample, OUT int* pClassValue);

private:
    /// @brief 获取指定样本属于指定类别的概率值, Pr(class | sample)
    /// @param[in] sample 样本
    /// @param[in] classValue 类别值
    /// @return 概率值
    float GetProbSampleInClass(IN const LBayesMatrix& sample, IN int classValue);

private:
    vector<CFeatureClassCount> m_featureClassCountList; ///< 特征类别计数组
    map<int, unsigned int> m_sampleClassCount; ///< 训练样本类别计数
    unsigned int m_featureCount; ///< 样本特征数量
    unsigned int m_sampleCount; ///< 训练样本总数
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
    // 进行参数检查
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
    // 检查参数
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
    // 贝叶斯公式:
    // P(y|x) = P(x|y) * P(y) / P(x)
    // 对于每个样本来说P(x)值相同, 所以我们只需考察分母的值, 也就是P(x|y) * P(y)
    // 因为各个特征独立所以
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
        // 使用权重概率可以解决以下问题:
        // 特征值在指定分类出现次数为0导致概率为0的情况
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