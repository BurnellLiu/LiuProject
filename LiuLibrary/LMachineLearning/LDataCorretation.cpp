
#include "LDataCorretation.h"

#include <math.h>


float LEuclideanDistance::Calculate(IN const LDSDataList& dataListA, IN const LDSDataList& dataListB)
{
    if (dataListA.Length != dataListB.Length)
        return 0;

    float sqrSum = 0.0f;
    for (int i = 0; i < dataListA.Length; i++)
    {
        float dif = dataListA.Data[i] - dataListB.Data[i];
        sqrSum += dif * dif;
    }

    return 1.0f/(1.0f + sqrtf(sqrSum));
}

float LPearsonCorretation::Calculate(IN const LDSDataList& dataListA, IN const LDSDataList& dataListB)
{
    if (dataListA.Length != dataListB.Length)
        return 0;

    int length = dataListA.Length;

    float sumA = 0.0f;
    float sumB = 0.0f;
    float sqrSumA = 0.0f;
    float sqrSumB = 0.0f;
    float proSum = 0.0f;
    for (int i = 0; i < length; i++)
    {
        const float& a = dataListA.Data[i];
        const float& b = dataListB.Data[i];

        sumA += a;
        sumB += b;

        sqrSumA += a * a;
        sqrSumB += b * b;

        proSum += a * b;
    }

    // 计算皮尔逊相关系数
    float num = proSum - (sumA * sumB/length);
    float den = sqrtf((sqrSumA - sumA * sumA/length) * (sqrSumB - sumB * sumB/length));

    if (den == 0)
        return 0;

    return num/den;
}

float LTanimotoCoefficient::Calculate(IN const LDSDataList& dataListA, IN const LDSDataList& dataListB)
{
    if (dataListA.Length != dataListB.Length)
        return 0;

    int coutA = 0;
    int countB = 0;
    int countAB = 0;
    for (int i = 0; i < dataListA.Length; i++)
    {
        if (dataListA.Data[i] == m_sameValue)
            coutA++;
        if (dataListB.Data[i] == m_sameValue)
            countB++;
        if (dataListA.Data[i] == m_sameValue && dataListB.Data[i] == m_sameValue)
            countAB++;

    }

    if (countAB == 0)
        return 0.0f;

    return (float)countAB/(float)(coutA + countB - countAB);
}

LTanimotoCoefficient::LTanimotoCoefficient()
{
    m_sameValue = 1.0f;
}

LTanimotoCoefficient::~LTanimotoCoefficient()
{

}

void LTanimotoCoefficient::SetSameValue(IN float sameValue)
{
    m_sameValue = sameValue;
}