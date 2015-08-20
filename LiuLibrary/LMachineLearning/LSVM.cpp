
#include "LSVM.h"

#include <cstdlib>
#include <ctime>
#include <cstdio>
#include <cmath>

#include <Windows.h>

#ifndef LMAX
#define LMAX(a,b)    (((a) > (b)) ? (a) : (b))
#endif

#ifndef LMIN
#define LMIN(a,b)    (((a) < (b)) ? (a) : (b))
#endif

/// @brief SVM�����ṹ
struct LSVMParam
{
    float C; ///< ����C, CԽ�����ģ�Ͷ���Ⱥ��Խ����(��Խ���C����Խ��ϣ������Ⱥ��)
    float Toler; ///< �ݴ���
    float MinAlphaChange; ///< alphaֵ�ı����С��
    unsigned int MaxIterCount; ///< ����������
};

/// @brief SVM��ṹ
struct LSVMSolution
{
    /// @brief ���캯��
    ///  
    /// @param[in] m ��������
    LSVMSolution(unsigned int m)
    {
        this->AVector.Reset(m, 1);
        for (unsigned int i = 0; i < m; i++)
        {
            this->AVector[i][0] = 0.0f;
        }

        this->B = 0.0f;
    }

    float B; ///< �ָƽ��Ľؾ�
    LSVMMatrix AVector; ///< alpha����(������)
};

LSVMRBF::LSVMRBF()
{
    m_gamma = 0.1f;
}

LSVMRBF::~LSVMRBF()
{

}

void LSVMRBF::SetGamma(IN float gamma)
{
    m_gamma = gamma;
}

float LSVMRBF::Translate(IN const LSVMMatrix& vectorA, IN const LSVMMatrix& vectorB) const 
{
    LSVMMatrix deltaRow(1, vectorA.ColumnLen);

    for (unsigned int i = 0; i < vectorA.ColumnLen; i++)
    {
        deltaRow[0][i] = vectorA[0][i] - vectorB[0][i];
    }

    LSVMMatrix K = deltaRow * deltaRow.T();

    return exp(K[0][0]/(-2 * m_gamma * m_gamma));
}

/// @brief ԭʼ����(��ʹ�ú˺���, ֱ�Ӽ����ڻ�)
class LSVMKernelFuncOriginal : public LSVMKernelFunc
{
public:
    virtual float Translate(IN const LSVMMatrix& vectorA, IN const LSVMMatrix& vectorB) const
    {
        LSVMMatrix ABT = vectorA * vectorB.T();
        return ABT[0][0];
    }
};

LSVM::LSVM()
{
    srand((unsigned int)time(0));

    m_pProblem = 0;
    m_pSolution = 0;
    m_pKMatrix = 0;
    m_pKernelFunc = new LSVMKernelFuncOriginal();
    m_bUseKernelFunc = false;

    m_pParam = new LSVMParam;
    m_pParam->C = 50.0f;
    m_pParam->Toler = 0.01f;
    m_pParam->MinAlphaChange = 0.00001f;
    m_pParam->MaxIterCount = 40;
}

LSVM::~LSVM()
{
    if (m_pSolution != 0)
    {
        delete m_pSolution;
        m_pSolution = 0;
    }

    if (m_pKMatrix != 0)
    {
        delete m_pKMatrix;
        m_pKMatrix = 0;
    }

    if (m_pParam != 0)
    {
        delete m_pParam;
        m_pParam = 0;
    }

    if (!m_bUseKernelFunc)
    {
        if (m_pKernelFunc != 0)
        {
            delete m_pKernelFunc;
            m_pKernelFunc = 0;
        }
        
    }
}

bool LSVM::TrainModel(IN const LSVMProblem& problem, IN const LSVMKernelFunc* pKernelFunc)
{
    // ���в������
    if (problem.XMatrix.ColumnLen < 1)
        return false;
    if (problem.XMatrix.RowLen < 2)
        return false;
    if (problem.YVector.ColumnLen != 1)
        return false;
    if (problem.XMatrix.RowLen != problem.YVector.RowLen)
        return false;



    m_pProblem = &problem;

    // ��ȡ�˺����ӿ�
    if (pKernelFunc != 0)
    {
        if (!m_bUseKernelFunc)
        {
            delete m_pKernelFunc;
            m_pKernelFunc = 0;
        }

        m_pKernelFunc = pKernelFunc;
        m_bUseKernelFunc = true;
    }

    if (pKernelFunc == 0)
    {
        if (m_bUseKernelFunc)
        {
            m_pKernelFunc = new LSVMKernelFuncOriginal();
            m_bUseKernelFunc = false;
        }
    }

    // ��ʼ����ṹ
    if (m_pSolution != 0)
    {
        delete m_pSolution;
        m_pSolution = 0;
    }
    m_pSolution = new LSVMSolution(problem.XMatrix.RowLen);

    // ͨ���˺���������������֮���ڸ�γ�ռ�ĵ��ڻ�, ���洢��K������
    if (m_pKMatrix != 0)
    {
        delete m_pKMatrix;
        m_pKMatrix = 0;
    }
    m_pKMatrix = new LSVMMatrix(problem.XMatrix.RowLen, problem.XMatrix.RowLen);
    LSVMMatrix sampleA;
    LSVMMatrix sampleB;
    for (unsigned int i = 0; i < problem.XMatrix.RowLen; i++)
    {
        sampleA = problem.XMatrix.GetRow(i);
        for (unsigned int j = 0; j < problem.XMatrix.RowLen; j++)
        {
            sampleB = problem.XMatrix.GetRow(j);
            (*m_pKMatrix)[i][j] = m_pKernelFunc->Translate(sampleA, sampleB);
        }
    }

    // ѵ��ģ��
    this->SMOTrainModel(*m_pProblem, *m_pParam, *m_pSolution);

    // ��ȡ֧������������
    unsigned int supportVectorNum = 0;
    for (unsigned int i = 0; i < m_pSolution->AVector.RowLen; i++)
    {
        if (m_pSolution->AVector[i][0] != 0.0f)
            supportVectorNum++;
    }
    m_supportVectorIndex.Reset(supportVectorNum, 1);
    unsigned int j = 0;
    for (unsigned int i = 0; i < m_pSolution->AVector.RowLen; i++)
    {
        if (m_pSolution->AVector[i][0] != 0.0f)
        {
            m_supportVectorIndex[j][0] = (float)i;
            j++;
        }
    }

    return true;
}

float LSVM::Predict(const LSVMMatrix& sample)
{
    // ������
    if (sample.RowLen != 1)
        return 0.0f;
    if (sample.ColumnLen != this->m_pProblem->XMatrix.ColumnLen)
        return 0.0f;
    if (this->m_pProblem == 0)
        return 0.0f;

    LSVMMatrix AY = LSVMMatrix::DOTMUL(this->m_pSolution->AVector, this->m_pProblem->YVector); // ������
    LSVMMatrix AYT = AY.T(); // ������
    
    LSVMMatrix sampleA;
    LSVMMatrix KColumn(this->m_pProblem->XMatrix.RowLen, 1); // ������
    
    // ֻ��֧���������ڻ�, ��ʡʱ��
    for (unsigned int i = 0; i < KColumn.RowLen; i++)
    {
        KColumn[i][0] = 0.0f;
    }
    for (unsigned int i = 0; i < m_supportVectorIndex.RowLen; i++)
    {
        unsigned int j = (unsigned int)m_supportVectorIndex[i][0];
        sampleA = this->m_pProblem->XMatrix.GetRow(j);
        KColumn[j][0] = m_pKernelFunc->Translate(sampleA, sample);
    }
    LSVMMatrix AYTK = AYT * KColumn;
    if (AYTK[0][0] + this->m_pSolution->B >= 0.0f)
        return 1.0f;
    else
        return -1.0f;

}

unsigned int LSVM::GetSupportVectorNumber()
{
    return m_supportVectorIndex.RowLen;
}

int LSVM::SelectRand(IN unsigned int i, IN unsigned int max)
{
    int j = i; 
    while (j == i)
    {
        j = rand()%(max + 1);
    }
    return j;
}

unsigned int LSVM::SelectSecondAlpha(IN unsigned int i, IN const LSVMMatrix& E)
{
    unsigned int j = 0;
    float maxDeltaE = 0.0f;

    for (unsigned int k = 0; k < E.RowLen; k++)
    {

        float deltaE = abs(E[i][0] - E[k][0]);
        if (deltaE > maxDeltaE)
        {
            maxDeltaE = deltaE;
            j = k;
        }
    }

    if (maxDeltaE == 0.0f)
    {
        j = this->SelectRand(i, E.RowLen-1);
    }

    return j;
    
}

float LSVM::ClipAlpha(IN float a, IN float min, IN float max)
{
    if (a > max)
        a = max;
    if (a < min)
        a = min;

    return a;
}

void LSVM::CalculateError(IN const LSVMProblem& problem, IN const LSVMSolution& solution, OUT LSVMMatrix& errorVector)
{
    LSVMMatrix AY = LSVMMatrix::DOTMUL(solution.AVector, problem.YVector); // ������
    LSVMMatrix AYT = AY.T(); // ������
    for (unsigned int i = 0; i < problem.XMatrix.RowLen; i++)
    {
        LSVMMatrix KColumn = this->m_pKMatrix->GetColumn(i); // ������
        LSVMMatrix AYTK = AYT * KColumn;
        float E = AYTK[0][0] + solution.B - problem.YVector[i][0]; // ����i��ǩ�����
        errorVector[i][0] = E;
    }
}

void LSVM::SMOTrainModelExample(
    IN const LSVMMatrix& sampleMatrix, 
    IN const LSVMMatrix& classVector, 
    IN float C,
    IN float toler, 
    OUT float& b, 
    OUT LSVMMatrix& alphaVector)
{
    unsigned int sampleNum = sampleMatrix.RowLen; // ��������
    unsigned int featureNum = sampleMatrix.ColumnLen; // ��������������

	float B = 0.0f; // �ָƽ��Ľؾ�

    // ��alpha������ʼ��Ϊ0����(������)
    alphaVector.Reset(sampleNum, 1);
    for (unsigned int i = 0; i < alphaVector.RowLen; i++)
    {
        alphaVector[i][0] = 0.0f;
    }

    int iter = 0;
    while (iter < 40)
    {
        bool alphaChanged = false;
        // ѭ��ɨ��ÿһ��alpha, �Կ��Ż���alpha�����Ż�
        for (unsigned int i = 0; i < sampleNum; i++)
        {
			// ��������alpha�������i�������ļ��, �������ֵ������KKT����������Ҫ�Ż�

			// ����Ȩ��
			// w = sum for i = 1 to m (alpha(i) * y(i) * x(i))
            LSVMMatrix W = LSVMMatrix::DOTMUL(alphaVector, classVector).T() * sampleMatrix;// �������Ȩ��Ϊ������

			// ����Ȩ�ؼ�����
			// margin = y(i) * (W * X(i) + b)
			LSVMMatrix Xi = sampleMatrix.GetRow(i); // ������
            LSVMMatrix XiT = Xi.T(); // ������
			LSVMMatrix WXi = W * XiT;
            float Ei = WXi[0][0] + B - classVector[i][0]; // ����i��ǩ�����
			float marginXi = classVector[i][0] * (WXi[0][0] + B); // ����i�ļ��
            
			// KKT����::
			//alpha���С��C, ��ômarginӦ�ô��ڵ���1, �粻������ôΥ��KKT����, ����Ҫ�Ż�
			//alpha�������0, ��ômarginӦ��С�ڵ���1, �粻������ôΥ��KKT����, ����Ҫ�Ż�
			if (((alphaVector[i][0] < C) && (marginXi < 1 - toler)) ||
				((alphaVector[i][0] > 0) && (marginXi > 1 + toler)))
			{
                // ���ѡ����һ��alpha j
                int j = this->SelectRand(i, sampleNum-1);
                LSVMMatrix Xj = sampleMatrix.GetRow(j); // ������
                LSVMMatrix XjT = Xj.T(); // ������
                LSVMMatrix WXj = W * XjT;
                float Ej = WXj[0][0] + B - classVector[j][0]; // ����j��ǩ�����
                float marginXj = classVector[j][0] * (WXj[0][0] + B); // ����j�ļ��
               
                float LAj = 0.0f; // alpha j ���½�
                float HAj = 0.0f; // alpha j ���Ͻ�
                // ����alpha j �����½�
                if (classVector[i][0] != classVector[j][0])
                {
                    LAj = LMAX(0, alphaVector[j][0] - alphaVector[i][0]);
                    HAj = LMIN(C, C + alphaVector[j][0] - alphaVector[i][0]);
                }
                if (classVector[i][0] == classVector[j][0])
                {
                    LAj = LMAX(0, alphaVector[j][0] - alphaVector[i][0] - C);
                    HAj = LMIN(C, alphaVector[j][0] - alphaVector[i][0]);
                }

               
                if (LAj == HAj)
                {
                    continue;
                }
               
                // ����eta = 2 * Xi * Xj - Xi * Xi - Xj * Xj
                LSVMMatrix XiXj = Xi * XjT;
                LSVMMatrix XiXi = Xi * XiT;
                LSVMMatrix XjXj = Xj * XjT;

                float eta = 2 * XiXj[0][0] - XiXi[0][0] - XjXj[0][0];
                if (eta == 0.0f)
                {
                    continue;
                }
               

                // �����µ�alpha j 
                float AjOld = alphaVector[j][0];
                alphaVector[j][0] -= classVector[j][0] * (Ei - Ej) / eta;
                // �ü�alpha j
                alphaVector[j][0] = this->ClipAlpha(alphaVector[j][0], LAj, HAj);

                // ���alpha j �ı���̫С, ���˳�
                if (abs(alphaVector[j][0] - AjOld) < 0.00001)
                {
                    continue;
                }

                // �����µ�alpha i
                float AiOld = alphaVector[i][0];
                alphaVector[i][0] += classVector[i][0] * classVector[j][0] * (AjOld - alphaVector[j][0]);

                // ����ؾ�B��ֵ
                float B1 = B - Ei - classVector[i][0] * (alphaVector[i][0] - AiOld) * XiXi[0][0] - 
                    classVector[j][0] * (alphaVector[j][0] - AjOld) * XiXj[0][0];
                float B2 = B - Ej - classVector[i][0] * (alphaVector[i][0] - AiOld) * XiXj[0][0] -
                    classVector[j][0] * (alphaVector[j][0] - AjOld) * XjXj[0][0];

                if (alphaVector[i][0] > 0 && alphaVector[i][0] < C)
                {
                    B = B1;
                }
                else if (alphaVector[j][0] > 0 && alphaVector[j][0] < C)
                {
                    B = B1;
                }
                else
                {
                    B = (B1 + B2)/2;
                }
               
               alphaChanged = true;
             
			}
        }
    
        if (alphaChanged)
            iter = 0;
        else
            iter++;
    }

    b = B;
}


void LSVM::SMOTrainModel(IN const LSVMProblem& problem, IN const LSVMParam& param, OUT LSVMSolution& solution)
{
    const unsigned int M = problem.XMatrix.RowLen; // ��������

    LSVMMatrix E(M, 1); ///< ��������(������)
    for (unsigned int i = 0; i < M; i++)
    {
        E[i][0] = 0.0f;
    }

    this->CalculateError(problem, solution, E);

    bool entireSet = true;
    bool alphaChanged = false;

    unsigned int iter = 0;
    while (iter < m_pParam->MaxIterCount)
    {
        alphaChanged = false;

        // ��������alpha����
        if (entireSet)
        {
            for (unsigned int i = 0; i < M; i++)
            {
                if (!this->SMOOptimizeAlpha(i, problem, param, E, solution))
                    continue;

                alphaChanged = true;
                this->CalculateError(problem, solution, E);
            }
           
        }

        // �������ڱ߽��ϵ�alpha
       if (!entireSet)
        {
            for (unsigned int i = 0; i < M; i++)
            {
                if (solution.AVector[i][0] == 0 || solution.AVector[i][0] == param.C)
                    continue;

                if (!this->SMOOptimizeAlpha(i, problem, param, E, solution))
                    continue;

                alphaChanged = true;
                this->CalculateError(problem, solution, E);
            }
        }

       iter++;

       // �����������ϲ���alphaֵ��û�ı�, ��ô��������
       if (entireSet && !alphaChanged)
           break;

       // �����������ϲ��Ҹı���alphaֵ, ��ô�´��л��������Ǳ߽缯��
       if (entireSet && alphaChanged)
           entireSet = false;

       // �����Ǳ߽缯�ϲ���alphaֵ�õ��˸ı�, ��ô�´μ��������Ǳ߽缯��
       if (!entireSet && alphaChanged)
           continue;

       // �����Ǳ߽缯�ϲ���alphaֵû�иı�, ��ô�´��л���������������
       if (!entireSet && !alphaChanged)
           entireSet = true;
    }

}

bool LSVM::SMOOptimizeAlpha(
    IN unsigned int fristAlpha, 
    IN const LSVMProblem& problem, 
    IN const LSVMParam& param, 
    IN const LSVMMatrix& error, 
    OUT LSVMSolution& solution)
{
    const unsigned int i = fristAlpha;
    const LSVMMatrix& X = problem.XMatrix; // ��������
    const LSVMMatrix& Y = problem.YVector; // ��ǩ����(������)
    const unsigned int M = X.RowLen; // ��������
    const LSVMMatrix& K = *m_pKMatrix; // K����

    const float C = param.C; // ����C
    const float Toler = param.Toler; // �ݴ���
    const float MinAlphaChange = param.MinAlphaChange;

    float& B = solution.B; // �ָƽ��Ľؾ�
    LSVMMatrix& A = solution.AVector; // alpha����(������)

    const LSVMMatrix& E = error; ///< ��������(������)

    float marginXi = E[i][0] * Y[i][0] + 1;

    // KKT����::
    // alpha���С��C, ��ômarginӦ�ô��ڵ���1, �粻������ôΥ��KKT����, ����Ҫ�Ż�
    // alpha�������0, ��ômarginӦ��С�ڵ���1, �粻������ôΥ��KKT����, ����Ҫ�Ż�
    if (((A[i][0] < C) && (marginXi < 1 - Toler)) ||
        ((A[i][0] > 0) && (marginXi > 1 + Toler)))
    {
        unsigned int j = this->SelectSecondAlpha(i, E);

        float LAj = 0.0f; // alpha j ���½�
        float HAj = 0.0f; // alpha j ���Ͻ�
        // ����alpha j �����½�
        if (Y[i][0] != Y[j][0])
        {
            LAj = LMAX(0, A[j][0] - A[i][0]);
            HAj = LMIN(C, C + A[j][0] - A[i][0]);
        }
        if (Y[i][0] == Y[j][0])
        {
            LAj = LMAX(0, A[j][0] - A[i][0] - C);
            HAj = LMIN(C, A[j][0] - A[i][0]);
        }

        if (LAj == HAj)
        {
            return false;
        }

        float eta = 2 * K[i][j] - K[i][i] - K[j][j];

        if (eta >= 0.0f)
        {
            return false;
        }

        // �����µ�alpha j 
        float AjOld = A[j][0];
        A[j][0] -= Y[j][0] * (E[i][0] - E[j][0]) / eta;
        // �ü�alpha j
        A[j][0] = this->ClipAlpha(A[j][0], LAj, HAj);

        // ���alpha j �ı���̫С, ���˳�
        if (abs(A[j][0] - AjOld) < MinAlphaChange)
        {
            return false;
        }

        // �����µ�alpha i
        float AiOld = A[i][0];
        A[i][0] += Y[i][0] * Y[j][0] * (AjOld - A[j][0]);

        // ����ؾ�B��ֵ
        float B1 = B - E[i][0] - Y[i][0] * (A[i][0] - AiOld) * K[i][i] - 
            Y[j][0] * (A[j][0] - AjOld) * K[i][j];
        float B2 = B - E[j][0] - Y[i][0] * (A[i][0] - AiOld) * K[i][j] -
            Y[j][0] * (A[j][0] - AjOld) * K[j][j];

        if (A[i][0] > 0 && A[i][0] < C)
        {
            B = B1;
        }
        else if (A[j][0] > 0 && A[j][0] < C)
        {
            B = B1;
        }
        else
        {
            B = (B1 + B2)/2;
        }

        return true;

    }

    return false;
}
    
