
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

/// @brief SVM参数结构
struct LSVMParam
{
    float C; ///< 常数C, C越大表明模型对离群点越敏感(即越大的C表明越不希望有离群点)
    float Toler; ///< 容错率
    float MinAlphaChange; ///< alpha值改变的最小量
    unsigned int MaxIterCount; ///< 最大遍历次数
};

/// @brief SVM解结构
struct LSVMSolution
{
    /// @brief 构造函数
    ///  
    /// @param[in] m 样本数量
    LSVMSolution(unsigned int m)
    {
        this->AVector.Reset(m, 1);
        for (unsigned int i = 0; i < m; i++)
        {
            this->AVector[i][0] = 0.0f;
        }

        this->B = 0.0f;
    }

    float B; ///< 分割超平面的截距
    LSVMMatrix AVector; ///< alpha向量(列向量)
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

/// @brief 原始函数(不使用核函数, 直接计算内积)
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
    // 进行参数检查
    if (problem.XMatrix.ColumnLen < 1)
        return false;
    if (problem.XMatrix.RowLen < 2)
        return false;
    if (problem.YVector.ColumnLen != 1)
        return false;
    if (problem.XMatrix.RowLen != problem.YVector.RowLen)
        return false;



    m_pProblem = &problem;

    // 获取核函数接口
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

    // 初始化解结构
    if (m_pSolution != 0)
    {
        delete m_pSolution;
        m_pSolution = 0;
    }
    m_pSolution = new LSVMSolution(problem.XMatrix.RowLen);

    // 通过核函数计算所有样本之间在高纬空间的的内积, 并存储在K矩阵中
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

    // 训练模型
    this->SMOTrainModel(*m_pProblem, *m_pParam, *m_pSolution);

    // 获取支持向量的索引
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
    // 检查参数
    if (sample.RowLen != 1)
        return 0.0f;
    if (sample.ColumnLen != this->m_pProblem->XMatrix.ColumnLen)
        return 0.0f;
    if (this->m_pProblem == 0)
        return 0.0f;

    LSVMMatrix AY = LSVMMatrix::DOTMUL(this->m_pSolution->AVector, this->m_pProblem->YVector); // 列向量
    LSVMMatrix AYT = AY.T(); // 行向量
    
    LSVMMatrix sampleA;
    LSVMMatrix KColumn(this->m_pProblem->XMatrix.RowLen, 1); // 列向量
    
    // 只对支持向量做内积, 节省时间
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
    LSVMMatrix AY = LSVMMatrix::DOTMUL(solution.AVector, problem.YVector); // 列向量
    LSVMMatrix AYT = AY.T(); // 行向量
    for (unsigned int i = 0; i < problem.XMatrix.RowLen; i++)
    {
        LSVMMatrix KColumn = this->m_pKMatrix->GetColumn(i); // 列向量
        LSVMMatrix AYTK = AYT * KColumn;
        float E = AYTK[0][0] + solution.B - problem.YVector[i][0]; // 样本i标签的误差
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
    unsigned int sampleNum = sampleMatrix.RowLen; // 样本数量
    unsigned int featureNum = sampleMatrix.ColumnLen; // 样本的特征数量

	float B = 0.0f; // 分割超平面的截距

    // 将alpha向量初始化为0向量(列向量)
    alphaVector.Reset(sampleNum, 1);
    for (unsigned int i = 0; i < alphaVector.RowLen; i++)
    {
        alphaVector[i][0] = 0.0f;
    }

    int iter = 0;
    while (iter < 40)
    {
        bool alphaChanged = false;
        // 循环扫描每一个alpha, 对可优化的alpha进行优化
        for (unsigned int i = 0; i < sampleNum; i++)
        {
			// 根据现有alpha计算出第i个样本的间隔, 计算出的值不符合KKT条件表明需要优化

			// 计算权重
			// w = sum for i = 1 to m (alpha(i) * y(i) * x(i))
            LSVMMatrix W = LSVMMatrix::DOTMUL(alphaVector, classVector).T() * sampleMatrix;// 计算出的权重为行向量

			// 根据权重计算间隔
			// margin = y(i) * (W * X(i) + b)
			LSVMMatrix Xi = sampleMatrix.GetRow(i); // 行向量
            LSVMMatrix XiT = Xi.T(); // 列向量
			LSVMMatrix WXi = W * XiT;
            float Ei = WXi[0][0] + B - classVector[i][0]; // 样本i标签的误差
			float marginXi = classVector[i][0] * (WXi[0][0] + B); // 样本i的间隔
            
			// KKT条件::
			//alpha如果小于C, 那么margin应该大于等于1, 如不符合那么违反KKT条件, 则需要优化
			//alpha如果大于0, 那么margin应该小于等于1, 如不符合那么违反KKT条件, 则需要优化
			if (((alphaVector[i][0] < C) && (marginXi < 1 - toler)) ||
				((alphaVector[i][0] > 0) && (marginXi > 1 + toler)))
			{
                // 随机选择另一个alpha j
                int j = this->SelectRand(i, sampleNum-1);
                LSVMMatrix Xj = sampleMatrix.GetRow(j); // 行向量
                LSVMMatrix XjT = Xj.T(); // 列向量
                LSVMMatrix WXj = W * XjT;
                float Ej = WXj[0][0] + B - classVector[j][0]; // 样本j标签的误差
                float marginXj = classVector[j][0] * (WXj[0][0] + B); // 样本j的间隔
               
                float LAj = 0.0f; // alpha j 的下届
                float HAj = 0.0f; // alpha j 的上届
                // 计算alpha j 的上下界
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
               
                // 计算eta = 2 * Xi * Xj - Xi * Xi - Xj * Xj
                LSVMMatrix XiXj = Xi * XjT;
                LSVMMatrix XiXi = Xi * XiT;
                LSVMMatrix XjXj = Xj * XjT;

                float eta = 2 * XiXj[0][0] - XiXi[0][0] - XjXj[0][0];
                if (eta == 0.0f)
                {
                    continue;
                }
               

                // 计算新的alpha j 
                float AjOld = alphaVector[j][0];
                alphaVector[j][0] -= classVector[j][0] * (Ei - Ej) / eta;
                // 裁剪alpha j
                alphaVector[j][0] = this->ClipAlpha(alphaVector[j][0], LAj, HAj);

                // 如果alpha j 改变量太小, 则退出
                if (abs(alphaVector[j][0] - AjOld) < 0.00001)
                {
                    continue;
                }

                // 计算新的alpha i
                float AiOld = alphaVector[i][0];
                alphaVector[i][0] += classVector[i][0] * classVector[j][0] * (AjOld - alphaVector[j][0]);

                // 计算截距B的值
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
    const unsigned int M = problem.XMatrix.RowLen; // 样本数量

    LSVMMatrix E(M, 1); ///< 误差缓存向量(列向量)
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

        // 遍历整个alpha集合
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

        // 遍历不在边界上的alpha
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

       // 遍历整个集合并且alpha值都没改变, 那么结束遍历
       if (entireSet && !alphaChanged)
           break;

       // 遍历整个集合并且改变了alpha值, 那么下次切换到遍历非边界集合
       if (entireSet && alphaChanged)
           entireSet = false;

       // 遍历非边界集合并且alpha值得到了改变, 那么下次继续遍历非边界集合
       if (!entireSet && alphaChanged)
           continue;

       // 遍历非边界集合并且alpha值没有改变, 那么下次切换到遍历整个集合
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
    const LSVMMatrix& X = problem.XMatrix; // 样本矩阵
    const LSVMMatrix& Y = problem.YVector; // 标签向量(列向量)
    const unsigned int M = X.RowLen; // 样本数量
    const LSVMMatrix& K = *m_pKMatrix; // K矩阵

    const float C = param.C; // 常数C
    const float Toler = param.Toler; // 容错率
    const float MinAlphaChange = param.MinAlphaChange;

    float& B = solution.B; // 分割超平面的截距
    LSVMMatrix& A = solution.AVector; // alpha向量(列向量)

    const LSVMMatrix& E = error; ///< 误差缓存向量(列向量)

    float marginXi = E[i][0] * Y[i][0] + 1;

    // KKT条件::
    // alpha如果小于C, 那么margin应该大于等于1, 如不符合那么违反KKT条件, 则需要优化
    // alpha如果大于0, 那么margin应该小于等于1, 如不符合那么违反KKT条件, 则需要优化
    if (((A[i][0] < C) && (marginXi < 1 - Toler)) ||
        ((A[i][0] > 0) && (marginXi > 1 + Toler)))
    {
        unsigned int j = this->SelectSecondAlpha(i, E);

        float LAj = 0.0f; // alpha j 的下届
        float HAj = 0.0f; // alpha j 的上届
        // 计算alpha j 的上下界
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

        // 计算新的alpha j 
        float AjOld = A[j][0];
        A[j][0] -= Y[j][0] * (E[i][0] - E[j][0]) / eta;
        // 裁剪alpha j
        A[j][0] = this->ClipAlpha(A[j][0], LAj, HAj);

        // 如果alpha j 改变量太小, 则退出
        if (abs(A[j][0] - AjOld) < MinAlphaChange)
        {
            return false;
        }

        // 计算新的alpha i
        float AiOld = A[i][0];
        A[i][0] += Y[i][0] * Y[j][0] * (AjOld - A[j][0]);

        // 计算截距B的值
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
    
