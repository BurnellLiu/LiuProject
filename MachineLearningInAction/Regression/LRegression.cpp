
#include "LRegression.h"

#include <cmath>

LLinearRegression::LLinearRegression(IN const LRTrainingSet& trainingSet, IN LRType learningRate)
    : m_trainingSet(trainingSet), m_learningRate(learningRate)
{
    m_paramList.Reset(m_trainingSet[0].FeatureList.Length + 1);

    for (int i = 0; i < m_paramList.Length; i++)
    {
        m_paramList[i] = 0.0f;
    }
}

LLinearRegression::~LLinearRegression()
{

}

LRType LLinearRegression::BatchDescend()
{
    /*
    θi = θi - α * ∑((h(x) - y) * xi)
    */
    LRType ERROR_RET = -1.0f;

    if (m_trainingSet.Length <= 0)
        return ERROR_RET;
    if (m_learningRate <= 0)
        return ERROR_RET;

    LRType ret = ERROR_RET;
    for (int i = 0; i < m_paramList.Length; i++)
    {
        float derivative = 0.0f; // 参数的导数
        for (int m = 0; m < m_trainingSet.Length; m++)
        {
            float actValue = 0.0f; // 实际值
            for (int n = 0; n < m_trainingSet[0].FeatureList.Length; n++)
            {
                actValue += m_trainingSet[m].FeatureList[n] * m_paramList[n];
            }

            actValue += m_paramList[m_paramList.Length-1] * 1;

            float difValue = actValue-m_trainingSet[m].Target;

            if ( i == m_trainingSet[0].FeatureList.Length)
                derivative += difValue;
            else
                derivative += difValue * m_trainingSet[m].FeatureList[i];
        }
        m_paramList[i] = m_paramList[i] - m_learningRate * derivative;
    }

    return ret;
}

LRType LLinearRegression::ForeCast(IN const LRFeatureList& featureList)
{
    LRType expectedValue = 0.0f; // 预期值

    if (featureList.Length + 1 != m_paramList.Length)
        return expectedValue;

    for (int n = 0; n < featureList.Length; n++)
    {
        expectedValue += featureList[n] * m_paramList[n];
    }

    expectedValue += m_paramList[m_paramList.Length-1] * 1;

    return expectedValue;
}

LRType LLinearRegression::GetSquareValue()
{
    LRType squareValue = 0.0f;
    for (int m = 0; m < m_trainingSet.Length; m++)
    {
        LRType difValue = 0.0f;
        difValue = this->ForeCast(m_trainingSet[m].FeatureList) - m_trainingSet[m].Target;
        difValue = difValue * difValue;
        squareValue += difValue;
    }

    return squareValue;
}

void LLinearRegression::GetFeatureParam(OUT LRFeatureParamList& paramList)
{
    paramList = m_paramList;
}

LLogisticRegression::LLogisticRegression(IN const LRTrainingSet& trainingSet, IN LRType learningRate)
    : m_trainingSet(trainingSet), m_learningRate(learningRate)
{
    m_paramList.Reset(m_trainingSet[0].FeatureList.Length + 1);

    for (int i = 0; i < m_paramList.Length; i++)
    {
        m_paramList[i] = 0.0f;
    }
}

LLogisticRegression::~LLogisticRegression()
{

}

void LLogisticRegression::BatchAscend()
{
    /*
    θi = θi + α * ∑((y - h(x)) * xi)
    */
    if (m_trainingSet.Length <= 0)
        return;
    if (m_learningRate <= 0)
        return;

    for (int i = 0; i < m_paramList.Length; i++)
    {
        float derivative = 0.0f; // 参数的导数
        for (int m = 0; m < m_trainingSet.Length; m++)
        {
            float actValue = 0.0f; // 实际值
            for (int n = 0; n < m_trainingSet[0].FeatureList.Length; n++)
            {
                actValue += m_trainingSet[m].FeatureList[n] * m_paramList[n];
            }
            actValue += m_paramList[m_paramList.Length-1] * 1;

            actValue = 1/(1 + exp(actValue * -1.0f));

            float difValue = m_trainingSet[m].Target - actValue;

            if ( i == m_trainingSet[0].FeatureList.Length)
                derivative += difValue * 1;
            else
                derivative += difValue * m_trainingSet[m].FeatureList[i];
        }
        m_paramList[i] = m_paramList[i] + m_learningRate * derivative;
    }

}

LRType LLogisticRegression::ForeCast(IN const LRFeatureList& featureList, IN LRBINARY_CLASS bClass)
{
    const LRType ERROR_RET = -1.0f;

    if (featureList.Length + 1 != m_paramList.Length)
        return ERROR_RET;

    float prob = 0.0f; // 概率值
    for (int n = 0; n < featureList.Length; n++)
    {
        prob += featureList[n] * m_paramList[n];
    }
    prob += m_paramList[m_paramList.Length-1] * 1;

    prob = 1/(1 + exp(prob * -1.0f));

    if (bClass == ONE)
        return prob;

    if (bClass == ZERO)
        return 1-prob;

    return ERROR_RET;
}

LRType LLogisticRegression::GetLikelihood()
{
    LRType likelihood = 1.0f;
    for (int m = 0; m < m_trainingSet.Length; m++)
    {
        if (m_trainingSet[m].Target == ONE)
            likelihood *= this->ForeCast(m_trainingSet[m].FeatureList, ONE);
        if (m_trainingSet[m].Target == ZERO)
            likelihood *= this->ForeCast(m_trainingSet[m].FeatureList, ZERO);
    }

    return likelihood;
}


void LLogisticRegression::GetFeatureParam(OUT LRFeatureParamList& paramList)
{
    paramList = m_paramList;
}

