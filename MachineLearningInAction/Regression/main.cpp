
#include "LDataStruct/LMatrix.h"
#include "LMachineLearning/LRegression.h"
#include "LMachineLearning/LDataCorrelation.h"

#include "LFile/LFile.h"
#include "LDataStruct/LString.h"

#include <cstdio>
#include <vector>
using std::vector;

#include <Windows.h>

float GetCorrelation(const LRegressionMatrix& yA, const LRegressionMatrix& yB)
{
    if (yA.RowLen != yB.RowLen)
        return 0.0f;

    if (yA.ColumnLen != yB.ColumnLen)
        return 0.0f;

    if (yA.ColumnLen != 1)
        return 0.0f;

    unsigned int length = yA.RowLen;

    float sumA = 0.0f;
    float sumB = 0.0f;
    float sqrSumA = 0.0f;
    float sqrSumB = 0.0f;
    float proSum = 0.0f;
    for (unsigned int i = 0; i < length; i++)
    {
        const float& a = yA[i][0];
        const float& b = yB[i][0];

        sumA += a;
        sumB += b;

        sqrSumA += a * a;
        sqrSumB += b * b;

        proSum += a * b;
    }

    // 计算皮尔逊相关系数
    float num = proSum - (sumA * sumB / length);
    float den = sqrtf((sqrSumA - sumA * sumA / length) * (sqrSumB - sumB * sumB / length));

    if (den == 0)
        return 0;

    return num / den;
}



void PrintMatrix(const LMatrix<float>& matrix)
{
    for (unsigned int row = 0; row < matrix.RowLen; row++)
    {
        for (unsigned int col = 0; col < matrix.ColumnLen; col++)
        {
            float abc = matrix[row][col];
            printf("%f  ", abc);
        }
        printf("\n");
    }
}

int main()
{
    /*
    vector<string> lineList;
    LFileReader fileReader(".\\ex0.txt");
    fileReader.Open();
    while (!fileReader.AtEnd())
    {
    string line;
    fileReader.ReadLine(line);
    if (line.empty())
    continue;

    lineList.push_back(line);

    }
    fileReader.Close();

    for (unsigned int i = 0; i < lineList.size(); i++)
    {
    LStringList strList;
    StringSplit('\t', lineList[i].c_str(), strList);
    xMatrix[i][0] = ConvertFromString<float>(strList[1]);
    yMatrix[i][0] = ConvertFromString<float>(strList[2]);

    }
    */
    
    // 定义训练样本
    float trainX[4] = 
    {
        2.0f,
        4.0f,
        6.0f,
        8.0f
    };
    LRegressionMatrix xMatrix(4, 1, trainX);

    // 定义训练样本输出
    float trainY[4] =
    {
        1.0f,
        2.0f,
        3.0f,
        4.0f
    };
    LRegressionMatrix yMatrix(4, 1, trainY);

 
    // 定义线性回归对象
    LLinearRegression linearReg(4, 1);

    // 训练模型
    for (unsigned int i = 0; i < 500; i++)
    {
        linearReg.TrainModel(xMatrix, yMatrix, 0.01f);
    }

    // 进行预测
    LRegressionMatrix yVector;
    linearReg.Predict(xMatrix, yVector);

   // PrintMatrix(yVector);

    //system("pause");




    return 0;
}