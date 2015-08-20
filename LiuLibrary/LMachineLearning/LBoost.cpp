

#include <cmath>
#include <cstdio>

#include <vector>
using std::vector;

#include "LBoost.h"

/// @brief Boost分类规则
enum LBOOST_CLASSIFY_RULE
{
    LARGER_SUN = 1, ///< 大于则分类为BOOST_SUN, 小于等于则分类为BOOST_MOON
    LARGER_MOON ///< 大于则分类为BOOST_MOON, 小于等于则分类为BOOST_SUN
};

/// @brief Boost树桩分类器
///
/// 弱分类器
struct LBoostStump
{
    unsigned int FeatureIndex; ///< 特征索引, 使用该特征索引对样本进行分类
    float FeatureThreshold; ///< 特征阈值, 使用该特征阈值对样本进行分类
    LBOOST_CLASSIFY_RULE ClassifyRule; ///< 分类规则
    float Alpha; //< 该树桩的Alpha
};

LBoostTree::LBoostTree()
{
    this->m_featureNumber = 0;
    this->m_weakClassifierNum = 0;
    this->m_pWeakClassifierList = 0;
    this->m_maxWeakClassifierNum = 40;
}


LBoostTree::~LBoostTree()
{
    if (this->m_pWeakClassifierList )
    {
        delete[] m_pWeakClassifierList;
        m_pWeakClassifierList = 0;
    }
}

void LBoostTree::SetMaxClassifierNum(IN unsigned int num)
{
    m_maxWeakClassifierNum = num;
}

bool LBoostTree::TrainModel(IN const LBoostProblem& problem)
{
    if (problem.XMatrix.ColumnLen < 1)
        return false;
    if (problem.XMatrix.RowLen < 2)
        return false;
    if (problem.YVector.ColumnLen != 1)
        return false;
    if (problem.YVector.RowLen != problem.XMatrix.RowLen)
        return false;

    for (unsigned int i = 0; i < problem.YVector.RowLen; i++)
    {
        if (problem.YVector[i][0] != LBOOST_SUN &&
            problem.YVector[i][0] != LBOOST_MOON)
            return false;
    }

    this->m_featureNumber = problem.XMatrix.ColumnLen;
    

    // 构造并且初始化权重向量(列向量)
    LBoostMatrix weightVector(problem.XMatrix.RowLen, 1);
    for (unsigned int i =0; i < weightVector.RowLen; i++)
    {
        weightVector[i][0] = 1.0f/(float)weightVector.RowLen;
    }

    // 构造累加类别向量(列向量)并且初始化
    LBoostMatrix sumClassisVector(problem.XMatrix.RowLen, 1);
    for (unsigned int i = 0; i < sumClassisVector.RowLen; i++)
    {
        sumClassisVector[i][0] = 0.0f;
    }

    LBoostStump stump;
    LBoostMatrix classisVector;

    vector<LBoostStump> weakClassifierList;

    for (unsigned int i = 0; i < m_maxWeakClassifierNum; i++)
    {
       
        this->BuildStump(problem, weightVector, stump, classisVector);

        weakClassifierList.push_back(stump);

        // 使用alpha更新权重向量
        float sumWeight = 0.0f;
        for (unsigned int m = 0; m < weightVector.RowLen; m++)
        {
            weightVector[m][0] = weightVector[m][0] * exp(-1 * stump.Alpha * classisVector[m][0] * problem.YVector[m][0]);
            sumWeight += weightVector[m][0];
        }
        for (unsigned int m = 0; m < weightVector.RowLen; m++)
        {
            weightVector[m][0] = weightVector[m][0]/sumWeight;
        }

        // 计算累加类别向量
        for (unsigned int m = 0; m < sumClassisVector.RowLen; m++)
        {
            sumClassisVector[m][0] += stump.Alpha * classisVector[m][0];
        }

        // 计算累加错误率
        int errorCount = 0; // 错误分类计数
        for (unsigned int m = 0; m < sumClassisVector.RowLen; m++)
        {
            if (sumClassisVector[m][0] * problem.YVector[m][0] < 0)
                errorCount++;
        }
        float sumError = (float)errorCount/(float)sumClassisVector.RowLen;
        if (sumError == 0.0f)
            break;

    }

    if (weakClassifierList.size() > 0)
    {
        if (this->m_pWeakClassifierList != 0)
        {
            delete[] this->m_pWeakClassifierList;
            this->m_pWeakClassifierList = 0;
        }

        this->m_weakClassifierNum = weakClassifierList.size();
        this->m_pWeakClassifierList = new LBoostStump[weakClassifierList.size()];

        for (unsigned int i = 0; i < m_weakClassifierNum; i++)
        {
            this->m_pWeakClassifierList[i] = weakClassifierList[i];
        }
    }

    return true;
}

float LBoostTree::Predict(IN const LBoostMatrix& sample)
{
    if (sample.RowLen != 1)
        return 0.0f;

    LBoostMatrix classisVector(1, 1);
    bool bRet = this->Predict(sample, classisVector);
    if (!bRet)
        return 0.0f;

    return classisVector[0][0];
}

bool LBoostTree::Predict(IN const LBoostMatrix& sampleMatrix, OUT LBoostMatrix& classisVector)
{
    if (this->m_weakClassifierNum < 1)
        return false;

    if (sampleMatrix.RowLen < 1)
        return false;

    if (sampleMatrix.ColumnLen != this->m_featureNumber)
        return false;

    LBoostMatrix classisVectorTemp(sampleMatrix.RowLen, 1);
    LBoostMatrix sumClassisVector(sampleMatrix.RowLen, 1);
    for (unsigned int m = 0; m < sumClassisVector.RowLen; m++)
    {
        sumClassisVector[m][0] = 0.0f;
    }

    for (unsigned int i = 0; i < this->m_weakClassifierNum; i++)
    {
        LBoostStump& stump = this->m_pWeakClassifierList[i];
        this->StumpClassify(sampleMatrix, stump, classisVectorTemp);
        for (unsigned int m = 0; m < classisVectorTemp.RowLen; m++)
        {
            sumClassisVector[m][0] += stump.Alpha * classisVectorTemp[m][0];
        }
    }

    classisVector.Reset(sampleMatrix.RowLen, 1);

    for (unsigned int m = 0; m < sumClassisVector.RowLen; m++)
    {
        if (sumClassisVector[m][0] >= 0.0f)
            classisVector[m][0] = LBOOST_SUN;
        else
            classisVector[m][0] = LBOOST_MOON;
    }

    return true;


}


void LBoostTree::BuildStump(
    IN const LBoostProblem& problem, 
    IN const LBoostMatrix& weightVector, 
    OUT LBoostStump& stump, 
    OUT LBoostMatrix& classisVector)
{
    const float CLASSIFY_RIGHT = 0.0f; ///< 分类正确标记
    const float CLASSIFY_ERROR = 1.0f; ///< 分类错误标记

    const LBoostMatrix& X = problem.XMatrix; // 样本矩阵
    const LBoostMatrix& Y = problem.YVector; // 标签矩阵

    const unsigned int M = X.RowLen; // 样本数量
    const unsigned int N = X.ColumnLen; // 样本特征数量

    const  int StepNum = 10;

    LBoostMatrix classisVectorTemp(M, 1); // 分类出的向量(列向量)
    LBoostMatrix errorVector(M, 1); // 错误向量(列向量), 矩阵行数为样本数目, 矩阵列为1, 
    //被正确分类的样本对应在错误向量中会被标记为0.0f, 被错误分类的样本对应在错误向量中会被标记为1.0f

    LBoostStump stumpTemp; // 用于分类的树桩
    LBoostMatrix weightErrorMatrix(1, 1);
    
    float minWeightError = 1.0f; // 最小权重错误率
    LBoostStump bestStump; // 最好的树桩
    LBoostMatrix bestClassisVector(M, 1);  // 最好类别向量

    

    // 对每一个特征
    for (unsigned int n = 0; n < N; n++)
    {
        stumpTemp.FeatureIndex = n;

        float rangeMin = X[0][n]; // 所有样本中列i(特征)中的最小值
        float rangeMax = X[0][n]; // 所有样本中列i(特征)中的最大值
        for (unsigned int m = 0; m < M; m++)
        {
            if (X[m][n] < rangeMin)
                rangeMin = X[m][n];
            if (X[m][n] > rangeMax)
                rangeMax = X[m][n];
        }

        float stepSize = (rangeMax - rangeMin)/(float)StepNum;

        for (int k = -1; k <= StepNum + 1; k++)
        {
            stumpTemp.FeatureThreshold = rangeMin + k * stepSize;
            stumpTemp.ClassifyRule = LARGER_SUN;
            this->StumpClassify(X, stumpTemp, classisVectorTemp);
            
            for (unsigned int m = 0; m < M; m++)
            {
                if (classisVectorTemp[m][0] == Y[m][0])
                    errorVector[m][0] = CLASSIFY_RIGHT;
                else
                    errorVector[m][0] = CLASSIFY_ERROR;
            }

            weightErrorMatrix = weightVector.T() * errorVector;
            float weightError = weightErrorMatrix[0][0];
            if (weightError < minWeightError)
            {
                minWeightError = weightError;
                bestStump = stumpTemp;
                bestClassisVector = classisVectorTemp;
            }

            stumpTemp.ClassifyRule = LARGER_MOON;
            this->StumpClassify(X, stumpTemp, classisVectorTemp);

            for (unsigned int m = 0; m < M; m++)
            {
                if (classisVectorTemp[m][0] == Y[m][0])
                    errorVector[m][0] = CLASSIFY_RIGHT;
                else
                    errorVector[m][0] = CLASSIFY_ERROR;
            }

            weightErrorMatrix = weightVector.T() * errorVector;
            weightError = weightErrorMatrix[0][0];
            if (weightError < minWeightError)
            {
                minWeightError = weightError;
                bestStump = stumpTemp;
                bestClassisVector = classisVectorTemp;
            }
        }


    }


    // 确保没有错误时, 不会发生除0溢出
    if (minWeightError < 1e-16)
        minWeightError = (float)1e-16;

    bestStump.Alpha = 0.5f * log((1.0f-minWeightError)/minWeightError);

    stump = bestStump;
    classisVector = bestClassisVector;

}

void LBoostTree::StumpClassify(
    IN const LBoostMatrix& sampleMatrix, 
    IN const LBoostStump& stump, 
    OUT LBoostMatrix& classisVector)
{
    classisVector.Reset(sampleMatrix.RowLen, 1);

    for (unsigned int i = 0; i < sampleMatrix.RowLen; i++)
    {
        if (stump.ClassifyRule == LARGER_SUN)
        {
            if (sampleMatrix[i][stump.FeatureIndex] > stump.FeatureThreshold)
                classisVector[i][0] = LBOOST_SUN;
            else
                classisVector[i][0] = LBOOST_MOON;
        }

        if (stump.ClassifyRule == LARGER_MOON)
        {
            if (sampleMatrix[i][stump.FeatureIndex] > stump.FeatureThreshold)
                classisVector[i][0] = LBOOST_MOON;
            else
                classisVector[i][0] = LBOOST_SUN;
        }
    }
}