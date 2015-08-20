

#include <cmath>
#include <cstdio>

#include <vector>
using std::vector;

#include "LBoost.h"

/// @brief Boost�������
enum LBOOST_CLASSIFY_RULE
{
    LARGER_SUN = 1, ///< ���������ΪBOOST_SUN, С�ڵ��������ΪBOOST_MOON
    LARGER_MOON ///< ���������ΪBOOST_MOON, С�ڵ��������ΪBOOST_SUN
};

/// @brief Boost��׮������
///
/// ��������
struct LBoostStump
{
    unsigned int FeatureIndex; ///< ��������, ʹ�ø������������������з���
    float FeatureThreshold; ///< ������ֵ, ʹ�ø�������ֵ���������з���
    LBOOST_CLASSIFY_RULE ClassifyRule; ///< �������
    float Alpha; //< ����׮��Alpha
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
    

    // ���첢�ҳ�ʼ��Ȩ������(������)
    LBoostMatrix weightVector(problem.XMatrix.RowLen, 1);
    for (unsigned int i =0; i < weightVector.RowLen; i++)
    {
        weightVector[i][0] = 1.0f/(float)weightVector.RowLen;
    }

    // �����ۼ��������(������)���ҳ�ʼ��
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

        // ʹ��alpha����Ȩ������
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

        // �����ۼ��������
        for (unsigned int m = 0; m < sumClassisVector.RowLen; m++)
        {
            sumClassisVector[m][0] += stump.Alpha * classisVector[m][0];
        }

        // �����ۼӴ�����
        int errorCount = 0; // ����������
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
    const float CLASSIFY_RIGHT = 0.0f; ///< ������ȷ���
    const float CLASSIFY_ERROR = 1.0f; ///< ���������

    const LBoostMatrix& X = problem.XMatrix; // ��������
    const LBoostMatrix& Y = problem.YVector; // ��ǩ����

    const unsigned int M = X.RowLen; // ��������
    const unsigned int N = X.ColumnLen; // ������������

    const  int StepNum = 10;

    LBoostMatrix classisVectorTemp(M, 1); // �����������(������)
    LBoostMatrix errorVector(M, 1); // ��������(������), ��������Ϊ������Ŀ, ������Ϊ1, 
    //����ȷ�����������Ӧ�ڴ��������лᱻ���Ϊ0.0f, ����������������Ӧ�ڴ��������лᱻ���Ϊ1.0f

    LBoostStump stumpTemp; // ���ڷ������׮
    LBoostMatrix weightErrorMatrix(1, 1);
    
    float minWeightError = 1.0f; // ��СȨ�ش�����
    LBoostStump bestStump; // ��õ���׮
    LBoostMatrix bestClassisVector(M, 1);  // ����������

    

    // ��ÿһ������
    for (unsigned int n = 0; n < N; n++)
    {
        stumpTemp.FeatureIndex = n;

        float rangeMin = X[0][n]; // ������������i(����)�е���Сֵ
        float rangeMax = X[0][n]; // ������������i(����)�е����ֵ
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


    // ȷ��û�д���ʱ, ���ᷢ����0���
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