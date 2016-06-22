
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

LSVMKRBF::LSVMKRBF(IN float gamma)
{
    m_gamma = gamma;
}

LSVMKRBF::~LSVMKRBF()
{

}

float LSVMKRBF::Translate(IN const LSVMMatrix& vectorA, IN const LSVMMatrix& vectorB) 
{
	LSVMMatrix::SUB(vectorA, vectorB, m_deltaRow);
	LSVMMatrix::T(m_deltaRow, m_deltaRowT);
	LSVMMatrix::MUL(m_deltaRow, m_deltaRowT, m_k);

    return exp(m_k[0][0]/(-2 * m_gamma * m_gamma));
}

/// @brief ԭʼ����(��ʹ�ú˺���, ֱ�Ӽ����ڻ�)
class LSVMKOriginal : public ISVMKernelFunc
{
public:
    /// @brief ���캯��
    LSVMKOriginal()
    {

    }

    /// @brief ��������
    ~LSVMKOriginal()
    {

    }

    /// @brief ת������
    virtual float Translate(IN const LSVMMatrix& vectorA, IN const LSVMMatrix& vectorB)
    {
		LSVMMatrix::T(vectorB, m_bT);
		LSVMMatrix::MUL(vectorA, m_bT, m_abT);
        return m_abT[0][0];
    }

private:

	/*
	���±�������Ϊ��Ա����Ϊ�Ż�����Ч��Ŀ��
	*/
	LSVMMatrix m_bT;
	LSVMMatrix m_ab;
	LSVMMatrix m_abT;
};

/// @brief ֧��������ʵ����
class CSVM
{
public:
    /// @brief ���캯��
    explicit CSVM(IN const LSVMParam& param)
    {
        srand((unsigned int)time(0));

        m_pProblem = 0;
        m_pSolution = 0;
        m_pKMatrix = 0;
        m_pKernelFunc = NULL;

        m_pParam = new LSVMParam;
		(*m_pParam) = param;

		// ��ȡ�˺����ӿ�
		if (m_pParam->PKernelFunc != 0)
		{
			m_pKernelFunc = m_pParam->PKernelFunc;
		}
		else
		{
			m_pKernelFunc = &m_kOriginal;
		}
    }

    /// @brief ��������
    ~CSVM()
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
    }

    /// @brief ѵ��ģ��
    bool TrainModel(IN const LSVMProblem& problem, OUT LSVMResult& result)
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
            problem.XMatrix.GetRow(i, sampleA);
            for (unsigned int j = 0; j < problem.XMatrix.RowLen; j++)
            {
                problem.XMatrix.GetRow(j, sampleB);
                (*m_pKMatrix)[i][j] = m_pKernelFunc->Translate(sampleA, sampleB);
            }
        }

        // ѵ��ģ��
        result.IterCount = this->SMOTrainModel(*m_pProblem, *m_pSolution);

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
                m_supportVectorIndex[j][0] = i;
                j++;
            }
        }

		result.SupportVevtorNum = supportVectorNum;


        return true;
    }

    /// @brief ʹ��ѵ���õ�ģ�ͽ���Ԥ��
	bool Predict(IN const LSVMMatrix& sampleSet, OUT LSVMMatrix& yVector)
    {
        // ������
		if (this->m_pProblem == 0)
			return false;
        if (sampleSet.RowLen < 1)
            return false;
        if (sampleSet.ColumnLen != this->m_pProblem->XMatrix.ColumnLen)
            return false;

		yVector.Reset(sampleSet.RowLen, 1);
       

        LSVMMatrix AY;
        LSVMMatrix::DOTMUL(this->m_pSolution->AVector, this->m_pProblem->YVector, AY); // ������
        LSVMMatrix AYT = AY.T(); // ������

        LSVMMatrix sampleA;
		LSVMMatrix sampleB;
        LSVMMatrix KColumn(this->m_pProblem->XMatrix.RowLen, 1); // ������

		for (unsigned int row = 0; row < sampleSet.RowLen; row++)
		{
			sampleSet.GetRow(row, sampleB);

			// ֻ��֧���������ڻ�, ��ʡʱ��
			for (unsigned int i = 0; i < KColumn.RowLen; i++)
			{
				KColumn[i][0] = 0.0f;
			}
			for (unsigned int i = 0; i < m_supportVectorIndex.RowLen; i++)
			{
				unsigned int j = m_supportVectorIndex[i][0];
				sampleA = this->m_pProblem->XMatrix.GetRow(j);
				KColumn[j][0] = m_pKernelFunc->Translate(sampleA, sampleB);
			}
			LSVMMatrix AYTK = AYT * KColumn;
			if (AYTK[0][0] + this->m_pSolution->B >= 0.0f)
				yVector[row][0] = 1.0f;
			else
				yVector[row][0] = -1.0f;
		}
        
		return true;

    }

private:
    /// @brief ����һ��������i�������, ��Χ[0, max]
    /// @param[in] i ������б����˵�ֵ
    /// @param[in] max ������е����ֵ, Ҫ�����0
    /// @return �����
    int SelectRand(IN unsigned int i, IN unsigned int max)
    {
        int j = i; 
        while (j == i)
        {
            j = rand()%(max + 1);
        }
        return j;
    }

    /// @brief ʹ������ʽ����ѡ��ڶ���alpha
    /// @param[in] i ��һ��alpha����
    /// @param[in] E ���������������
    /// @return �ڶ���alpha����
    unsigned int SelectSecondAlpha(IN unsigned int i, IN const LSVMMatrix& E)
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

    /// @brief ����aֵ
    ///  aӦ����min~max, ��a > maxʱa = max, ��a < minʱa = min
    /// @param[in] a ��Ҫ������ֵ
    /// @param[in] min ��Сֵ
    /// @param[in] max ���ֵ
    /// @return �������ֵ
    float ClipAlpha(IN float a, IN float min, IN float max)
    {
        if (a > max)
            a = max;
        if (a < min)
            a = min;

        return a;
    }

    /// @brief ���ݽ������������������
    /// @param[in] solution ��
    /// @param[in] k ��������
    /// @return ���ֵ
    void CalculateError(IN const LSVMSolution& solution, OUT LSVMMatrix& errorVector)
    {
        LSVMMatrix AY;
        LSVMMatrix::DOTMUL(solution.AVector, m_pProblem->YVector, AY); // ������
        LSVMMatrix AYT = AY.T(); // ������

		LSVMMatrix KColumn;
		LSVMMatrix AYTK;
        for (unsigned int i = 0; i < m_pProblem->XMatrix.RowLen; i++)
        {
            m_pKMatrix->GetColumn(i, KColumn); // ������
			LSVMMatrix::MUL(AYT, KColumn, AYTK);
            float E = AYTK[0][0] + solution.B - m_pProblem->YVector[i][0]; // ����i��ǩ�����
            errorVector[i][0] = E;
        }
    }

    /// @brief SMOѵ���㷨
    /// @param[in] problem ԭʼ����
    /// @param[out] solution ����Ľ�
	/// @return ��������
    unsigned int SMOTrainModel(IN const LSVMProblem& problem, OUT LSVMSolution& solution)
    {
        const unsigned int M = problem.XMatrix.RowLen; // ��������

        LSVMMatrix E(M, 1); ///< ��������(������)
        for (unsigned int i = 0; i < M; i++)
        {
            E[i][0] = 0.0f;
        }

        this->CalculateError(solution, E);

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
                    if (!this->SMOOptimizeAlpha(i, E, solution))
                        continue;

                    alphaChanged = true;
                    this->CalculateError(solution, E);
                }

            }

            // �������ڱ߽��ϵ�alpha
            if (!entireSet)
            {
                for (unsigned int i = 0; i < M; i++)
                {
                    if (solution.AVector[i][0] == 0 || solution.AVector[i][0] == m_pParam->C)
                        continue;

                    if (!this->SMOOptimizeAlpha(i, E, solution))
                        continue;

                    alphaChanged = true;
                    this->CalculateError(solution, E);
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

		return iter;

    }

    /// @brief SMO�Ż�Alpha 
    /// @param[in] fristAlpha ��Ҫ�Ż���alpha������
    /// @param[in] error ��������
    /// @param[out] solution ����Ľ�
    /// @return �ɹ��Ż�����true, �Ż�ʧ�ܷ���false
    bool SMOOptimizeAlpha(
        IN unsigned int fristAlpha,   
        IN const LSVMMatrix& error, 
        OUT LSVMSolution& solution)
    {
        const unsigned int i = fristAlpha;
        const LSVMMatrix& X = m_pProblem->XMatrix; // ��������
        const LSVMMatrix& Y = m_pProblem->YVector; // ��ǩ����(������)
        const unsigned int M = X.RowLen; // ��������
        const LSVMMatrix& K = *m_pKMatrix; // K����

        const float C = m_pParam->C; // ����C

        float& B = solution.B; // �ָƽ��Ľؾ�
        LSVMMatrix& A = solution.AVector; // alpha����(������)

        const LSVMMatrix& E = error; ///< ��������(������)

        float marginXi = E[i][0] * Y[i][0] + 1;

        // KKT����::
        // alpha���С��C, ��ômarginӦ�ô��ڵ���1, �粻������ôΥ��KKT����, ����Ҫ�Ż�
        // alpha�������0, ��ômarginӦ��С�ڵ���1, �粻������ôΥ��KKT����, ����Ҫ�Ż�
        if (((A[i][0] < C) && (marginXi < 1)) ||
            ((A[i][0] > 0) && (marginXi > 1)))
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

            if (eta == 0.0f)
            {
                return false;
            }

            // �����µ�alpha j 
            float AjOld = A[j][0];
            A[j][0] -= Y[j][0] * (E[i][0] - E[j][0]) / eta;
            // �ü�alpha j
            A[j][0] = this->ClipAlpha(A[j][0], LAj, HAj);

			// ���alpha j �ı���̫С, ���˳�
			if (abs(A[j][0] - AjOld) < 0.0001)
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

private:
    const LSVMProblem* m_pProblem; ///< ԭʼ����
    LSVMParam* m_pParam; ///< ����
    LSVMSolution* m_pSolution; ///< SVM�Ľ�
    LSVMMatrix* m_pKMatrix; ///< K����
    ISVMKernelFunc* m_pKernelFunc; ///< �˺����ӿ�
    LSVMKOriginal m_kOriginal; ///< ԭʼ�˺���
    LMatrix<unsigned int> m_supportVectorIndex; ///< ��¼֧������������������(������)
};

LSVM::LSVM(IN const LSVMParam& param)
{
    m_pSVM = new CSVM(param);
}

LSVM::~LSVM()
{
    if (m_pSVM != 0)
    {
        delete m_pSVM;
    }
}

bool LSVM::TrainModel(IN const LSVMProblem& problem, OUT LSVMResult& result)
{
    return m_pSVM->TrainModel(problem, result);
}

bool LSVM::Predict(IN const LSVMMatrix& sampleSet, OUT LSVMMatrix& yVector)
{
    return m_pSVM->Predict(sampleSet, yVector);
}




