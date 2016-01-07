

#include <cmath>
#include <cstdio>

#include <vector>
using std::vector;

#include "LBoost.h"

/// @brief ׮�������
enum LSTUMP_CLASSIFY_RULE
{
    LARGER_SUN = 1, ///< ���������ΪBOOST_SUN, С�ڵ��������ΪBOOST_MOON
    LARGER_MOON ///< ���������ΪBOOST_MOON, С�ڵ��������ΪBOOST_SUN
};

/// @brief ׮�ṹ
struct LStump
{
    unsigned int FeatureIndex; ///< ��������, ʹ�ø������������������з���
    float FeatureThreshold; ///< ������ֵ, ʹ�ø�������ֵ���������з���
    LSTUMP_CLASSIFY_RULE ClassifyRule; ///< �������, ʹ�ø÷��������������з���
};

/// @brief ��׮������
/// ��������
class CStumpClassifer
{
public:
    /// @brief ���캯��
    CStumpClassifer()
    {
        m_bTrained = false;
    }

    /// @brief ��������
    ~CStumpClassifer()
    {

    }

    /// @brief ѵ��
    /// @param[in] problem ԭʼ����
    /// @param[inout] weightVector ѵ��������Ȩ������, �ɹ�ѵ���󱣴���º��Ȩ������
    /// @param[out] pResultVector �洢�÷���׮�ķ�����, ����Ϊ0
    /// @return �ɹ�����true, ʧ�ܷ���false, �������󷵻�false
    bool Train(
        IN const LBoostProblem& problem, 
        INOUT vector<float>& weightVector, 
        OUT vector<float>* pResultVector)
    {
        // ������
        if (problem.XMatrix.ColumnLen < 1)
            return false;
        if (problem.XMatrix.RowLen < 2)
            return false;
        if (problem.YVector.ColumnLen != 1)
            return false;
        if (problem.YVector.RowLen != problem.XMatrix.RowLen)
            return false;
        if (problem.YVector.RowLen != weightVector.size())
            return false;
        if (0 == pResultVector)
            return false;

        for (unsigned int i = 0; i < problem.YVector.RowLen; i++)
        {
            if (problem.YVector[i][0] != LBOOST_SUN &&
                problem.YVector[i][0] != LBOOST_MOON)
                return false;
        }

        const LBoostMatrix& X = problem.XMatrix; // ��������
        const LBoostMatrix& Y = problem.YVector; // ��ǩ����

        const unsigned int M = X.RowLen; // ��������
        const unsigned int N = X.ColumnLen; // ������������

        const int StepNum = 10;

        LStump stump; // ��׮��ʱ����
        vector<float> classisVector(M);

        LStump bestStump; // ��õ���׮
        float minWeightError = 1.0f; // ��СȨ�ش�����
        vector<float> bestClassisVector(M); // ��õķ���������
        

        // ��ÿһ������
        for (unsigned int n = 0; n < N; n++)
        {
            stump.FeatureIndex = n;

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
                stump.FeatureThreshold = rangeMin + k * stepSize;
                stump.ClassifyRule = LARGER_SUN;
                this->Classify(X, stump, &classisVector);

                float weightError = 0.0f;
                for (unsigned int m = 0; m < M; m++)
                {
                    if (classisVector[m] != Y[m][0])
                        weightError += weightVector[m];
                }

                // ��������ʴ���0.5f���ʾ�������Ӧ��ȡ��, ������ֱ��ȡ��, ��ʡ
                if (weightError > 0.5f)
                {
                    stump.ClassifyRule = LARGER_MOON;
                    weightError = 1.0f - weightError;
                    for (unsigned int m = 0; m < M; m++)
                        classisVector[m] *= LBOOST_MOON;
                }

                if (weightError < minWeightError)
                {
                    minWeightError = weightError;
                    bestStump = stump;
                    bestClassisVector = classisVector;
                }

            }
        }


        // ȷ��û�д���ʱ, ���ᷢ����0���
        if (minWeightError < 1e-16)
            minWeightError = (float)1e-16;

        m_featureNum = N;

        m_alpha = 0.5f * log((1.0f-minWeightError)/minWeightError);

        m_stump = bestStump;
        (*pResultVector) = bestClassisVector;
        for (auto iter = pResultVector->begin(); iter != pResultVector->end(); iter++)
        {
            (*iter) *= m_alpha; 
        }

        // ʹ��alpha����Ȩ������
        float sumWeight = 0.0f;
        for (unsigned int m = 0; m < M; m++)
        {
            weightVector[m] = weightVector[m] * exp(-1 * m_alpha * bestClassisVector[m] * problem.YVector[m][0]);
            sumWeight += weightVector[m];
        }
        for (unsigned int m = 0; m < M; m++)
        {
            weightVector[m] = weightVector[m]/sumWeight;
        }

        m_bTrained = true;
        return true;

    }

    /// @brief ʹ�÷���������Ԥ��
    /// @param[in] sampleMatrix ��������
    /// @param[out] pResultVector �洢�������, ����Ϊ0
    /// @return �ɹ�����true, ʧ�ܷ���false, ����������߷�����δѵ����ʧ��
    bool Predict(IN const LBoostMatrix& sampleMatrix, OUT vector<float>* pResultVector)
    {
        if (!m_bTrained)
            return false;

        if (sampleMatrix.ColumnLen != m_featureNum)
            return false;

        if (sampleMatrix.RowLen < 1)
            return false;

        if (0 == pResultVector)
            return false;

        vector<float> classisVector;
        this->Classify(sampleMatrix, m_stump, &classisVector);
        pResultVector->resize(classisVector.size());
        for (unsigned int i = 0; i < classisVector.size(); i++)
        {
            (*pResultVector)[i] = classisVector[i] * m_alpha;
        }

        return true;
    }

private:
    /// @brief ����
    /// @param[in] ampleMatrix ��������
    /// @param[in] stump ����׮�ṹ
    /// @param[in] pClassisVector �洢����������
    void Classify(
        IN const LBoostMatrix& sampleMatrix,
        IN const LStump& stump,
        OUT vector<float>* pClassisVector)
    {
        pClassisVector->resize(sampleMatrix.RowLen);

        for (unsigned int i = 0; i < sampleMatrix.RowLen; i++)
        {
            if (stump.ClassifyRule == LARGER_SUN)
            {
                if (sampleMatrix[i][stump.FeatureIndex] > stump.FeatureThreshold)
                    (*pClassisVector)[i] = LBOOST_SUN;
                else
                    (*pClassisVector)[i] = LBOOST_MOON;
            }

            if (stump.ClassifyRule == LARGER_MOON)
            {
                if (sampleMatrix[i][stump.FeatureIndex] > stump.FeatureThreshold)
                    (*pClassisVector)[i] = LBOOST_MOON;
                else
                    (*pClassisVector)[i] = LBOOST_SUN;
            }
        }
    }

private:
    bool m_bTrained; ///< ��ʶ�þ���׮�Ƿ��Ѿ���ѵ��
    unsigned int m_featureNum; ///< ����׮Ҫ�������������
    

    LStump m_stump; ///< ׮�ṹ
    float m_alpha; //< ����׮��Ȩ��ֵ
};

/// @brief ������
///
/// �Ծ�����Ϊ������������������Ϊ������
class CBoostTree
{
public:
    /// @brief ���캯��
    CBoostTree()
    {
        this->m_maxWeakClassifierNum = 40;
    }

    /// @brief ��������
    ~CBoostTree()
    {

    }

    /// @brief �������������������
    /// ��ϸ���ͼ�ͷ�ļ�LBoostTree�е�����
    void SetMaxClassifierNum(IN unsigned int num)
    {
        m_maxWeakClassifierNum = num;
    }

    /// @brief ѵ��ģ��
    /// ��ϸ���ͼ�ͷ�ļ�LBoostTree�е�����
    bool TrainModel(IN const LBoostProblem& problem)
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

        m_featureNum = problem.XMatrix.ColumnLen;

        // ���첢�ҳ�ʼ��Ȩ������(������)
        vector<float> weightVector(problem.XMatrix.RowLen);
        for (unsigned int i =0; i < weightVector.size(); i++)
        {
            weightVector[i] = 1.0f/(float)weightVector.size();
        }

        // �����ۼ��������(������)���ҳ�ʼ��
        vector<float> sumClassisVector(problem.XMatrix.RowLen, 0.0f);

        // ������������Ľ������
        vector<float> resultVector;

        m_weakClassifierList.clear();

        CStumpClassifer stumpClassifer;
        for (unsigned int i = 0; i < m_maxWeakClassifierNum; i++)
        {
            stumpClassifer.Train(problem, weightVector, &resultVector);
            m_weakClassifierList.push_back(stumpClassifer);

            // �����ۼ��������
            for (unsigned int m = 0; m < sumClassisVector.size(); m++)
            {
                sumClassisVector[m] += resultVector[m];
            }

            // �����ۼӴ�����
            int errorCount = 0; // ����������
            for (unsigned int m = 0; m < sumClassisVector.size(); m++)
            {
                if (sumClassisVector[m] * problem.YVector[m][0] < 0)
                    errorCount++;
            }
            float sumError = (float)errorCount/(float)sumClassisVector.size();
            if (sumError == 0.0f)
                break;

        }

        return true;
    }

    /// @brief ʹ��ѵ���õ�ģ�ͽ���Ԥ��(������Ԥ��)
    /// ��ϸ���ͼ�ͷ�ļ�LBoostTree�е�����
    float Predict(IN const LBoostMatrix& sample)
    {
        if (sample.RowLen != 1)
            return 0.0f;

        LBoostMatrix classisVector(1, 1);
        bool bRet = this->Predict(sample, &classisVector);
        if (!bRet)
            return 0.0f;

        return classisVector[0][0];
    }

    /// @brief ʹ��ѵ���õ�ģ�ͽ���Ԥ��(������Ԥ��)
    /// ��ϸ���ͼ�ͷ�ļ�LBoostTree�е�����
    bool Predict(IN const LBoostMatrix& sampleMatrix, OUT LBoostMatrix* pClassisVector)
    {
        if (this->m_weakClassifierList.size() < 1)
            return false;

        if (sampleMatrix.RowLen < 1)
            return false;

        if (sampleMatrix.ColumnLen != this->m_featureNum)
            return false;

        if (0 == pClassisVector)
            return false;

        pClassisVector->Reset(sampleMatrix.RowLen, 1);

        vector<float> resultVector(sampleMatrix.RowLen);
        vector<float> sumResultVector(sampleMatrix.RowLen, 0.0f);
        for (unsigned int i = 0; i < this->m_weakClassifierList.size(); i++)
        {
            CStumpClassifer& stumpClassifer = m_weakClassifierList[i];
            stumpClassifer.Predict(sampleMatrix, &resultVector);
            for (unsigned int j = 0; j < resultVector.size(); j++)
            {
                sumResultVector[j] += resultVector[j];
            }
        }

        for (unsigned int m = 0; m < sumResultVector.size(); m++)
        {
            if (sumResultVector[m] >= 0.0f)
                (*pClassisVector)[m][0] = LBOOST_SUN;
            else
                (*pClassisVector)[m][0] = LBOOST_MOON;
        }

        return true;
    }

private:
    vector<CStumpClassifer> m_weakClassifierList; ///< ���������б�
    unsigned int m_maxWeakClassifierNum; ///< ���������������
    unsigned int m_featureNum; ///< ������Ҫ�������������
};

LBoostTree::LBoostTree()
{
    m_pBoostTree = 0;
    m_pBoostTree = new CBoostTree();
}

LBoostTree::~LBoostTree()
{
    if (m_pBoostTree != 0)
    {
        delete m_pBoostTree;
        m_pBoostTree = 0;
    }
}

void LBoostTree::SetMaxClassifierNum(IN unsigned int num)
{
    m_pBoostTree->SetMaxClassifierNum(num);
}

bool LBoostTree::TrainModel(IN const LBoostProblem& problem)
{
    return m_pBoostTree->TrainModel(problem);
}

float LBoostTree::Predict(IN const LBoostMatrix& sample)
{
    return m_pBoostTree->Predict(sample);
}

bool LBoostTree::Predict(IN const LBoostMatrix& sampleMatrix, OUT LBoostMatrix* pClassisVector)
{
    return m_pBoostTree->Predict(sampleMatrix, pClassisVector);
}

